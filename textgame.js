/*
 * The actual code for the game
 */

var Room = function () {
    this.description = "";
    this.look = function () {
        $("#world").append("<p>" + this.description + "</p>" );
    }
    this.exits;
    this.contents;
};

var Object = function () {
    this.description = "";
    this.look = function () {
        $("#world").append("<p>" + this.description + "</p>" );
    }
    this.drop = function () {

    }
    this.pickup = function () {

    }
};

function processinput() {
    $("#world").append("<p>" + $("#he-said").val() + "</p>");
    $("#he-said").val("");
}

$("#title").text("This is the title");

var lobby = new Room();
lobby.description = "You are in the lobby of a building.";

var cokebottle = new Object();
cokebottle.description = "It's a coke bottle.";

var spritebottle = new Object();
spritebottle.description = "This sprite bottle is empty.";


lobby.look();
cokebottle.look();
spritebottle.look();
lobby.look();