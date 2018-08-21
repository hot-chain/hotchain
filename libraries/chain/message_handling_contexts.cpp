#include <hotc/chain/message_handling_contexts.hpp>
#include <hotc/chain/permission_object.hpp>
#include <hotc/chain/exceptions.hpp>
#include <hotc/chain/key_value_object.hpp>

#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/range/algorithm/find_if.hpp>

namespace hotc { namespace chain {

void message_validate_context::require_authorization(const types::AccountName& account) {

   auto itr = boost::find_if(msg.authorization, [&account](const types::AccountPermission& ap) {
      return ap.account == account;
   });

   HOTC_ASSERT(itr != msg.authorization.end(), tx_missing_auth,
              "Required authorization ${auth} not found", ("auth", account));

   used_authorizations[itr - msg.authorization.begin()] = true;
}

void message_validate_context::require_scope(const types::AccountName& account)const {
   auto itr = boost::find_if(trx.scope, [&account](const auto& scope) {
      return scope == account;
   });

   HOTC_ASSERT( itr != trx.scope.end(), tx_missing_scope,
              "Required scope ${scope} not declared by transaction", ("scope",account) );
}

void message_validate_context::require_recipient(const types::AccountName& account)const {
   auto itr = boost::find_if(msg.recipients, [&account](const auto& recipient) {
      return recipient == account;
   });

   HOTC_ASSERT( itr != msg.recipients.end(), tx_missing_recipient,
              "Required recipient ${recipient} not declared by message", ("recipient",account)("recipients",msg.recipients) );
}

bool message_validate_context::all_authorizations_used() const {
   return boost::algorithm::all_of_equal(used_authorizations, true);
}

int32_t message_validate_context::load_i64( Name scope, Name code, Name table, Name key, char* value, uint32_t valuelen ) {
   require_scope( scope );

   const auto* obj = db.find<key_value_object,by_scope_key>( boost::make_tuple(
                                                            AccountName(scope), 
                                                            AccountName(code), 
                                                            AccountName(table), 
                                                            AccountName(key) ) );
   if( obj == nullptr ) { return -1; }
   auto copylen =  std::min<size_t>(obj->value.size(),valuelen);
   if( copylen ) {
      obj->value.copy(value, copylen);
   }
   return copylen;
}

int32_t message_validate_context::back_primary_i128i128( Name scope, Name code, Name table, 
                                 uint128_t* primary, uint128_t* secondary, char* value, uint32_t valuelen ) {

   return -1;
   /*
    require_scope( scope );
    const auto& idx = db.get_index<key128x128_value_index,by_scope_primary>();
    auto itr = idx.lower_bound( boost::make_tuple( uint64_t(scope), 
                                                  uint64_t(code), 
                                                  table.value+1, 
                                                  *primary, uint128_t(0) ) );

    if( itr == idx.begin() && itr == idx.end() ) 
       return 0;

    --itr;

    if( itr->scope != scope ||
        itr->code != code ||
        itr->table != table ||
        itr->primary_key != *primary ) return -1;

    *secondary = itr->secondary_key;
    *primary = itr->primary_key;
    
    auto copylen =  std::min<size_t>(itr->value.size(),valuelen);
    if( copylen ) {
       itr->value.copy(value, copylen);
    }
    return copylen;
    */
}

int32_t message_validate_context::back_secondary_i128i128( Name scope, Name code, Name table, 
                                 uint128_t* primary, uint128_t* secondary, char* value, uint32_t valuelen ) {

   /*
    require_scope( scope );
    const auto& idx = db.get_index<key128x128_value_index,by_scope_secondary>();
    auto itr = idx.lower_bound( boost::make_tuple( AccountName(scope), 
                                                  AccountName(code), 
                                                  table.value+1, 
                                                  *secondary, uint128_t(0) ) );

    if( itr == idx.begin() && itr == idx.end() ) 
       return 0;

    --itr;

    if( itr->scope != scope ||
        itr->code != code ||
        itr->table != table ||
        itr->primary_key != *primary ) return -1;

    *secondary = itr->secondary_key;
    *primary = itr->primary_key;
    
    auto copylen =  std::min<size_t>(itr->value.size(),valuelen);
    if( copylen ) {
       itr->value.copy(value, copylen);
    }
    return copylen;
    */
   return -1;
}


int32_t message_validate_context::front_primary_i128i128( Name scope, Name code, Name table, 
                                 uint128_t* primary, uint128_t* secondary, char* value, uint32_t valuelen ) {

   /*
    require_scope( scope );
    const auto& idx = db.get_index<key128x128_value_index,by_scope_primary>();
    auto itr = idx.lower_bound( boost::make_tuple( uint64_t(scope), 
                                                  uint64_t(code), 
                                                  uint64_t(table), 
                                                  *primary, uint128_t(0) ) );

    if( itr == idx.end() ) 
       return -1;

    --itr;

    if( itr->scope != scope ||
        itr->code != code ||
        itr->table != table ||
        itr->primary_key != *primary ) return -1;

    *secondary = itr->secondary_key;
    *primary = itr->primary_key;
    
    auto copylen =  std::min<size_t>(itr->value.size(),valuelen);
    if( copylen ) {
       itr->value.copy(value, copylen);
    }
    return copylen;
    */
   return -1;
}
int32_t message_validate_context::front_secondary_i128i128( Name scope, Name code, Name table, 
                                 uint128_t* primary, uint128_t* secondary, char* value, uint32_t valuelen ) {

    require_scope( scope );
    const auto& idx = db.get_index<key128x128_value_index,by_scope_secondary>();
    /*
    auto itr = idx.lower_bound( boost::make_tuple( AccountName(scope), 
                                                  AccountName(code), 
                                                  AccountName(table), 
                                                  uint128_t(0), uint128_t(0) ) );
                                                  */
    auto itr = idx.lower_bound( boost::make_tuple( uint64_t(0ull), 
                                                  uint64_t(0ull), 
                                                  uint64_t(0ull),
                                                  uint128_t(0),
                                                  uint128_t(0)) );

    wdump((scope)(code)(table));
    idump((idx.size()));
    for( const auto& item : idx ) {
       auto tup = boost::make_tuple( item.scope, item.code, item.table, item.primary_key, item.secondary_key );
       auto in  = boost::make_tuple( AccountName(scope), AccountName(code), AccountName(table), 
                                     uint128_t(1), uint128_t(1) );
       idump( (scope)(code)(table)(fc::uint128(*primary))(fc::uint128(*secondary)) );
       if( item.scope == uint64_t(scope) )
          if( item.code == uint64_t(code) )
             if( item.table == uint64_t(table) )
             {
                ilog( "matching scope, code, table" );
                /*
                if( item.secondary_key >= uint128_t(0) ) {
                   idump( "secondary_key >= 0" );
                   if( item.primary_key >= uint128_t(0) )
                      idump( "primary_key >= 0" );
                }
                */
             }

       wdump( (item.scope)(item.code)(item.table)(fc::uint128(item.primary_key))(fc::uint128(item.secondary_key)) );
    }
    ilog(".");

    if( itr == idx.end() ) 
       return -1;

    /*
    --itr;

    if( itr->scope != scope ||
        itr->code != code ||
        itr->table != table ||
        itr->primary_key != *primary ) return -1;

    *secondary = itr->secondary_key;
    *primary = itr->primary_key;
    
    auto copylen =  std::min<size_t>(itr->value.size(),valuelen);
    if( copylen ) {
       itr->value.copy(value, copylen);
    }
    return copylen;
    */
    return -1;
}


int32_t message_validate_context::load_primary_i128i128( Name scope, Name code, Name table, 
                                 uint128_t* primary, uint128_t* secondary, char* value, uint32_t valuelen ) {

    require_scope( scope );
    const auto& idx = db.get_index<key128x128_value_index,by_scope_primary>();
    auto itr = idx.lower_bound( boost::make_tuple( uint64_t(scope), 
                                                  uint64_t(code), 
                                                  uint64_t(table), 
                                                  *primary, uint128_t(0) ) );

    if( itr == idx.end() ||
        itr->scope != uint64_t(scope) ||
        itr->code != uint64_t(code) ||
        itr->table != uint64_t(table) ||
        itr->primary_key != *primary ) return -1;

    *secondary = itr->secondary_key;
    
    auto copylen =  std::min<size_t>(itr->value.size(),valuelen);
    if( copylen ) {
       itr->value.copy(value, copylen);
    }
    return copylen;
}
int32_t message_validate_context::lowerbound_primary_i128i128( Name scope, Name code, Name table, 
                                 uint128_t* primary, uint128_t* secondary, char* value, uint32_t valuelen ) {

   /*
   require_scope( scope );
   const auto& idx = db.get_index<key128x128_value_index,by_scope_primary>();
   auto itr = idx.lower_bound( boost::make_tuple( uint64_t(scope), 
                                                  uint64_t(code), 
                                                  uint64_t(table), 
                                                  *primary, uint128_t(0) ) );

   if( itr == idx.end() ||
       itr->scope != scope ||
       itr->code != code ||
       itr->table != table ) return -1;

   *primary   = itr->primary_key;
   *secondary = itr->secondary_key;

   auto copylen =  std::min<size_t>(itr->value.size(),valuelen);
   if( copylen ) {
      itr->value.copy(value, copylen);
   }
   return copylen;
   */
   return -1;
}

int32_t message_validate_context::lowerbound_secondary_i128i128( Name scope, Name code, Name table, 
                                 uint128_t* primary, uint128_t* secondary, char* value, uint32_t valuelen ) {

   /*
   require_scope( scope );
   const auto& idx = db.get_index<key128x128_value_index,by_scope_secondary>();
   auto itr = idx.lower_bound( boost::make_tuple( uint64_t(scope), 
                                                  uint64_t(code), 
                                                  uint64_t(table), 
                                                  uint128_t(0), *secondary  ) );

   if( itr == idx.end() ||
       itr->scope != scope ||
       itr->code != code ||
       itr->table != table ) return -1;

   *primary   = itr->primary_key;
   *secondary = itr->secondary_key;

   auto copylen =  std::min<size_t>(itr->value.size(),valuelen);
   if( copylen ) {
      itr->value.copy(value, copylen);
   }
   return copylen;
   */
   return -1;
}

int32_t apply_context::remove_i64( Name scope, Name table, Name key ) {
   require_scope( scope );

   const auto* obj = db.find<key_value_object,by_scope_key>( boost::make_tuple(
                                                            AccountName(scope), 
                                                            AccountName(code), 
                                                            AccountName(table), 
                                                            AccountName(key) ) );
   if( obj ) {
      mutable_db.remove( *obj );
      return 1;
   }
   return 0;
}

int32_t apply_context::store_i64( Name scope, Name table, Name key, const char* value, uint32_t valuelen ) {
   require_scope( scope );

   const auto* obj = db.find<key_value_object,by_scope_key>( boost::make_tuple(
                                                            AccountName(scope), 
                                                            AccountName(code), 
                                                            AccountName(table), 
                                                            AccountName(key) ) );
   if( obj ) {
      mutable_db.modify( *obj, [&]( auto& o ) {
         o.value.assign(value, valuelen);
      });
      return 0;
   } else {
      mutable_db.create<key_value_object>( [&](auto& o) {
         o.scope = scope;
         o.code  = code;
         o.table = table;
         o.key   = key;
         o.value.insert( 0, value, valuelen );
      });
      return valuelen;
   }
}

int32_t apply_context::store_i128i128( Name scope, Name table, uint128_t primary, uint128_t secondary,
                                       const char* value, uint32_t valuelen ) {
   const auto* obj = db.find<key128x128_value_object,by_scope_primary>( boost::make_tuple(
                                                            AccountName(scope), 
                                                            AccountName(code), 
                                                            AccountName(table), 
                                                            primary, secondary ) );
   idump(( *((fc::uint128_t*)&primary)) );
   idump(( *((fc::uint128_t*)&secondary)) );
   if( obj ) {
      wlog( "modify" );
      mutable_db.modify( *obj, [&]( auto& o ) {
         o.primary_key = primary;
         o.secondary_key = secondary;
         o.value.assign(value, valuelen);
      });
      return valuelen;
   } else {
      wlog( "new" );
      mutable_db.create<key128x128_value_object>( [&](auto& o) {
         o.scope = scope;
         o.code  = code;
         o.table = table;
         o.primary_key = primary;
         o.secondary_key = secondary;
         o.value.insert( 0, value, valuelen );
      });
      return valuelen;
   }
}

} } // namespace hotc::chain
