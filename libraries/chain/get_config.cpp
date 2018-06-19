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

#include <hotc/chain/get_config.hpp>
#include <hotc/chain/config.hpp>
#include <hotc/chain/protocol/types.hpp>

namespace hotc { namespace chain {

fc::variant_object get_config()
{
   fc::mutable_variant_object result;

   result[ "HOTC_KEY_PREFIX" ] = HOTC_KEY_PREFIX;
   result[ "HOTC_MIN_TRANSACTION_SIZE_LIMIT" ] = HOTC_MIN_TRANSACTION_SIZE_LIMIT;
   result[ "HOTC_MIN_BLOCK_INTERVAL" ] = HOTC_MIN_BLOCK_INTERVAL;
   result[ "HOTC_MAX_BLOCK_INTERVAL" ] = HOTC_MAX_BLOCK_INTERVAL;
   result[ "HOTC_DEFAULT_BLOCK_INTERVAL" ] = HOTC_DEFAULT_BLOCK_INTERVAL;
   result[ "HOTC_DEFAULT_MAX_TRANSACTION_SIZE" ] = HOTC_DEFAULT_MAX_TRANSACTION_SIZE;
   result[ "HOTC_DEFAULT_MAX_BLOCK_SIZE" ] = HOTC_DEFAULT_MAX_BLOCK_SIZE;
   result[ "HOTC_DEFAULT_MAX_TIME_UNTIL_EXPIRATION" ] = HOTC_DEFAULT_MAX_TIME_UNTIL_EXPIRATION;
   result[ "HOTC_DEFAULT_MAINTENANCE_INTERVAL" ] = HOTC_DEFAULT_MAINTENANCE_INTERVAL;
   result[ "HOTC_DEFAULT_MAINTENANCE_SKIP_SLOTS" ] = HOTC_DEFAULT_MAINTENANCE_SKIP_SLOTS;
   result[ "HOTC_MIN_UNDO_HISTORY" ] = HOTC_MIN_UNDO_HISTORY;
   result[ "HOTC_MAX_UNDO_HISTORY" ] = HOTC_MAX_UNDO_HISTORY;
   result[ "HOTC_MIN_BLOCK_SIZE_LIMIT" ] = HOTC_MIN_BLOCK_SIZE_LIMIT;
   result[ "HOTC_MIN_TRANSACTION_EXPIRATION_LIMIT" ] = HOTC_MIN_TRANSACTION_EXPIRATION_LIMIT;
   result[ "HOTC_100_PERCENT" ] = HOTC_100_PERCENT;
   result[ "HOTC_1_PERCENT" ] = HOTC_1_PERCENT;
   result[ "HOTC_DEFAULT_MAX_PRODUCERES" ] = HOTC_DEFAULT_MAX_PRODUCERES;
   result[ "HOTC_MAX_URL_LENGTH" ] = HOTC_MAX_URL_LENGTH;
   result[ "HOTC_NEAR_SCHEDULE_CTR_IV" ] = HOTC_NEAR_SCHEDULE_CTR_IV;
   result[ "HOTC_FAR_SCHEDULE_CTR_IV" ] = HOTC_FAR_SCHEDULE_CTR_IV;
   result[ "HOTC_CORE_ASSET_CYCLE_RATE" ] = HOTC_CORE_ASSET_CYCLE_RATE;
   result[ "HOTC_CORE_ASSET_CYCLE_RATE_BITS" ] = HOTC_CORE_ASSET_CYCLE_RATE_BITS;

   return result;
}

} } // hotc::chain
