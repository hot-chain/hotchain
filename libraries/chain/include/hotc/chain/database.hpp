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
#pragma once
#include <hotc/chain/global_property_object.hpp>
#include <hotc/chain/node_property_object.hpp>
#include <hotc/chain/fork_database.hpp>
#include <hotc/chain/genesis_state.hpp>
#include <hotc/chain/block_log.hpp>

#include <chainbase/chainbase.hpp>
#include <fc/scoped_exit.hpp>
#include <fc/signals.hpp>

#include <hotc/chain/protocol/protocol.hpp>

#include <fc/log/logger.hpp>

#include <map>

namespace hotc { namespace chain {

   class database;

   class message_validate_context {
      public:
         message_validate_context( const transaction& t, const message& m )
         :trx(t),msg(m){}

         const transaction& trx;
         const message&     msg;
   };


   class precondition_validate_context : public message_validate_context {
      public:
         precondition_validate_context( const database& d, const transaction& t, const message& m, const account_name& r )
         :message_validate_context(t,m),receiver(r),db(d){}

         const account_name& receiver;
         const database&    db;
   };

   class apply_context : public precondition_validate_context {
      public:
         apply_context( database& d, const transaction& t, const message& m, const account_name& receiver )
         :precondition_validate_context(d,t,m,receiver),mutable_db(d){}

         database&    mutable_db;
   };

   typedef std::function<void( message_validate_context& )> message_validate_handler;
   typedef std::function<void( precondition_validate_context& )>   precondition_validate_handler;
   typedef std::function<void( apply_context& )>            apply_handler;

   /**
    *   @class database
    *   @brief tracks the blockchain state in an extensible manner
    */
   class database : public chainbase::database
   {

      public:
         database();
         ~database();

         /**
          *  This signal is emitted after all operations and virtual operation for a
          *  block have been applied but before the get_applied_operations() are cleared.
          *
          *  You may not yield from this callback because the blockchain is holding
          *  the write lock and may be in an "inconstant state" until after it is
          *  released.
          */
         fc::signal<void(const signed_block&)> applied_block;

         /**
          * This signal is emitted any time a new transaction is added to the pending
          * block state.
          */
         fc::signal<void(const signed_transaction&)> on_pending_transaction;



         /**
          *  The database can override any script handler with native code.
          */
         ///@{
         void set_validate_handler( const account_name& contract, const message_type& action, message_validate_handler v );
         void set_precondition_validate_handler(  const account_name& contract, const message_type& action, precondition_validate_handler v );
         void set_apply_handler( const account_name& contract, const message_type& action, apply_handler v );
         //@}

         enum validation_steps
         {
            skip_nothing                = 0,
            skip_producer_signature     = 1 << 0,  ///< used while reindexing
            skip_transaction_signatures = 1 << 1,  ///< used by non-producer nodes
            skip_transaction_dupe_check = 1 << 2,  ///< used while reindexing
            skip_fork_db                = 1 << 3,  ///< used while reindexing
            skip_block_size_check       = 1 << 4,  ///< used when applying locally generated transactions
            skip_tapos_check            = 1 << 5,  ///< used while reindexing -- note this skips expiration check as well
            skip_authority_check        = 1 << 6,  ///< used while reindexing -- disables any checking of authority on transactions
            skip_merkle_check           = 1 << 7,  ///< used while reindexing
            skip_assert_evaluation      = 1 << 8,  ///< used while reindexing
            skip_undo_history_check     = 1 << 9,  ///< used while reindexing
            skip_producer_schedule_check= 1 << 10,  ///< used while reindexing
            skip_validate               = 1 << 11 ///< used prior to checkpoint, skips validate() call on transaction
         };

         /**
          * @brief Open a database, creating a new one if necessary
          *
          * Opens a database in the specified directory. If no initialized database is found, genesis_loader is called
          * and its return value is used as the genesis state when initializing the new database
          *
          * genesis_loader will not be called if an existing database is found.
          *
          * @param data_dir Path to open or create database in
          * @param shared_file_size Size of the database memory mapped file
          * @param genesis_loader A callable object which returns the genesis state to initialize new databases on
          */
          void open(const fc::path& data_dir,
                    uint64_t shared_file_size,
                    std::function<genesis_state_type()> genesis_loader );

         /**
          * @brief Rebuild object graph from block history and open detabase
          *
          * This method may be called after or instead of @ref database::open, and will rebuild the object graph by
          * replaying blockchain history. When this method exits successfully, the database will be open.
          */
         void replay(fc::path data_dir, uint64_t shared_file_size, const genesis_state_type& initial_allocation = genesis_state_type());

         /**
          * @brief wipe Delete database from disk, and potentially the raw chain as well.
          * @param include_blocks If true, delete the raw chain as well as the database.
          *
          * Will close the database before wiping. Database will be closed when this function returns.
          */
         void wipe(const fc::path& data_dir, bool include_blocks);
         void close();

         /**
          *  @return true if the block is in our fork DB or saved to disk as
          *  part of the official chain, otherwise return false
          */
         bool                       is_known_block( const block_id_type& id )const;
         bool                       is_known_transaction( const transaction_id_type& id )const;
         block_id_type              get_block_id_for_num( uint32_t block_num )const;
         optional<signed_block>     fetch_block_by_id( const block_id_type& id )const;
         optional<signed_block>     fetch_block_by_number( uint32_t num )const;
         const signed_transaction&  get_recent_transaction( const transaction_id_type& trx_id )const;
         std::vector<block_id_type> get_block_ids_on_fork(block_id_type head_of_fork) const;

         /**
          *  Calculate the percent of block production slots that were missed in the
          *  past 128 blocks, not including the current block.
          */
         uint32_t producer_participation_rate()const;

         void                                   add_checkpoints(const flat_map<uint32_t,block_id_type>& checkpts);
         const flat_map<uint32_t,block_id_type> get_checkpoints()const { return _checkpoints; }
         bool before_last_checkpoint()const;

         bool push_block( const signed_block& b, uint32_t skip = skip_nothing );
         void push_transaction( const signed_transaction& trx, uint32_t skip = skip_nothing );
         bool _push_block( const signed_block& b );
         void _push_transaction( const signed_transaction& trx );

         signed_block generate_block(
            const fc::time_point_sec when,
            producer_id_type producer,
            const fc::ecc::private_key& block_signing_private_key,
            uint32_t skip
            );
         signed_block _generate_block(
            const fc::time_point_sec when,
            producer_id_type producer,
            const fc::ecc::private_key& block_signing_private_key
            );


         template<typename Function>
         auto with_skip_flags( uint64_t flags, Function&& f ) -> decltype((*((Function*)nullptr))()) 
         {
            auto old_flags = _skip_flags;
            auto on_exit   = fc::make_scoped_exit( [&](){ _skip_flags = old_flags; } );
            _skip_flags = flags;
            return f();
         }

         template<typename Function>
         auto with_producing( Function&& f ) -> decltype((*((Function*)nullptr))()) 
         {
            auto old_producing = _producing;
            auto on_exit   = fc::make_scoped_exit( [&](){ _producing = old_producing; } );
            _producing = true;
            return f();
         }


         template<typename Function>
         auto without_pending_transactions( Function&& f ) -> decltype((*((Function*)nullptr))()) 
         {
            auto old_pending = std::move( _pending_transactions );
            _pending_tx_session.reset();
            auto on_exit = fc::make_scoped_exit( [&](){ 
               for( const auto& t : old_pending ) {
                  try {
                     push_transaction( t );
                  } catch ( ... ){}
               }
            });
            return f();
         }


         void pop_block();
         void clear_pending();




         /**
          * @brief Get the producer scheduled for block production in a slot.
          *
          * slot_num always corresponds to a time in the future.
          *
          * If slot_num == 1, returns the next scheduled producer.
          * If slot_num == 2, returns the next scheduled producer after
          * 1 block gap.
          *
          * Use the get_slot_time() and get_slot_at_time() functions
          * to convert between slot_num and timestamp.
          *
          * Passing slot_num == 0 returns HOTC_NULL_PRODUCER
          */
         producer_id_type get_scheduled_producer(uint32_t slot_num)const;

         /**
          * Get the time at which the given slot occurs.
          *
          * If slot_num == 0, return time_point_sec().
          *
          * If slot_num == N for N > 0, return the Nth next
          * block-interval-aligned time greater than head_block_time().
          */
         fc::time_point_sec get_slot_time(uint32_t slot_num)const;

         /**
          * Get the last slot which occurs AT or BEFORE the given time.
          *
          * The return value is the greatest value N such that
          * get_slot_time( N ) <= when.
          *
          * If no such N exists, return 0.
          */
         uint32_t get_slot_at_time(fc::time_point_sec when)const;

         void update_producer_schedule();

         const chain_id_type&                   get_chain_id()const;
         const global_property_object&          get_global_properties()const;
         const dynamic_global_property_object&  get_dynamic_global_properties()const;
         const node_property_object&            get_node_properties()const;

         time_point_sec   head_block_time()const;
         uint32_t         head_block_num()const;
         block_id_type    head_block_id()const;
         producer_id_type head_block_producer()const;

         uint32_t  block_interval( )const { return config::BlockIntervalSeconds; }

         node_property_object& node_properties();


         uint32_t last_irreversible_block_num() const;

         /// Reset the object graph in-memory
         void initialize_indexes();
         void init_genesis(const genesis_state_type& genesis_state = genesis_state_type());

         void debug_dump();
         void apply_debug_updates();
         void debug_update(const fc::variant_object& update);

         /**
           *  This method validates transactions without adding it to the pending state.
           *  @return true if the transaction would validate
           */
          void validate_transaction(const signed_transaction& trx)const;
          void validate_tapos( const signed_transaction& trx )const;

       private:
          optional<session> _pending_tx_session;

       public:
         // these were formerly private, but they have a fairly well-defined API, so let's make them public
         void apply_block(const signed_block& next_block, uint32_t skip = skip_nothing);
         void apply_transaction(const signed_transaction& trx, uint32_t skip = skip_nothing);
      private:
         void _apply_block(const signed_block& next_block);
         void _apply_transaction(const signed_transaction& trx);

         void validate_uniqueness( const signed_transaction& trx )const;
         void validate_message_precondition( precondition_validate_context& c )const;
         void apply_message( apply_context& c );



         bool check_for_duplicate_transactions()const { return !(_skip_flags&skip_transaction_dupe_check); }
         bool check_tapos()const                      { return !(_skip_flags&skip_tapos_check);            }

         ///Steps involved in applying a new block
         ///@{

         const producer_object& validate_block_header(uint32_t skip, const signed_block& next_block)const;
         const producer_object& _validate_block_header(const signed_block& next_block)const;
         void create_block_summary(const signed_block& next_block);

         void update_global_dynamic_data(const signed_block& b);
         void update_signing_producer(const producer_object& signing_producer, const signed_block& new_block);
         void update_last_irreversible_block();
         void clear_expired_transactions();

         deque< signed_transaction >       _pending_transactions;
         fork_database                     _fork_db;

         block_log                         _block_log;

         bool                              _producing = false;
         bool                              _pushing  = false;
         uint64_t                          _skip_flags = 0;

         flat_map<uint32_t,block_id_type>  _checkpoints;

         node_property_object              _node_property_object;

         map< account_name, map<message_type, message_validate_handler> > message_validate_handlers;
         map< account_name, map<message_type, precondition_validate_handler> >   precondition_validate_handlers;
         map< account_name, map<message_type, apply_handler> >            apply_handlers;
   };

} }
