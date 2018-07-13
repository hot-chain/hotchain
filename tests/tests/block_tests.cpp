/*
 * Copyright (c) 2017, Respective Authors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <boost/test/unit_test.hpp>

#include <HOTC/chain/database.hpp>
#include <HOTC/chain/exceptions.hpp>
#include <HOTC/chain/account_object.hpp>
#include <HOTC/chain/sys_contract.hpp>

#include <HOTC/utilities/tempdir.hpp>

#include <fc/crypto/digest.hpp>

#include "../common/database_fixture.hpp"

using namespace HOTC;
using namespace chain;

BOOST_AUTO_TEST_SUITE(block_tests)

// Simple test of block production and head_block_num tracking
BOOST_FIXTURE_TEST_CASE(produce_blocks, testing_fixture)
{ try {
      MKDB(db)

      BOOST_CHECK_EQUAL(db.head_block_num(), 0);
      db.produce_blocks();
      BOOST_CHECK_EQUAL(db.head_block_num(), 1);
      db.produce_blocks(5);
      BOOST_CHECK_EQUAL(db.head_block_num(), 6);
      db.produce_blocks(db.get_global_properties().active_producers.size());
      BOOST_CHECK_EQUAL(db.head_block_num(), db.get_global_properties().active_producers.size() + 6);
} FC_LOG_AND_RETHROW() }

// Simple test to verify a simple transfer transaction works
BOOST_FIXTURE_TEST_CASE(transfer, testing_fixture)
{ try {
      MKDB(db)

      BOOST_CHECK_EQUAL(db.head_block_num(), 0);
      db.produce_blocks(10);
      BOOST_CHECK_EQUAL(db.head_block_num(), 10);

      signed_transaction trx;
      BOOST_REQUIRE_THROW( db.push_transaction(trx), fc::assert_exception ); /// no messages
      trx.messages.resize(1);
      trx.set_reference_block( db.head_block_id() );
      trx.set_expiration( db.head_block_time() );
      trx.messages[0].sender = "init1";
      trx.messages[0].recipient = "sys";
      trx.messages[0].type = "Transfer";
      trx.messages[0].set( "Transfer", HOTC::chain::Transfer{ "init2", 100, "memo" } );
      BOOST_REQUIRE_THROW( db.push_transaction(trx), fc::assert_exception ); // "fail to notify receiver, init2"
      trx.messages[0].notify = {"init2"};
      trx.messages[0].set( "Transfer", HOTC::chain::Transfer{ "init2", 100, "memo" } );
      db.push_transaction(trx);

      BOOST_CHECK_EQUAL( db.get_account( "init1" ).balance, 100000 - 100 );
      BOOST_CHECK_EQUAL( db.get_account( "init2" ).balance, 100000 + 100 );
      db.produce_blocks(1);

      BOOST_REQUIRE_THROW( db.push_transaction(trx), fc::assert_exception ); /// no messages



} FC_LOG_AND_RETHROW() }


// Simple test of block production when a block is missed
BOOST_FIXTURE_TEST_CASE(missed_blocks, testing_fixture)
{ try {
      MKDB(db)

      db.produce_blocks();
      BOOST_CHECK_EQUAL(db.head_block_num(), 1);

      producer_id_type skipped_producers[3] = {db.get_scheduled_producer(1),
                                               db.get_scheduled_producer(2),
                                               db.get_scheduled_producer(3)};
      auto next_block_time = db.get_slot_time(4);
      auto next_producer = db.get_scheduled_producer(4);

      BOOST_CHECK_EQUAL(db.head_block_num(), 1);
      db.produce_blocks(1, 3);
      BOOST_CHECK_EQUAL(db.head_block_num(), 2);
      BOOST_CHECK_EQUAL(db.head_block_time().to_iso_string(), next_block_time.to_iso_string());
      BOOST_CHECK_EQUAL(db.head_block_producer()._id, next_producer._id);
      BOOST_CHECK_EQUAL(db.get(next_producer).total_missed, 0);

      for (auto producer : skipped_producers) {
         BOOST_CHECK_EQUAL(db.get(producer).total_missed, 1);
      }
} FC_LOG_AND_RETHROW() }

// Simple sanity test of test network: if databases aren't connected to the network, they don't sync to eachother
BOOST_FIXTURE_TEST_CASE(no_network, testing_fixture)
{ try {
      MKDBS((db1)(db2))

      BOOST_CHECK_EQUAL(db1.head_block_num(), 0);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 0);
      db1.produce_blocks();
      BOOST_CHECK_EQUAL(db1.head_block_num(), 1);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 0);
      db2.produce_blocks(5);
      BOOST_CHECK_EQUAL(db1.head_block_num(), 1);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 5);
} FC_LOG_AND_RETHROW() }

// Test that two databases on the same network do sync to eachother
BOOST_FIXTURE_TEST_CASE(simple_network, testing_fixture)
{ try {
      MKDBS((db1)(db2))
      MKNET(net, (db1)(db2))

      BOOST_CHECK_EQUAL(db1.head_block_num(), 0);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 0);
      db1.produce_blocks();
      BOOST_CHECK_EQUAL(db1.head_block_num(), 1);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 1);
      BOOST_CHECK_EQUAL(db1.head_block_id().str(), db2.head_block_id().str());
      db2.produce_blocks(5);
      BOOST_CHECK_EQUAL(db1.head_block_num(), 6);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 6);
      BOOST_CHECK_EQUAL(db1.head_block_id().str(), db2.head_block_id().str());
} FC_LOG_AND_RETHROW() }

// Test that two databases joining and leaving a network sync correctly after a fork
BOOST_FIXTURE_TEST_CASE(forked_network, testing_fixture)
{ try {
      MKDBS((db1)(db2))
      MKNET(net)

      BOOST_CHECK_EQUAL(db1.head_block_num(), 0);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 0);
      db1.produce_blocks();
      BOOST_CHECK_EQUAL(db1.head_block_num(), 1);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 0);
      BOOST_CHECK_NE(db1.head_block_id().str(), db2.head_block_id().str());

      net.connect_database(db1);
      net.connect_database(db2);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 1);
      BOOST_CHECK_EQUAL(db1.head_block_id().str(), db2.head_block_id().str());

      db2.produce_blocks(5);
      BOOST_CHECK_EQUAL(db1.head_block_num(), 6);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 6);
      BOOST_CHECK_EQUAL(db1.head_block_id().str(), db2.head_block_id().str());

      net.disconnect_database(db1);
      db1.produce_blocks(1, 1);
      db2.produce_blocks();
      BOOST_CHECK_EQUAL(db1.head_block_num(), 7);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 7);
      BOOST_CHECK_NE(db1.head_block_id().str(), db2.head_block_id().str());

      db2.produce_blocks(1, 1);
      net.connect_database(db1);
      BOOST_CHECK_EQUAL(db1.head_block_num(), 8);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 8);
      BOOST_CHECK_EQUAL(db1.head_block_id().str(), db2.head_block_id().str());
} FC_LOG_AND_RETHROW() }

// Check that the recent_slots_filled bitmap is being updated correctly
BOOST_FIXTURE_TEST_CASE( rsf_missed_blocks, testing_fixture )
{ try {
      MKDB(db)
      db.produce_blocks();

      auto rsf = [&]() -> string
      {
         auto rsf = db.get_dynamic_global_properties().recent_slots_filled;
         string result = "";
         result.reserve(64);
         for( int i=0; i<64; i++ )
         {
            result += ((rsf & 1) == 0) ? '0' : '1';
            rsf >>= 1;
         }
         return result;
      };

      auto pct = []( uint32_t x ) -> uint32_t
      {
         return uint64_t( config::Percent100 ) * x / 64;
      };

      BOOST_CHECK_EQUAL( rsf(),
         "1111111111111111111111111111111111111111111111111111111111111111"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), config::Percent100 );

      db.produce_blocks(1, 1);
      BOOST_CHECK_EQUAL( rsf(),
         "0111111111111111111111111111111111111111111111111111111111111111"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(127-64) );

      db.produce_blocks(1, 1);
      BOOST_CHECK_EQUAL( rsf(),
         "0101111111111111111111111111111111111111111111111111111111111111"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(126-64) );

      db.produce_blocks(1, 2);
      BOOST_CHECK_EQUAL( rsf(),
         "0010101111111111111111111111111111111111111111111111111111111111"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(124-64) );

      db.produce_blocks(1, 3);
      BOOST_CHECK_EQUAL( rsf(),
         "0001001010111111111111111111111111111111111111111111111111111111"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(121-64) );

      db.produce_blocks(1, 5);
      BOOST_CHECK_EQUAL( rsf(),
         "0000010001001010111111111111111111111111111111111111111111111111"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(116-64) );

      db.produce_blocks(1, 8);
      BOOST_CHECK_EQUAL( rsf(),
         "0000000010000010001001010111111111111111111111111111111111111111"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(108-64) );

      db.produce_blocks(1, 13);
      BOOST_CHECK_EQUAL( rsf(),
         "0000000000000100000000100000100010010101111111111111111111111111"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(95-64) );

      db.produce_blocks();
      BOOST_CHECK_EQUAL( rsf(),
         "1000000000000010000000010000010001001010111111111111111111111111"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(95-64) );

      db.produce_blocks();
      BOOST_CHECK_EQUAL( rsf(),
         "1100000000000001000000001000001000100101011111111111111111111111"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(95-64) );

      db.produce_blocks();
      BOOST_CHECK_EQUAL( rsf(),
         "1110000000000000100000000100000100010010101111111111111111111111"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(95-64) );

      db.produce_blocks();
      BOOST_CHECK_EQUAL( rsf(),
         "1111000000000000010000000010000010001001010111111111111111111111"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(95-64) );

      db.produce_blocks(1, 64);
      BOOST_CHECK_EQUAL( rsf(),
         "0000000000000000000000000000000000000000000000000000000000000000"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(0) );

      db.produce_blocks(1, 63);
      BOOST_CHECK_EQUAL( rsf(),
         "0000000000000000000000000000000000000000000000000000000000000001"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(1) );

      db.produce_blocks(1, 32);
      BOOST_CHECK_EQUAL( rsf(),
         "0000000000000000000000000000000010000000000000000000000000000000"
      );
      BOOST_CHECK_EQUAL( db.producer_participation_rate(), pct(1) );
} FC_LOG_AND_RETHROW() }

// Check that a db rewinds to the LIB after being closed and reopened
BOOST_FIXTURE_TEST_CASE(restart_db, testing_fixture)
{ try {
      MKDB(db)

      auto lag = HOTC_PERCENT(config::ProducerCount, config::IrreversibleThresholdPercent);
      db.produce_blocks(20);

      BOOST_CHECK_EQUAL(db.head_block_num(), 20);
      BOOST_CHECK_EQUAL(db.last_irreversible_block_num(), 20 - lag);

      db.close();
      db.open();

      // After restarting, we should have rewound to the last irreversible block.
      BOOST_CHECK_EQUAL(db.head_block_num(), 20 - lag);
      db.produce_blocks(5);
      BOOST_CHECK_EQUAL(db.head_block_num(), 25 - lag);
} FC_LOG_AND_RETHROW() }

// Check that a db which is closed and reopened successfully syncs back with the network, including retrieving blocks
// that it missed while it was down
BOOST_FIXTURE_TEST_CASE(sleepy_db, testing_fixture)
{ try {
      MKDB(producer)
      MKNET(net, (producer))

      auto lag = HOTC_PERCENT(config::ProducerCount, config::IrreversibleThresholdPercent);
      producer.produce_blocks(20);

      {
         // The new node, sleepy, joins, syncs, disconnects
         MKDB(sleepy, sleepy)
         net.connect_database(sleepy);
         BOOST_CHECK_EQUAL(producer.head_block_num(), 20);
         BOOST_CHECK_EQUAL(sleepy.head_block_num(), 20);

         net.disconnect_database(sleepy);
         sleepy.close();
      }

      // 5 new blocks are produced
      producer.produce_blocks(5);
      BOOST_CHECK_EQUAL(producer.head_block_num(), 25);

      // Sleepy is reborn! Check that it is now rewound to the LIB...
      MKDB(sleepy, sleepy)
      BOOST_CHECK_EQUAL(sleepy.head_block_num(), 20 - lag);

      // Reconnect sleepy to the network and check that it syncs up to the present
      net.connect_database(sleepy);
      BOOST_CHECK_EQUAL(sleepy.head_block_num(), 25);
      BOOST_CHECK_EQUAL(sleepy.head_block_id().str(), producer.head_block_id().str());
} FC_LOG_AND_RETHROW() }

// Test reindexing the blockchain
BOOST_FIXTURE_TEST_CASE(reindex, testing_fixture)
{ try {
      MKDB(db)

      auto lag = HOTC_PERCENT(config::ProducerCount, config::IrreversibleThresholdPercent);
      db.produce_blocks(100);

      BOOST_CHECK_EQUAL(db.last_irreversible_block_num(), 100 - lag);
      db.close();
      db.replay();
      BOOST_CHECK_EQUAL(db.head_block_num(), 100 - lag);
      db.produce_blocks(20);
      BOOST_CHECK_EQUAL(db.head_block_num(), 120 - lag);
} FC_LOG_AND_RETHROW() }

// Test wiping a database and resyncing with an ongoing network
BOOST_FIXTURE_TEST_CASE(wipe, testing_fixture)
{ try {
      MKDBS((db1)(db2)(db3))
      MKNET(net, (db1)(db2)(db3))

      db1.produce_blocks(3);
      db2.produce_blocks(3);
      BOOST_CHECK_EQUAL(db1.head_block_num(), 6);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 6);
      BOOST_CHECK_EQUAL(db3.head_block_num(), 6);
      BOOST_CHECK_EQUAL(db1.head_block_id().str(), db2.head_block_id().str());
      BOOST_CHECK_EQUAL(db1.head_block_id().str(), db3.head_block_id().str());

      net.disconnect_database(db3);
      db3.close();
      db3.wipe();
      db3.open();
      BOOST_CHECK_EQUAL(db3.head_block_num(), 0);

      net.connect_database(db3);
      BOOST_CHECK_EQUAL(db3.head_block_num(), 6);

      db1.produce_blocks(3);
      db2.produce_blocks(3);
      BOOST_CHECK_EQUAL(db1.head_block_num(), 12);
      BOOST_CHECK_EQUAL(db2.head_block_num(), 12);
      BOOST_CHECK_EQUAL(db3.head_block_num(), 12);
      BOOST_CHECK_EQUAL(db1.head_block_id().str(), db2.head_block_id().str());
      BOOST_CHECK_EQUAL(db1.head_block_id().str(), db3.head_block_id().str());
} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_SUITE_END()
