read 99		#00 get number
lda 99		#01 load it
sta 98		#02 store a copy
lda 98		#03 get index
sub 12		#04 minus 1
jzero 10	#05 end if zero
sta 98		#06 save index
add 99		#07 add to sum
sta 99		#08 store the sum
jump 3		#09 back to start
write 99	#10 print sum
halt		#11 end program
0001
