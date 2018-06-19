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
#include <hotc/chain/protocol/types.hpp>

#include "multi_index_includes.hpp"

namespace hotc { namespace chain {
   class producer_object : public chainbase::object<producer_object_type, producer_object>
   {
         producer_object() = delete;
   public:
         template<typename Constructor, typename Allocator>
         producer_object(Constructor&& c, chainbase::allocator<Allocator> a)
            : owner_name(a), url(a) {
            c(*this);
         }

         id_type          id;
         shared_string    owner_name;
         uint64_t         last_aslot = 0;
         public_key_type  signing_key;
         shared_string    url;
         int64_t          total_missed = 0;
         uint32_t         last_confirmed_block_num = 0;
   };

   struct by_key;
   using producer_multi_index = chainbase::shared_multi_index_container<
      producer_object,
      indexed_by<
         ordered_unique<tag<by_id>, member<producer_object, producer_object::id_type, &producer_object::id>>,
         ordered_non_unique<tag<by_key>, member<producer_object, public_key_type, &producer_object::signing_key>>
      >
   >;
} } // hotc::chain

CHAINBASE_SET_INDEX_TYPE(hotc::chain::producer_object, hotc::chain::producer_multi_index)

FC_REFLECT(hotc::chain::producer_object,
           (id)
           (owner_name)
           (last_aslot)
           (signing_key)
           (url)
           (total_missed)
           (last_confirmed_block_num)
          )
