/*
 * Will return a random integer from the range low-high
 * inclusive
 */
function randBetween(low, high) {
    var number = Math.floor(Math.random()*(high-low+1));
    number += low;
    return number;
}

/*
 * Will be a random multiplication problem. Difficulty
 * will be a string "easy", "normal", "hard" that determines
 * the type of problem.
 */
function Problem(difficulty) {
    if( "easy" == difficulty ) {
	var tmp = randBetween(1,2);
	if( tmp == 1 ) {
	    this.multiplicand = randBetween(1,19);
	    this.multiplicand *= 5;
	    this.multiplier = randBetween(2,9);
	} else {
	    this.multiplier = randBetween(1,9);
	    this.multiplicand = randBetween(1,9);
	    this.multiplicand *= 11;
	}
    } else {
	if( "normal" == difficulty ) {
	    this.multiplicand = randBetween(11,99);
	    this.multiplier = randBetween(2,10);
	} else {
	    if( "hard" != difficulty ) {
		console.log("Invalid difficulty level: " + difficulty);
	    }
	    this.multiplicand = randBetween(11,99);
	    this.multiplier = randBetween(2,12);
	}
    }
    this.product = this.multiplicand * this.multiplier;
}

/*
 * Will update the page to show the problem
 */
function displayProblem() {
    document.getElementById("top").innerHTML = '<font size="7">'
	+ prob.multiplicand + '</font>';
    document.getElementById("bottom").innerHTML = '<font size="7">x '
	+ prob.multiplier + '</font>';
    document.getElementById("ans").innerHTML = '<font size="7">?</font>';
}

/*
 * Will generate and display a new problem
 */
function newProb(level) {
    prob = new Problem(level);
    displayProblem();
    document.getElementById("butt").innerHTML =
	'<button onclick="displayAnswer()">Answer</button>';
}

/*
 * Will update the page to show the answer
 */
function displayAnswer() {
    document.getElementById("ans").innerHTML = '<font size="7">' +
	prob.product + '</font>';
    document.getElementById("butt").innerHTML =
	'<button onclick=newProb("easy")>New Easy Problem</button>' +
	'<button onclick=newProb("normal")>New Normal Problem</button>' +
	'<button onclick=newProb("hard")>New Hard Problem</button>';
}

var prob;

window.onload = function() {
    newProb("normal");
}
