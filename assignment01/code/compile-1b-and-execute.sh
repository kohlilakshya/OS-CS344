unset n
export n=10
gcc 1b.c -o 210123077-1b
./210123077-1b

unset n
./210123077-1b


unset n
export n="10 20 30"
./210123077-1b
