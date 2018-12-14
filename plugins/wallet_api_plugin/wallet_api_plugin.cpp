#include <hotc/wallet_api_plugin/wallet_api_plugin.hpp>
#include <hotc/wallet_plugin/wallet_manager.hpp>
#include <hotc/chain/exceptions.hpp>
#include <hotc/chain/transaction.hpp>

#include <fc/variant.hpp>
#include <fc/io/json.hpp>

#include <chrono>

namespace hotc { namespace detail {
  struct wallet_api_plugin_empty {};
}}

FC_REFLECT(hotc::detail::wallet_api_plugin_empty, );

namespace hotc {


using namespace hotc;


#define CALL(api_name, api_handle, call_name, INVOKE) \
{std::string("/v1/" #api_name "/" #call_name), \
   [&api_handle](string, string body, url_response_callback cb) mutable { \
          try { \
             if (body.empty()) body = "{}"; \
             INVOKE \
             cb(200, fc::json::to_string(result)); \
          } catch (fc::eof_exception&) { \
             cb(400, "Invalid arguments"); \
             elog("Unable to parse arguments: ${args}", ("args", body)); \
          } catch (fc::exception& e) { \
             cb(500, e.to_detail_string()); \
             elog("Exception encountered while processing ${call}: ${e}", ("call", #api_name "." #call_name)("e", e)); \
          } \
       }}

#define INVOKE_R_R(api_handle, call_name, in_param) \
     auto result = api_handle.call_name(fc::json::from_string(body).as<in_param>());

#define INVOKE_R_R_R(api_handle, call_name, in_param0, in_param1) \
     const auto& vs = fc::json::json::from_string(body).as<fc::variants>(); \
     auto result = api_handle.call_name(vs.at(0).as<in_param0>(), vs.at(1).as<in_param1>());

#define INVOKE_R_V(api_handle, call_name) \
     auto result = api_handle.call_name();

#define INVOKE_V_R(api_handle, call_name, in_param) \
     api_handle.call_name(fc::json::from_string(body).as<in_param>()); \
     hotc::detail::wallet_api_plugin_empty result;

#define INVOKE_V_R_R(api_handle, call_name, in_param0, in_param1) \
     const auto& vs = fc::json::json::from_string(body).as<fc::variants>(); \
     api_handle.call_name(vs.at(0).as<in_param0>(), vs.at(1).as<in_param1>()); \
     hotc::detail::wallet_api_plugin_empty result;

#define INVOKE_V_V(api_handle, call_name) \
     api_handle.call_name(); \
     hotc::detail::wallet_api_plugin_empty result;


void wallet_api_plugin::plugin_startup() {
   ilog("starting wallet_api_plugin");
   // lifetime of plugin is lifetime of application
   auto& wallet_mgr = app().get_plugin<wallet_plugin>().get_wallet_manager();

   // TODO: http_plugin needs to add ability to restrict to localhost, once added add call here.
   // TODO: For now see TODO below.

   app().get_plugin<http_plugin>().add_api({
       CALL(wallet, wallet_mgr, set_timeout,
            INVOKE_V_R(wallet_mgr, set_timeout, int64_t)),
       CALL(wallet, wallet_mgr, sign_transaction,
            INVOKE_R_R_R(wallet_mgr, sign_transaction, chain::SignedTransaction, chain::chain_id_type)),
       CALL(wallet, wallet_mgr, create,
            INVOKE_R_R(wallet_mgr, create, std::string)),
       CALL(wallet, wallet_mgr, open,
            INVOKE_V_R(wallet_mgr, open, std::string)),
       CALL(wallet, wallet_mgr, lock_all,
            INVOKE_V_V(wallet_mgr, lock_all)),
       CALL(wallet, wallet_mgr, lock,
            INVOKE_V_R(wallet_mgr, lock, std::string)),
       CALL(wallet, wallet_mgr, unlock,
            INVOKE_V_R_R(wallet_mgr, unlock, std::string, std::string)),
       CALL(wallet, wallet_mgr, import_key,
            INVOKE_V_R_R(wallet_mgr, import_key, std::string, std::string)),
       CALL(wallet, wallet_mgr, list_wallets,
            INVOKE_R_V(wallet_mgr, list_wallets)),
       CALL(wallet, wallet_mgr, list_keys,
            INVOKE_R_V(wallet_mgr, list_keys))
   });
}

void wallet_api_plugin::plugin_initialize(const variables_map& options) {
   // TODO: see TODO above, this is temporary until http_plugin has option to restrict to localhost
   if (options.count("http-server-endpoint")) {
      const auto& lipstr = options.at("http-server-endpoint").as<string>();
      const auto& host = lipstr.substr(0, lipstr.find(':'));
      if (host != "localhost" && host != "127.0.0.1") {
         FC_THROW("wallet api restricted to localhost");
      }
   }
}


#undef INVOKE_R_R
#undef INVOKE_R_V
#undef INVOKE_V_R
#undef INVOKE_V_V
#undef CALL

}
