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
    this.multiplicand = randBetween(11,99);
    this.multiplier = randBetween(2,10);
    this.product = this.multiplicand * this.multiplier;
}

/*
 * Will update the page to show the problem
 */
function displayProblem() {

}

/*
 * Will update the page to show the answer
 */
function displayAnswer() {

}

var prob;

window.onload = function() {
    prob = new Problem("easy");
    displayProblem();
}
