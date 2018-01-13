# HAS BUG >> INFINITE LOOP ON IDENTICAL NUMBERS, SEE 30numbers.sml
# for fix (which reverses the sort but doesn't halt).
# This will read in 20 numbers, sort them, and print them in descending
# order. It would be easy to change this to print them in ascending order
# by changing only the print statement at the end.  This uses almost all
# the memory.  The program takes 61 words to run and the data needs 20.
# This could probably handle sorting up the 39 words, but after that
# we have maxed out all the usable memory in this system.
1099	# read in here
2000	# load read loop
3107    # subtract one
2100	# store read loop
3108    # subtract ENDLOOP
4209	# jump zero to done
4000    # jump to the start
0001	# Data ONE
1079	# ENDLOOP
2080	# LOWER BOUND
2099	# UPPER BOUND
2080	# LOADA load a
3181	# SUBB sub a-b 
4136	# jneg NOSWAP
2080    # LOADA load a
2152    # store TMP
2081	# LOADB load b
2180	# STOREA  store a
2052    # load TMP
2181	# STOREB store b
2011	# load (load a)
3109	# subtract LOWER BOUND
4236	# jzero NOSWAP (keep going)
2011	# load (load a)
2116	# save it where load B was AC
3107	# decrement it
2111	# store at AA
2114	# store at AB
2017	# load (store a)
2119	# replace (store b)
3107	# decrement it
2117	# replace (store a)
2012	# load subtraction
3107	# decrement it
2112	# store it back
4011	# jump sort_start
2016	# NOSWAP increment all the a and b references
3110	# load (load b) subtract UPPER BOUND
4253	# end of program
2016	# load b
2114	# replace load a
2111	# replace load a
3007	# increment it
2116	# replace load b
2019	# load store b
2117	# replace store a
3007	# increment it
2119	# save it back
2012	# load subtraction
3007	# increment it
2112	# save it back
4011	# jump sort_start
0       #  fifty-two is temp
1199	# print here
2053	# load read loop
3107    # subtract one
2153	# store read loop
3161    # subtract ENDLOOP
4260	# jump zero to done
4053    # jump to the print
4300
1179
