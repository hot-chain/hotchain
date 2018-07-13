#pragma once
#include <HOTC/chain/protocol/types.hpp>


namespace HOTC { namespace chain {

   struct PermissionLevel {
      account_name      account;
      permission_name   level;
      uint16_t          weight;
   };

   struct KeyPermission {
      public_key_type key;
      uint16_t        weight;
   };

   struct Authority {
      uint32_t                threshold = 0;
      vector<PermissionLevel> accounts;
      vector<KeyPermission>   keys;
   };

} }  // HOTC::chain

FC_REFLECT( HOTC::chain::Authority, (threshold)(accounts)(keys) )
