#pragma once
#include <hotclib/transaction.h>
#include <hotclib/print.hpp>

namespace hotc {

   /**
    * @defgroup transactioncppapi Transaction C++ API
    * @ingroup transactionapi
    * @brief Type-safe C++ wrapers for Transaction C API
    *
    * @note There are some methods from the @ref transactioncapi that can be used directly from C++
    *
    * @{ 
    */

   class Transaction;
   class Message {
   public:
      template<typename Message, typename ...Permissions>
      Message(const AccountName& code, const FuncName& type, const Message& message, Permissions... permissions ) 
         : handle(messageCreate(code, type, &message, sizeof(Message)))
      {
         addPermissions(permissions...);
      }

      // no copy construtor due to opaque handle
      Message( const Message& ) = delete;

      Message( Message&& msg ) {
         handle = msg.handle;
         msg.handle = InvalidMessageHandle;
      }

      ~Message() {
         if (handle != InvalidMessageHandle) {
            messageDrop(handle);
            handle = InvalidMessageHandle;
         }
      }

      void addPermissions(AccountName account, PermissionName permission) {
         messageRequirePermission(handle, account, permission);
      }

      template<typename ...Permissions>
      void addPermissions(AccountName account, PermissionName permission, Permissions... permissions) {
         addPermissions(account, permission);
         addPermissions(permissions...);
      }

      void send() {
         assertValidHandle();
         messageSend(handle);
         handle = InvalidMessageHandle;
      }

   private:
      void assertValidHandle() {
         assert(handle != InvalidMessageHandle, "attempting to send or modify a finalized message" );
      }

      MessageHandle handle;

      friend class Transaction;

   };

   class Transaction {
   public:
      Transaction() 
         : handle(transactionCreate())
      {}

      // no copy construtor due to opaque handle
      Transaction( const Transaction& ) = delete;

      Transaction( Transaction&& trx ) {
         handle = trx.handle;
         trx.handle = InvalidTransactionHandle;
      }

      ~Transaction() {
         if (handle != InvalidTransactionHandle) {
            transactionDrop(handle);
            handle = InvalidTransactionHandle;
         }
      }

      void addScope(AccountName scope, bool readOnly) {
         assertValidHandle();
         transactionRequireScope(handle, scope, readOnly ? 1 : 0);
      }

      template<typename P, typename T>
      void addMessage(Message &message) {
         assertValidHandle();
         message.assertValidHandle();
         transactionAddMessage(handle, message.handle);
         message.handle = InvalidMessageHandle;
      }

      void send() {
         assertValidHandle();
         transactionSend(handle);
         handle = InvalidTransactionHandle;
      }

      TransactionHandle get() {
         return handle;
      }

   private:
      void assertValidHandle() {
         assert(handle != InvalidTransactionHandle, "attempting to send or modify a finalized transaction" );
      }

      TransactionHandle handle;
   };


 ///@} transactioncpp api

} // namespace hotc
