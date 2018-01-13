1035	#	read A
2035	#	load A
3133	#	subtract TWO
4131	#	jneg END
2035	#CHECK:	load A
3232	#        divide FACTOR
3332	#        times FACTOR
3135	#        subtract A
4225	#	jzero DIVIDE
2035	#	load A
3134	#	subtract SQUARE
4130	#	jneg PQUIT
2032	#	load FACTOR
3133	#	subtract TWO
4218	#	jzero ADDTHREE
3033	#	add TWO
3033	#	add TWO
4021	#	jmp SQ
2032	#ADDTHR: load FACTOR
3232	#	divide FACTOR
3033	#	add TWO
2132	#SQ:     store FACTOR
3332	#	times FACTOR
2134	#	store SQUARE
4004	#        jmp CHECK
1132	#DIVIDE: print FACTOR
2035	#	load A
3232	#	divide FACTOR
2135	#	store A
4004	#	jmp CHECK
1135	#PQUIT:  print A
4300	#	halt
0002	#FACTOR:
0002	#TWO:
0004	#SQUARE:
