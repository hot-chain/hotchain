#pragma once
#include <hotc/account_history_plugin/account_history_plugin.hpp>
#include <hotc/chain_plugin/chain_plugin.hpp>
#include <hotc/http_plugin/http_plugin.hpp>

#include <appbase/application.hpp>

namespace hotc {

   using namespace appbase;

   class account_history_api_plugin : public plugin<account_history_api_plugin> {
      public:
        APPBASE_PLUGIN_REQUIRES((account_history_plugin)(chain_plugin)(http_plugin))

        account_history_api_plugin();
        virtual ~account_history_api_plugin();

        virtual void set_program_options(options_description&, options_description&) override;

        void plugin_initialize(const variables_map&);
        void plugin_startup();
        void plugin_shutdown();

      private:
   };

}
