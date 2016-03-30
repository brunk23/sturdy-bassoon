/*
 * The actual code for the game
 */

/*
 * These will be variables used in our program
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
 * Will print the number and update the screen
 */
function update_display() {

}

/*
 * This handles the main game loop
 */
function play_game() {
    while( true ) {
	update_display();
	player_turn();
	if( remaining_beads <= 0 ) {
	    win_game();
	    break;
	}
	update_display();
	computer_turn();
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

function processinput() {
    $("#world").append("<p>" + $("#he-said").val() + "</p>");
    $("#he-said").val("");
}


$("#title").text("Take up to " + MaxRemoved);

/* Start the game */
function game_loop() {
    start_game();
    play_game();
}

game_loop();
