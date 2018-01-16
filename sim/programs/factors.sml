read 96	#	read A
lda 96	#	load A
sub 97	#	subtract TWO
jneg 34	#	jneg END
lda 96	#CHECK:	load A
div 98	#        divide FACTOR
mult 98	#        times FACTOR
sub 96	#        subtract A
jzero 25	#	jzero DIVIDE
lda 96	#	load A
sub 99	#	subtract SQUARE
jneg 30	#	jneg PQUIT
lda 98	#	load FACTOR
sub 97	#	subtract TWO
jzero 18	#	jzero ADDTHREE
add 97	#	add TWO
add 97	#	add TWO
jump 21	#	jmp SQ
lda 98	#ADDTHR: load FACTOR
div 98	#	divide FACTOR
add 97	#	add TWO
sta 98	#SQ:     store FACTOR
mult 98	#	times FACTOR
sta 99	#	store SQUARE
jump 04	#        jmp CHECK
write 98	#DIVIDE: print FACTOR
lda 96	#	load A
div 98	#	divide FACTOR
sta 96	#	store A
jump 04	#	jmp CHECK
	#PQUIT:
lda 96
sub 97
jneg 34
write 96
halt	#	halt

@96
0000	# A
0002	#TWO:
0002	#FACTOR:
0004	#SQUARE:
