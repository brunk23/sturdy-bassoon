@0	read	87		# Get the desired value
	lda	88		# SUM()
	add	89
	add	90
	add	91
	add	92
	add	93		# Add up all the coins
	sub	87
	jzero	15		# This was a match
	jneg	11		#
	jump	18
	lda	88		# Load current pennies
	add	94		# Add another penny
	sta	88		# Save it
	jump	1		# Go back to SUM()
	lda	86		# HIT(14) get count
	add	94		# increase it
	sta	86		# Save it
	lda	88		# GREATER(18) get pennies
	jzero	26
	lda	85		# should be 0
	sta	88
	lda	89
	add	95
	sta	89
	jump	1
	lda	89		# NICK(26) get nickels
	jzero	34
	lda	85		# should be 0
	sta	89
	lda	90
	add	96
	sta	90
	jump	1
	lda	90		# DIME(34) get dimes
	jzero	42
	lda	85		# should be 0
	sta	90
	lda	91
	add	97
	sta	91
	jump	1
	lda	91		# QUART(42) get quarters
	jzero	50
	lda	85		# should be 0
	sta	91
	lda	92
	add	98
	sta	92
	jump	1
	lda	92		# HALF(50) get half-dollars
	jzero	58
	lda	82		# should be 0
	sta	92
	lda	93
	add	99
	sta	93
	jump	1
	write	86		# DOLLAR(58) -- we're done
	halt
@85 0 0 0 0 0 0 0 0 0
@94 1 5 10 25 50 100		# Value of each type of coin
go