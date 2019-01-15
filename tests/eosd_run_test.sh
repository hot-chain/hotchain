#!/bin/bash

error()
{
  (>&2 echo $1)
  cleanup
  exit 1
}

verifyErrorCode()
{
  rc=$?
  if [[ $rc != 0 ]]; then
    error "$1 returned error code $rc"
  fi
}

cleanup()
{
  programs/launcher/launcher -k 9
  kill -9 $WALLETD_PROC_ID
  rm -rf tn_data_0
  rm -rf tn_wallet_0
}

INITA_PRV_KEY="5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3"

# hotcd
programs/launcher/launcher -p 1
verifyErrorCode "launcher"
sleep 9
count=`grep -c "generated block" tn_data_0/stderr.txt`
if [ $count == 0 ]; then
  error "FAILURE - no blocks produced"
fi

# create 2 keys
KEYS="$(programs/hotcc/hotcc create key)"
verifyErrorCode "hotcc create key"
PRV_KEY1="$(echo "$KEYS" | awk '/Private/ {print $3}')"
PUB_KEY1="$(echo "$KEYS" | awk '/Public/ {print $3}')"
KEYS="$(programs/hotcc/hotcc create key)"
verifyErrorCode "hotcc create key"
PRV_KEY2="$(echo "$KEYS" | awk '/Private/ {print $3}')"
PUB_KEY2="$(echo "$KEYS" | awk '/Public/ {print $3}')"
if [ -z "$PRV_KEY1" ] || [ -z "$PRV_KEY2" ] || [ -z "$PUB_KEY1" ] || [ -z "$PUB_KEY2" ]; then
  error "FAILURE - create keys"
fi

# walletd
programs/hotc-walletd/hotc-walletd --data-dir tn_wallet_0 --http-server-endpoint=127.0.0.1:8899 > test_walletd_output.log 2>&1 &
verifyErrorCode "hotc-walletd"
WALLETD_PROC_ID=$!
sleep 3

# import into a wallet
PASSWORD="$(programs/hotcc/hotcc --wallet-port 8899 wallet create --name test)"
verifyErrorCode "hotcc wallet create"
programs/hotcc/hotcc --wallet-port 8899 wallet import --name test $PRV_KEY1
verifyErrorCode "hotcc wallet import"
programs/hotcc/hotcc --wallet-port 8899 wallet import --name test $PRV_KEY2
verifyErrorCode "hotcc wallet import"
programs/hotcc/hotcc --wallet-port 8899 wallet import --name test $INITA_PRV_KEY
verifyErrorCode "hotcc wallet import"

# create new account
programs/hotcc/hotcc --wallet-port 8899 create account inita tester $PUB_KEY1 $PUB_KEY2
verifyErrorCode "hotcc create account"

# verify account created
ACCOUNT_INFO="$(programs/hotcc/hotcc --wallet-port 8899 get account tester)"
verifyErrorCode "hotcc get account"
count=`echo $ACCOUNT_INFO | grep -c "exception"`
if [ $count != 0 ]; then
  error "FAILURE - account creation caused exception: $ACCOUNT_INFO"
fi
count=`echo $ACCOUNT_INFO | grep -c "staked_balance"`
if [ $count == 0 ]; then
  error "FAILURE - account creation failed: $ACCOUNT_INFO"
fi

# transfer
programs/hotcc/hotcc --wallet-port 8899 transfer inita tester 975321 "test transfer"
verifyErrorCode "hotcc transfer"

# verify transfer
ACCOUNT_INFO="$(programs/hotcc/hotcc --wallet-port 8899 get account tester)"
count=`echo $ACCOUNT_INFO | grep -c "exception"`
if [ $count != 0 ]; then
  error "FAILURE - transfer caused exception: $ACCOUNT_INFO"
fi
count=`echo $ACCOUNT_INFO | grep -c "97.5321"`
if [ $count == 0 ]; then
  error "FAILURE - transfer failed: $ACCOUNT_INFO"
fi

cleanup
echo SUCCESS!
