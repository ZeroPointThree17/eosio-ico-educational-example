#pragma once
#include <eoslib/types.hpp>
#include <eoslib/message.hpp>
#include <eoslib/datastream.hpp>
#include <eoslib/raw_fwd.hpp>

namespace eosio { namespace raw {
   template<typename Stream> inline void pack( Stream& s, const currency_tokens& value ) {
      raw::pack(s, value.quantity);
   }
   template<typename Stream> inline void unpack( Stream& s, currency_tokens& value ) {
      raw::unpack(s, value.quantity);
   }
   template<typename Stream> inline void pack( Stream& s, const transfer& value ) {
      raw::pack(s, value.from);
      raw::pack(s, value.to);
      raw::pack(s, value.quantity);
   }
   template<typename Stream> inline void unpack( Stream& s, transfer& value ) {
      raw::unpack(s, value.from);
      raw::unpack(s, value.to);
      raw::unpack(s, value.quantity);
   }
   template<typename Stream> inline void pack( Stream& s, const tokens& value ) {
      raw::pack(s, value.quantity);
   }
   template<typename Stream> inline void unpack( Stream& s, tokens& value ) {
      raw::unpack(s, value.quantity);
   }
   template<typename Stream> inline void pack( Stream& s, const account& value ) {
      raw::pack(s, value.key);
      raw::pack(s, value.balance);
   }
   template<typename Stream> inline void unpack( Stream& s, account& value ) {
      raw::unpack(s, value.key);
      raw::unpack(s, value.balance);
   }
} }

#include <eoslib/raw.hpp>
namespace eosio {
   void print_ident(int n){while(n-->0){print("  ");}};
   template<typename Type>
   Type current_message_ex() {
      uint32_t size = message_size();
      char* data = (char *)eosio::malloc(size);
      assert(data && read_message(data, size) == size, "error reading message");
      Type value;
      eosio::raw::unpack(data, size, value);
      eosio::free(data);
      return value;
   }
   void dump(const currency_tokens& value, int tab=0) {
      print_ident(tab);print("quantity:[");printi(uint64_t(value.quantity));print("]\n");
   }
   template<>
   currency_tokens current_message<currency_tokens>() {
      return current_message_ex<currency_tokens>();
   }
   void dump(const transfer& value, int tab=0) {
      print_ident(tab);print("from:[");printn(value.from);print("]\n");
      print_ident(tab);print("to:[");printn(value.to);print("]\n");
      print_ident(tab);print("quantity:[");print("\n"); eosio::dump(value.quantity, tab+1);print_ident(tab);print("]\n");
   }
   template<>
   transfer current_message<transfer>() {
      return current_message_ex<transfer>();
   }
   void dump(const tokens& value, int tab=0) {
      print_ident(tab);print("quantity:[");printi(uint64_t(value.quantity));print("]\n");
   }
   template<>
   tokens current_message<tokens>() {
      return current_message_ex<tokens>();
   }
   void dump(const account& value, int tab=0) {
      print_ident(tab);print("key:[");printi(uint64_t(value.key));print("]\n");
      print_ident(tab);print("balance:[");print("\n"); eosio::dump(value.balance, tab+1);print_ident(tab);print("]\n");
   }
   template<>
   account current_message<account>() {
      return current_message_ex<account>();
   }
} //eosio

