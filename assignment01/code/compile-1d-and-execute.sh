unset n
gcc 1d.c -o 210123077-1d
./210123077-1d 

export n=10
./210123077-1d
unset n

export n="10 20 30 40 50 60 70 80 90 100"
./210123077-1d
unset n
