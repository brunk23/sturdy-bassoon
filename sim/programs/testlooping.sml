lda 99	# loop1
jzero 9 # -> done
dec 99
lda 98
sta 97
lda 97	# loop2
jzero 0	# -> loop1
dec 97
jump 5	# -> loop2
halt #done
@98 9999 9999
go