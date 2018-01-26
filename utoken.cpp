/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#include <utoken.hpp> /// defines transfer struct (abi)


namespace UTOKEN {
   using namespace eosio;

   uint64_t tokens_per_eos = 3000ll*1000000000ll;
   uint64_t max_supply = ((1000000000ll)+6000ll)*1000000000ll;; // max supply + small buffer (9 decimals)
   time end_time = (unsigned long)1514659067;

   ///  When storing accounts, check for empty balance and remove account
   void store_account( account_name account_to_store, const account& a ) {
      if( a.is_empty() ) {
         ///               value, scope
         accounts::remove( a, account_to_store );
      } else {
         ///              value, scope
         accounts::store( a, account_to_store );
      }
   }

   void apply_currency_transfer( const UTOKEN::transfer& transfer_msg ) {

	  auto from = get_account( transfer_msg.from );
	  auto to   = get_account( transfer_msg.to );
	  auto token_account = get_account( N(utoken) );

	  if (token_account.balance.quantity < (6000*1000000000ll) || now() > end_time){
		  require_notice( transfer_msg.to, transfer_msg.from );
		  require_auth( transfer_msg.from );

		  from.balance.quantity -= transfer_msg.quantity.quantity; /// token subtraction has underflow assertion
		  to.balance.quantity   += transfer_msg.quantity.quantity; /// token addition has overflow assertion

		  store_account( transfer_msg.from, from );
		  store_account( transfer_msg.to, to );
	  } else {
	  	  assert( 1 == 2,  "Cannot transfer. Distribution has not yet ended.");
	  }
   }

   void accept_contribution( eosio::transfer transfer_msg ) {
	  auto from = get_account( transfer_msg.from );
	  auto token_account = get_account( N(utoken) );

	  if (token_account.balance.quantity < (6000*1000000000ll) && now() < end_time){

		  auto token_quantity = (transfer_msg.quantity.quantity * tokens_per_eos) / 10000;

		  if(token_quantity > to.balance.quantity){
			  assert( 1 == 2,  "Amount contributed would result in exceeding supply amount. Try contributing a smaller amount.");
		  }

		  from.balance.quantity += token_quantity;
		  to.balance.quantity -= token_quantity;

		  store_account( transfer_msg.from, from );
		  store_account( transfer_msg.to, to );

	  } else {
  		 assert( 1 == 2,  "Distribution has ended.");
 	  }
   }

}  // namespace TOKEN_NAME

using namespace UTOKEN;

extern "C" {

    void init()  {
       account owned_account;
       //Initialize currency account only if it does not exist
       if ( !accounts::get( owned_account, N(utoken) )) {
          store_account( N(utoken), account( currency_tokens(max_supply) ) );
       }
     }

    /// The apply method implements the dispatch of events to this contract
    void apply( uint64_t code, uint64_t action ) {

        if( code == N(eos) ) {
           if( action == N(transfer) ){
         		UTOKEN::accept_contribution( current_message< eosio::transfer >() );
           }
        } else if( code == N(utoken) ) {
            if( action == N(transfer) ){
         		 assert( 1 == 2, "Distribution period has not ended yet." );

         	  } else {
         		 UTOKEN::apply_currency_transfer( current_message< UTOKEN::transfer >() );
            }
        }
    }


}
