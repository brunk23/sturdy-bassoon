/*
 * The actual code for the game
 */

function processinput() {
    $("#world").append("<p>" + $("#he-said").val() + "</p>");
    $("#he-said").val("");
}

$("#title").text("There's no escape");

$("#world").append("<p>You are standing in the main lobby. To the "+
		       "east and west you see doors leading to the "+
		       "rest of the school. To the north there is a "+
		       "door leading back to the parking lot.</p>");
