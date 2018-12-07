#!/bin/sh
cd /opt/hotc/bin

if [ -f '/opt/hotc/bin/data-dir/config.ini' ]; then
    echo
  else
    cp /config.ini /opt/hotc/bin/data-dir
fi

if [ -f '/opt/hotc/bin/data-dir/genesis.json' ]; then
    echo
  else
    cp /genesis.json /opt/hotc/bin/data-dir
fi

if [ -d '/opt/hotc/bin/data-dir/contracts' ]; then
    echo
  else
    cp -r /contracts /opt/hotc/bin/data-dir
fi

exec /opt/hotc/bin/hotcd $@
