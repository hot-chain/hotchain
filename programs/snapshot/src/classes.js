class Registrant {

  constructor( eth, hotc = "", balance = 0 ){
    // let {unclaimed, wallet, recovered} = balances
    this.eth      = eth
    this.hotc      = hotc
    this.balance  = typeof balance == 'object' ? balance : new Balance()
    this.accepted = null
    this.error    = false
  }

  accept ( callback ) {
    this.accepted = true
  }

  reject ( error ) {
    if(!error) return;
    this.error    = error
    this.accepted = false
  }

  is_accepted () {
    return this.accepted === true 
  }

}

class Transaction {

  constructor( eth, tx, type = "transfer", amount ) {
    this.eth     = eth
    this.hash    = tx
    this.amount  = amount
    this.claimed = false
    this.type    = type
  } 

  claim( eth ) {
    return ( eth == this.eth ) 
      ? this.claimed = true
      : log("error", `${eth} should't be claiming ${this.eth}'s transaction`)
  }

}

class RejectedRegistrant extends Registrant {
  constructor( error, registrant ) {
    
    const { eth, hotc, balance } = registrant
    
    super( registrant.eth, registrant.hotc, registrant.balance )
    
    this.error             = error
  
  }
}

class Balance {

  constructor(){
    this.wallet    = web3.toBigNumber(0)
    this.unclaimed = web3.toBigNumber(0)
    this.reclaimed = web3.toBigNumber(0)
    this.total     = web3.toBigNumber(0)
  }

  set( type, balance ){
    this[ type ] = balance
    return this //chaining
  }

  add( type, balance ){
    this['type'].plus( balance )
  }

  readable( type = 'total' ){
    this[ type ] = formatHOTC( this[ type ] )
  }

  exists( type ){
    return (typeof this[ type ] !== "undefined" && this[type].gt( 0 ))
  }

  get( type ){
    return (typeof this[ type ] !== "undefined" && this[type].gt( 0 )) ? this[type] : false;
  }

  sum(){
    this.total = this.wallet.plus(this.unclaimed).plus(this.reclaimed)
  }
} 
