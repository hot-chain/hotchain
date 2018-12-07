#include <appbase/application.hpp>

#include <hotc/producer_plugin/producer_plugin.hpp>
#include <hotc/chain_plugin/chain_plugin.hpp>
#include <hotc/http_plugin/http_plugin.hpp>
#include <hotc/chain_api_plugin/chain_api_plugin.hpp>
#include <hotc/net_plugin/net_plugin.hpp>
#include <hotc/account_history_plugin/account_history_plugin.hpp>
#include <hotc/account_history_api_plugin/account_history_api_plugin.hpp>
#include "hotc/wallet_plugin/wallet_plugin.hpp"
#include "hotc/wallet_api_plugin/wallet_api_plugin.hpp"

#include <fc/log/logger_config.hpp>
#include <fc/exception/exception.hpp>

#include <boost/exception/diagnostic_information.hpp>

using namespace appbase;
using namespace hotc;

int main(int argc, char** argv)
{
   try {
      app().register_plugin<net_plugin>();
      app().register_plugin<chain_api_plugin>();
      app().register_plugin<http_plugin>();
      app().register_plugin<producer_plugin>();
      app().register_plugin<chain_plugin>();
      app().register_plugin<account_history_plugin>();
      app().register_plugin<account_history_api_plugin>();
      app().register_plugin<wallet_plugin>();
      app().register_plugin<wallet_api_plugin>();
      if(!app().initialize(argc, argv))
         return -1;
      app().startup();
      app().exec();
   } catch (const fc::exception& e) {
      elog("${e}", ("e",e.to_detail_string()));
   } catch (const boost::exception& e) {
      elog("${e}", ("e",boost::diagnostic_information(e)));
   } catch (const std::exception& e) {
      elog("${e}", ("e",e.what()));
   } catch (...) {
      elog("unknown exception");
   }
   return 0;
}
