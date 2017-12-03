/*
 * Define the FCC
 */
function FCC() {
    this.counter = 0;
    this.memory = [];
    this.acc = 0;
    this.running = false;
    this.error = "";
    
    for( var i = 0; i < 100; i++ ) {
	this.memory[i] = i;
    }

    this.errormessage = function( a ) {
	this.error = a;
    }

    this.load = function() {
	var dest = this.memory[ this.counter ] % 100;
	this.acc = this.memory[dest];
	this.counter++;
    }

    this.store = function() {
	var dest = this.memory[ this.counter ] % 100;
	this.memory[dest] = this.acc;
	this.counter++;
    }
    
    /*******
     * MATH FUNCTIONS
     *******/
    this.add = function() {
	var dest = this.memory[ this.counter ] % 100;
	this.acc += this.memory[dest];
	this.acc %= 10000;
	this.counter++;
    }

    this.subtract = function() {
	var dest = this.memory[ this.counter ] % 100;
	this.acc -= this.memory[dest];
	this.acc %= 10000;
	this.counter++;
    }

    this.divide = function() {
	var dest = this.memory[ this.counter ] % 100;
	var divisor = this.memory[dest];
	if( divisor == 0 ) {
	    this.errormessage( "DIVIDE BY ZERO" );
	    this.running = false;
	    return;
	}
	this.acc = Math.trunc(this.acc / divisor);
	this.counter++;
    }

    this.multiply = function() {
	var dest = this.memory[ this.counter ] % 100;
	this.acc *= this.memory[dest];
	this.acc %= 10000;
	this.counter++;
    }

    /*******
     * Control functions
     *******/
    this.branch = function () {
	this.counter = this.memory[ this.counter ] % 100;
    }
    
    this.branchneg = function () {
	var dest = this.memory[ this.counter ] % 100;
	if( this.acc < 0 ) {
	    this.counter = dest;
	} else {
	    this.counter++;
	}
    }

    this.branchzero = function () {
	var dest = this.memory[ this.counter ] % 100;
	if( this.acc == 0 ) {
	    this.counter = dest;
	} else {
	    this.counter++;
	}
    }

    this.halt = function () {
	var dest = this.memory[ this.counter ] % 100;
	this.errormessage( this.memory[ dest ] );
	this.running = false;
    }
}

var fcc = new FCC();

function run( a ) {
    while( a.running ) {
	switch ( parseInt( a.memory[a.counter] / 100 ) ) {
	case 20:
	    a.load();
	    break;
	case 21:
	    a.store();
	    break;
	case 30:
	    a.add();
	    break;
	case 31:
	    a.subtract();
	    break;
	case 32:
	    a.multiply();
	    break;
	case 33:
	    a.divide();
	    break;
	case 40:
	    a.branch();
	    break;
	case 41:
	    a.branchneg();
	    break;
	case 42:
	    a.branchzero();
	    break;
	case 43:
	    a.halt();
	    break;
	default:
	    a.running = false;
	    a.errormessage( "Unknown operation." );
	}
    }
}
