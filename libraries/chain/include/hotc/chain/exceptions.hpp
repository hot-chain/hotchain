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

#include <fc/exception/exception.hpp>
#include <hotc/chain/protocol.hpp>
#include <hotc/utilities/exception_macros.hpp>

namespace hotc { namespace chain {

   FC_DECLARE_EXCEPTION( chain_exception, 3000000, "blockchain exception" )
   FC_DECLARE_DERIVED_EXCEPTION( database_query_exception,          hotc::chain::chain_exception, 3010000, "database query exception" )
   FC_DECLARE_DERIVED_EXCEPTION( block_validate_exception,          hotc::chain::chain_exception, 3020000, "block validation exception" )
   FC_DECLARE_DERIVED_EXCEPTION( transaction_exception,             hotc::chain::chain_exception, 3030000, "transaction validation exception" )
   FC_DECLARE_DERIVED_EXCEPTION( message_validate_exception,        hotc::chain::chain_exception, 3040000, "message validation exception" )
   FC_DECLARE_DERIVED_EXCEPTION( message_precondition_exception,    hotc::chain::chain_exception, 3050000, "message precondition exception" )
   FC_DECLARE_DERIVED_EXCEPTION( message_evaluate_exception,        hotc::chain::chain_exception, 3060000, "message evaluation exception" )
   FC_DECLARE_DERIVED_EXCEPTION( utility_exception,                 hotc::chain::chain_exception, 3070000, "utility method exception" )
   FC_DECLARE_DERIVED_EXCEPTION( undo_database_exception,           hotc::chain::chain_exception, 3080000, "undo database exception" )
   FC_DECLARE_DERIVED_EXCEPTION( unlinkable_block_exception,        hotc::chain::chain_exception, 3090000, "unlinkable block" )
   FC_DECLARE_DERIVED_EXCEPTION( black_swan_exception,              hotc::chain::chain_exception, 3100000, "black swan" )
   FC_DECLARE_DERIVED_EXCEPTION( unknown_block_exception,           hotc::chain::chain_exception, 3110000, "unknown block" )

   FC_DECLARE_DERIVED_EXCEPTION( tx_missing_auth,                   hotc::chain::transaction_exception, 3030001, "missing required authority" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_missing_sigs,                   hotc::chain::transaction_exception, 3030002, "signatures do not satisfy declared authorizations" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_irrelevant_auth,                hotc::chain::transaction_exception, 3030003, "irrelevant authority included" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_irrelevant_sig,                 hotc::chain::transaction_exception, 3030004, "irrelevant signature included" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_duplicate_sig,                  hotc::chain::transaction_exception, 3030005, "duplicate signature included" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_committee_approval,        hotc::chain::transaction_exception, 3030006, "committee account cannot directly approve transaction" )
   FC_DECLARE_DERIVED_EXCEPTION( insufficient_fee,                  hotc::chain::transaction_exception, 3030007, "insufficient fee" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_missing_scope,                  hotc::chain::transaction_exception, 3030008, "missing required scope" )
   FC_DECLARE_DERIVED_EXCEPTION( tx_missing_recipient,              hotc::chain::transaction_exception, 3030009, "missing required recipient" )
   FC_DECLARE_DERIVED_EXCEPTION( checktime_exceeded,                hotc::chain::transaction_exception, 3030010, "allotted processing time was exceeded" )
   FC_DECLARE_DERIVED_EXCEPTION( unknown_transaction_exception,     hotc::chain::transaction_exception, 3030011, "unknown transaction" )

   FC_DECLARE_DERIVED_EXCEPTION( invalid_pts_address,               hotc::chain::utility_exception, 3060001, "invalid pts address" )
   FC_DECLARE_DERIVED_EXCEPTION( insufficient_feeds,                hotc::chain::chain_exception, 37006, "insufficient feeds" )

   FC_DECLARE_DERIVED_EXCEPTION( pop_empty_chain,                   hotc::chain::undo_database_exception, 3070001, "there are no blocks to pop" )

//   HOTC_DECLARE_OP_EVALUATE_EXCEPTION( from_account_not_whitelisted, transfer, 1, "owner mismatch" )
//   HOTC_DECLARE_OP_EVALUATE_EXCEPTION( to_account_not_whitelisted, transfer, 2, "owner mismatch" )
//   HOTC_DECLARE_OP_EVALUATE_EXCEPTION( restricted_transfer_asset, transfer, 3, "restricted transfer asset" )

   /*
   FC_DECLARE_DERIVED_EXCEPTION( addition_overflow,                 hotc::chain::chain_exception, 30002, "addition overflow" )
   FC_DECLARE_DERIVED_EXCEPTION( subtraction_overflow,              hotc::chain::chain_exception, 30003, "subtraction overflow" )
   FC_DECLARE_DERIVED_EXCEPTION( asset_type_mismatch,               hotc::chain::chain_exception, 30004, "asset/price mismatch" )
   FC_DECLARE_DERIVED_EXCEPTION( unsupported_chain_operation,       hotc::chain::chain_exception, 30005, "unsupported chain operation" )
   FC_DECLARE_DERIVED_EXCEPTION( unknown_transaction,               hotc::chain::chain_exception, 30006, "unknown transaction" )
   FC_DECLARE_DERIVED_EXCEPTION( duplicate_transaction,             hotc::chain::chain_exception, 30007, "duplicate transaction" )
   FC_DECLARE_DERIVED_EXCEPTION( zero_amount,                       hotc::chain::chain_exception, 30008, "zero amount" )
   FC_DECLARE_DERIVED_EXCEPTION( zero_price,                        hotc::chain::chain_exception, 30009, "zero price" )
   FC_DECLARE_DERIVED_EXCEPTION( asset_divide_by_self,              hotc::chain::chain_exception, 30010, "asset divide by self" )
   FC_DECLARE_DERIVED_EXCEPTION( asset_divide_by_zero,              hotc::chain::chain_exception, 30011, "asset divide by zero" )
   FC_DECLARE_DERIVED_EXCEPTION( new_database_version,              hotc::chain::chain_exception, 30012, "new database version" )
   FC_DECLARE_DERIVED_EXCEPTION( unlinkable_block,                  hotc::chain::chain_exception, 30013, "unlinkable block" )
   FC_DECLARE_DERIVED_EXCEPTION( price_out_of_range,                hotc::chain::chain_exception, 30014, "price out of range" )

   FC_DECLARE_DERIVED_EXCEPTION( block_numbers_not_sequential,      hotc::chain::chain_exception, 30015, "block numbers not sequential" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_previous_block_id,         hotc::chain::chain_exception, 30016, "invalid previous block" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_block_time,                hotc::chain::chain_exception, 30017, "invalid block time" )
   FC_DECLARE_DERIVED_EXCEPTION( time_in_past,                      hotc::chain::chain_exception, 30018, "time is in the past" )
   FC_DECLARE_DERIVED_EXCEPTION( time_in_future,                    hotc::chain::chain_exception, 30019, "time is in the future" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_block_digest,              hotc::chain::chain_exception, 30020, "invalid block digest" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_committee_member_signee,           hotc::chain::chain_exception, 30021, "invalid committee_member signee" )
   FC_DECLARE_DERIVED_EXCEPTION( failed_checkpoint_verification,    hotc::chain::chain_exception, 30022, "failed checkpoint verification" )
   FC_DECLARE_DERIVED_EXCEPTION( wrong_chain_id,                    hotc::chain::chain_exception, 30023, "wrong chain id" )
   FC_DECLARE_DERIVED_EXCEPTION( block_older_than_undo_history,     hotc::chain::chain_exception, 30025, "block is older than our undo history allows us to process" )

   FC_DECLARE_EXCEPTION( evaluation_error, 31000, "Evaluation Error" )
   FC_DECLARE_DERIVED_EXCEPTION( negative_deposit,                  hotc::chain::evaluation_error, 31001, "negative deposit" )
   FC_DECLARE_DERIVED_EXCEPTION( not_a_committee_member,                    hotc::chain::evaluation_error, 31002, "not a committee_member" )
   FC_DECLARE_DERIVED_EXCEPTION( unknown_balance_record,            hotc::chain::evaluation_error, 31003, "unknown balance record" )
   FC_DECLARE_DERIVED_EXCEPTION( insufficient_funds,                hotc::chain::evaluation_error, 31004, "insufficient funds" )
   FC_DECLARE_DERIVED_EXCEPTION( missing_signature,                 hotc::chain::evaluation_error, 31005, "missing signature" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_claim_password,            hotc::chain::evaluation_error, 31006, "invalid claim password" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_withdraw_condition,        hotc::chain::evaluation_error, 31007, "invalid withdraw condition" )
   FC_DECLARE_DERIVED_EXCEPTION( negative_withdraw,                 hotc::chain::evaluation_error, 31008, "negative withdraw" )
   FC_DECLARE_DERIVED_EXCEPTION( not_an_active_committee_member,            hotc::chain::evaluation_error, 31009, "not an active committee_member" )
   FC_DECLARE_DERIVED_EXCEPTION( expired_transaction,               hotc::chain::evaluation_error, 31010, "expired transaction" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_transaction_expiration,    hotc::chain::evaluation_error, 31011, "invalid transaction expiration" )
   FC_DECLARE_DERIVED_EXCEPTION( oversized_transaction,             hotc::chain::evaluation_error, 31012, "transaction exceeded the maximum transaction size" )

   FC_DECLARE_DERIVED_EXCEPTION( invalid_account_name,              hotc::chain::evaluation_error, 32001, "invalid account name" )
   FC_DECLARE_DERIVED_EXCEPTION( unknown_account_id,                hotc::chain::evaluation_error, 32002, "unknown account id" )
   FC_DECLARE_DERIVED_EXCEPTION( unknown_account_name,              hotc::chain::evaluation_error, 32003, "unknown account name" )
   FC_DECLARE_DERIVED_EXCEPTION( missing_parent_account_signature,  hotc::chain::evaluation_error, 32004, "missing parent account signature" )
   FC_DECLARE_DERIVED_EXCEPTION( parent_account_retracted,          hotc::chain::evaluation_error, 32005, "parent account retracted" )
   FC_DECLARE_DERIVED_EXCEPTION( account_expired,                   hotc::chain::evaluation_error, 32006, "account expired" )
   FC_DECLARE_DERIVED_EXCEPTION( account_already_registered,        hotc::chain::evaluation_error, 32007, "account already registered" )
   FC_DECLARE_DERIVED_EXCEPTION( account_key_in_use,                hotc::chain::evaluation_error, 32008, "account key already in use" )
   FC_DECLARE_DERIVED_EXCEPTION( account_retracted,                 hotc::chain::evaluation_error, 32009, "account retracted" )
   FC_DECLARE_DERIVED_EXCEPTION( unknown_parent_account_name,       hotc::chain::evaluation_error, 32010, "unknown parent account name" )
   FC_DECLARE_DERIVED_EXCEPTION( unknown_committee_member_slate,            hotc::chain::evaluation_error, 32011, "unknown committee_member slate" )
   FC_DECLARE_DERIVED_EXCEPTION( too_may_committee_members_in_slate,        hotc::chain::evaluation_error, 32012, "too many committee_members in slate" )
   FC_DECLARE_DERIVED_EXCEPTION( pay_balance_remaining,             hotc::chain::evaluation_error, 32013, "pay balance remaining" )

   FC_DECLARE_DERIVED_EXCEPTION( not_a_committee_member_signature,          hotc::chain::evaluation_error, 33002, "not committee_members signature" )

   FC_DECLARE_DERIVED_EXCEPTION( invalid_precision,                 hotc::chain::evaluation_error, 35001, "invalid precision" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_asset_symbol,              hotc::chain::evaluation_error, 35002, "invalid asset symbol" )
   FC_DECLARE_DERIVED_EXCEPTION( unknown_asset_id,                  hotc::chain::evaluation_error, 35003, "unknown asset id" )
   FC_DECLARE_DERIVED_EXCEPTION( asset_symbol_in_use,               hotc::chain::evaluation_error, 35004, "asset symbol in use" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_asset_amount,              hotc::chain::evaluation_error, 35005, "invalid asset amount" )
   FC_DECLARE_DERIVED_EXCEPTION( negative_issue,                    hotc::chain::evaluation_error, 35006, "negative issue" )
   FC_DECLARE_DERIVED_EXCEPTION( over_issue,                        hotc::chain::evaluation_error, 35007, "over issue" )
   FC_DECLARE_DERIVED_EXCEPTION( unknown_asset_symbol,              hotc::chain::evaluation_error, 35008, "unknown asset symbol" )
   FC_DECLARE_DERIVED_EXCEPTION( asset_id_in_use,                   hotc::chain::evaluation_error, 35009, "asset id in use" )
   FC_DECLARE_DERIVED_EXCEPTION( not_user_issued,                   hotc::chain::evaluation_error, 35010, "not user issued" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_asset_name,                hotc::chain::evaluation_error, 35011, "invalid asset name" )

   FC_DECLARE_DERIVED_EXCEPTION( committee_member_vote_limit,               hotc::chain::evaluation_error, 36001, "committee_member_vote_limit" )
   FC_DECLARE_DERIVED_EXCEPTION( insufficient_fee,                  hotc::chain::evaluation_error, 36002, "insufficient fee" )
   FC_DECLARE_DERIVED_EXCEPTION( negative_fee,                      hotc::chain::evaluation_error, 36003, "negative fee" )
   FC_DECLARE_DERIVED_EXCEPTION( missing_deposit,                   hotc::chain::evaluation_error, 36004, "missing deposit" )
   FC_DECLARE_DERIVED_EXCEPTION( insufficient_relay_fee,            hotc::chain::evaluation_error, 36005, "insufficient relay fee" )

   FC_DECLARE_DERIVED_EXCEPTION( invalid_market,                    hotc::chain::evaluation_error, 37001, "invalid market" )
   FC_DECLARE_DERIVED_EXCEPTION( unknown_market_order,              hotc::chain::evaluation_error, 37002, "unknown market order" )
   FC_DECLARE_DERIVED_EXCEPTION( shorting_base_shares,              hotc::chain::evaluation_error, 37003, "shorting base shares" )
   FC_DECLARE_DERIVED_EXCEPTION( insufficient_collateral,           hotc::chain::evaluation_error, 37004, "insufficient collateral" )
   FC_DECLARE_DERIVED_EXCEPTION( insufficient_depth,                hotc::chain::evaluation_error, 37005, "insufficient depth" )
   FC_DECLARE_DERIVED_EXCEPTION( insufficient_feeds,                hotc::chain::evaluation_error, 37006, "insufficient feeds" )
   FC_DECLARE_DERIVED_EXCEPTION( invalid_feed_price,                hotc::chain::evaluation_error, 37007, "invalid feed price" )

   FC_DECLARE_DERIVED_EXCEPTION( price_multiplication_overflow,     hotc::chain::evaluation_error, 38001, "price multiplication overflow" )
   FC_DECLARE_DERIVED_EXCEPTION( price_multiplication_underflow,    hotc::chain::evaluation_error, 38002, "price multiplication underflow" )
   FC_DECLARE_DERIVED_EXCEPTION( price_multiplication_undefined,    hotc::chain::evaluation_error, 38003, "price multiplication undefined product 0*inf" )
   */

   #define HOTC_RECODE_EXC( cause_type, effect_type ) \
      catch( const cause_type& e ) \
      { throw( effect_type( e.what(), e.get_log() ) ); }

} } // hotc::chain
