#include <hotc/native_system_contract_plugin/native_system_contract_plugin.hpp>

#include <hotc/chain/account_object.hpp>
#include <hotc/chain/exceptions.hpp>

namespace hotc_hotc {
using namespace chain;

void Transfer_validate(chain::message_validate_context& context) {
   auto transfer = context.msg.as<Transfer>();
   HOTC_HOTC_ASSERT(context.msg.has_notify(transfer.to), message_validate_exception, "Must notify recipient of transfer");
}
void Transfer_validate_preconditions(chain::precondition_validate_context& context) {
   const auto& db = context.db;
   auto transfer = context.msg.as<Transfer>();
   const auto& from = db.get_account(context.msg.sender);
   HOTC_HOTC_ASSERT(from.balance > transfer.amount, message_precondition_exception, "Insufficient Funds",
              ("from.balance",from.balance)("transfer.amount",transfer.amount));
}
void Transfer_apply(chain::apply_context& context) {
   auto& db = context.mutable_db;
   auto transfer = context.msg.as<Transfer>();
   const auto& from = db.get_account(context.msg.sender);
   const auto& to = db.get_account(transfer.to);
   db.modify(from, [&](account_object& a) {
      a.balance -= transfer.amount;
   });
   db.modify(to, [&](account_object& a) {
      a.balance += transfer.amount;
   });
}

class native_system_contract_plugin_impl {
public:
   native_system_contract_plugin_impl(database& db)
      : db(db) {}

   database& db;
};

native_system_contract_plugin::native_system_contract_plugin()
   : my(new native_system_contract_plugin_impl(app().get_plugin<chain_plugin>().db())){}
native_system_contract_plugin::~native_system_contract_plugin(){}

void native_system_contract_plugin::plugin_initialize(const variables_map& options) {
   install(my->db);
}

void native_system_contract_plugin::plugin_startup() {
   // Make the magic happen
}

void native_system_contract_plugin::plugin_shutdown() {
   // OK, that's enough magic
}

void native_system_contract_plugin::install(database& db) {
#define SET_HANDLERS(name) \
   db.set_validate_handler("sys", "sys", #name, \
   [&db](chain::message_validate_context& c) mutable { name ## _validate(c); }); \
   db.set_precondition_validate_handler("sys", "sys", #name, \
   [&db](chain::precondition_validate_context& c) mutable { name ## _validate_preconditions(c); }); \
   db.set_apply_handler("sys", "sys", #name, \
   [&db](chain::apply_context& c) mutable { name ## _apply(c); })

   SET_HANDLERS(Transfer);
}
}
