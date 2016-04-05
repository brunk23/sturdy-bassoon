/*
 * These will be variables used in our program. We set some constants
 * here, at the top, so they are easy to change and modify. It is a good
 * idea to not put the numbers directly in the code. We might want to
 * change them later and it can be hard to remember which 5s are the ones
 * we need to change. It can also help make the code more readable, as the
 * number "30" could mean anything but the variable LowestBeads makes the
 * meaning clear.
 */
var MaxRemoved = 4;
var LowestBeads = 30;
var HighestBeads = 50;
var remaining_beads = 10;

/*
 * Will return a random int between the two arguments, inclusive.
 */
function randBetween(low, high) {
    var i = 1;
    if( high > low ) {
	i += (high - low);
    } else {
	i += (low - high);
    }
    var j = Math.floor( Math.random() * i );
    if( high > low ) {
	j += low;
    } else {
	j += high;
    }
    return j;
}

/*
 * We call this to start the game
 */
function start_game() {
    remaining_beads = randBetween(LowestBeads, HighestBeads);
    create_beads();
}

/*
 * This will create the beads and put them on the screen.
 */
function create_beads() {

}

/*
 * This handles the main game loop
 */
function play_game() {

    // Infinite loop, we play until there is a winner
    while( true ) {

	// Update the display for the player and take their turn
	update_display();
	player_turn();

	// Check to see if the player won, if we did then we
	// run the code to tell the player and use break to
	// end the loop
	if( remaining_beads <= 0 ) {
	    win_game();
	    break;
	}

	// Update the display for the computer and it takes turn
	update_display();
	computer_turn();

	// Check to see if the computer won, if it did then we
	// run the code to tell the player and use break to
	// end the loop
	if( remaining_beads <= 0 ) {
	    lose_game();
	    break;
	}
    }
}

/*
 * The code that controls the player's turn
 */
function player_turn() {
    console.log(remaining_beads);
    remaining_beads--;
}

/*
 * The code that controls the computer's turn
 */
function computer_turn() {
    console.log(remaining_beads);
    remaining_beads -= 2;
}

/*
 * We call this if we win
 */
function win_game() {

}

/*
 * We call this if the computer wins
 */
function lose_game() {

}

// This function is probably going to be removed.
function update_display() {
    $("#he-said").val(remaining_beads);
}

/* Set the title based on the max we can take at once */
$("#title").text("Take up to " + MaxRemoved);

/* Start the game */
function game_loop() {
    start_game();
    play_game();
}

game_loop();
