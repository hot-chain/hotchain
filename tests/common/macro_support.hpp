/**
 * @file Contains support macros for the testcase helper macros. These macros are implementation details, and thus
 * should not be used directly. Use their frontends instead.
 */

#define MKCHAIN1(name) \
   chainbase::database name ## _db(get_temp_dir(), chainbase::database::read_write, TEST_DB_SIZE); \
   block_log name ## _log(get_temp_dir() / "blocklog"); \
   fork_database name ## _fdb; \
   native_contract::native_contract_chain_initializer name ## _initializer(genesis_state()); \
   testing_blockchain name(name ## _db, name ## _fdb, name ## _log, name ## _initializer, *this); \
   BOOST_TEST_CHECKPOINT("Created blockchain " << #name);
#define MKCHAIN2(name, id) \
   chainbase::database name ## _db(get_temp_dir(#id), chainbase::database::read_write, TEST_DB_SIZE); \
   block_log name ## _log(get_temp_dir(#id) / "blocklog"); \
   fork_database name ## _fdb; \
   native_contract::native_contract_chain_initializer name ## _initializer(genesis_state()); \
   testing_blockchain name(name ## _db, name ## _fdb, name ## _log, name ## _initializer, *this); \
   BOOST_TEST_CHECKPOINT("Created blockchain " << #name);
#define MKCHAINS_MACRO(x, y, name) Make_Blockchain(name)

#define MKNET1(name) testing_network name; BOOST_TEST_CHECKPOINT("Created testnet " << #name);
#define MKNET2_MACRO(x, name, chain) name.connect_blockchain(chain);
#define MKNET2(name, ...) MKNET1(name) BOOST_PP_SEQ_FOR_EACH(MKNET2_MACRO, name, __VA_ARGS__)

inline std::vector<Name> sort_names( std::vector<Name>&& names ) {
   std::sort( names.begin(), names.end() );
   auto itr = std::unique( names.begin(), names.end() );
   names.erase( itr, names.end() );
   return names;
}

#define MKACCT_IMPL(chain, name, creator, active, owner, recovery, deposit) \
   { \
      hotc::chain::SignedTransaction trx; \
      trx.scope = sort_names({ #creator, "system" }); \
      trx.emplaceMessage(config::SystemContractName, \
                         vector<AccountName>{#creator, config::StakedBalanceContractName, config::HotcContractName}, \
                         vector<types::AccountPermission>{}, \
                         "newaccount", types::newaccount{#creator, #name, owner, active, recovery, deposit}); \
      boost::sort(trx.messages.back().recipients); \
      trx.expiration = chain.head_block_time() + 100; \
      trx.set_reference_block(chain.head_block_id()); \
      chain.push_transaction(trx); \
      BOOST_TEST_CHECKPOINT("Created account " << #name); \
   }
#define MKACCT2(chain, name) \
   Make_Key(name) \
   MKACCT_IMPL(chain, name, inita, Key_Authority(name ## _public_key), Key_Authority(name ## _public_key), \
               Account_Authority(inita), Asset(100))
#define MKACCT3(chain, name, creator) \
   Make_Key(name) \
   MKACCT_IMPL(chain, name, creator, Key_Authority(name ## _public_key), Key_Authority(name ## _public_key), \
               Account_Authority(creator), Asset(100))
#define MKACCT4(chain, name, creator, deposit) \
   Make_Key(name) \
   MKACCT_IMPL(chain, name, creator, Key_Authority(name ## _public_key), Key_Authority(name ## _public_key), \
               Account_Authority(creator), deposit)
#define MKACCT5(chain, name, creator, deposit, owner) \
   Make_Key(name) \
   MKACCT_IMPL(chain, name, creator, owner, Key_Authority(name ## _public_key), Account_Authority(creator), deposit)
#define MKACCT6(chain, name, creator, deposit, owner, active) \
   MKACCT_IMPL(chain, name, creator, owner, active, Account_Authority(creator), deposit)
#define MKACCT7(chain, name, creator, deposit, owner, active, recovery) \
   MKACCT_IMPL(chain, name, creator, owner, active, recovery, deposit)

#define XFER5(chain, sender, recipient, Amount, memo) \
   { \
      hotc::chain::SignedTransaction trx; \
      trx.scope = sort_names({#sender,#recipient}); \
      trx.emplaceMessage(config::HotcContractName, vector<AccountName>{#sender, #recipient}, \
                         vector<types::AccountPermission>{}, \
                         "transfer", types::transfer{#sender, #recipient, Amount.amount/*, memo*/}); \
      boost::sort(trx.messages.back().recipients); \
      trx.expiration = chain.head_block_time() + 100; \
      trx.set_reference_block(chain.head_block_id()); \
      chain.push_transaction(trx); \
      BOOST_TEST_CHECKPOINT("Transfered " << Amount << " from " << #sender << " to " << #recipient); \
   }
#define XFER4(chain, sender, recipient, amount) XFER5(chain, sender, recipient, amount, "")

#define STAKE4(chain, sender, recipient, amount) \
   { \
      hotc::chain::SignedTransaction trx; \
      trx.scope = sort_names( { #sender, #recipient } ); \
      trx.emplaceMessage(config::HotcContractName, vector<AccountName>{#sender, config::StakedBalanceContractName}, \
                         vector<types::AccountPermission>{}, "lock", types::lock{#sender, #recipient, amount}); \
      if (std::string(#sender) != std::string(#recipient)) { \
         trx.messages.front().recipients.emplace_back(#recipient); \
         boost::sort(trx.messages.front().recipients); \
      } \
      trx.expiration = chain.head_block_time() + 100; \
      trx.set_reference_block(chain.head_block_id()); \
      chain.push_transaction(trx); \
      BOOST_TEST_CHECKPOINT("Staked " << amount << " to " << #recipient); \
   }
#define STAKE3(chain, account, amount) STAKE4(chain, account, account, amount)

#define BEGIN_UNSTAKE3(chain, account, amount) \
   { \
      hotc::chain::SignedTransaction trx; \
      trx.scope = sort_names( { "staked" } ); \
      trx.emplaceMessage(config::StakedBalanceContractName, vector<AccountName>{#account}, \
                         vector<types::AccountPermission>{}, \
                         "unlock", types::unlock{#account, amount}); \
      trx.expiration = chain.head_block_time() + 100; \
      trx.set_reference_block(chain.head_block_id()); \
      chain.push_transaction(trx); \
      BOOST_TEST_CHECKPOINT("Begin unstake " << amount << " to " << #account); \
   }

#define FINISH_UNSTAKE3(chain, account, amount) \
   { \
      hotc::chain::SignedTransaction trx; \
      trx.scope = sort_names( { "staked", #account } ); \
      trx.emplaceMessage(config::StakedBalanceContractName, vector<AccountName>{#account, config::HotcContractName}, \
                         vector<types::AccountPermission>{}, "claim", types::claim{#account, amount}); \
      boost::sort(trx.messages.back().recipients); \
      trx.expiration = chain.head_block_time() + 100; \
      trx.set_reference_block(chain.head_block_id()); \
      chain.push_transaction(trx); \
      BOOST_TEST_CHECKPOINT("Finish unstake " << amount << " to " << #account); \
   }

#define MKPDCR4(chain, owner, key, cfg) \
   { \
      hotc::chain::SignedTransaction trx; \
      trx.scope = sort_names( {#owner, "staked"} ); \
      trx.emplaceMessage(config::StakedBalanceContractName, vector<AccountName>{#owner}, \
                         vector<types::AccountPermission>{}, \
                         "setproducer", types::setproducer{#owner, key, cfg}); \
      trx.expiration = chain.head_block_time() + 100; \
      trx.set_reference_block(chain.head_block_id()); \
      chain.push_transaction(trx); \
      BOOST_TEST_CHECKPOINT("Create producer " << #owner); \
   }
#define MKPDCR3(chain, owner, key) MKPDCR4(chain, owner, key, BlockchainConfiguration{});
#define MKPDCR2(chain, owner) \
   Make_Key(owner ## _producer); \
   MKPDCR4(chain, owner, owner ## _producer_public_key, BlockchainConfiguration{});

#define APPDCR4(chain, voter, producer, approved) \
   { \
      hotc::chain::SignedTransaction trx; \
      trx.scope = sort_names( {#voter, "staked"} ); \
      trx.emplaceMessage(config::StakedBalanceContractName, vector<AccountName>{#voter, #producer}, \
                         vector<types::AccountPermission>{}, \
                         "okproducer", types::okproducer{0, 1, approved? 1 : 0}); \
      boost::sort(trx.messages.back().recipients); \
      trx.expiration = chain.head_block_time() + 100; \
      trx.set_reference_block(chain.head_block_id()); \
      chain.push_transaction(trx); \
      BOOST_TEST_CHECKPOINT("Set producer approval from " << #voter << " for " << #producer << " to " << approved); \
   }

#define UPPDCR4(chain, owner, key, cfg) \
   { \
      hotc::chain::SignedTransaction trx; \
      trx.scope = sort_names( {#owner, "staked"} ); \
      trx.emplaceMessage(config::StakedBalanceContractName, vector<AccountName>{owner}, \
                         vector<types::AccountPermission>{}, \
                         "setproducer", types::setproducer{owner, key, cfg}); \
      trx.expiration = chain.head_block_time() + 100; \
      trx.set_reference_block(chain.head_block_id()); \
      chain.push_transaction(trx); \
      BOOST_TEST_CHECKPOINT("Update producer " << owner); \
   }
#define UPPDCR3(chain, owner, key) UPPDCR4(chain, owner, key, chain.get_producer(owner).configuration)
