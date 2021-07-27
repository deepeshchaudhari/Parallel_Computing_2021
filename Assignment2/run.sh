#!/usr/bin/env bash

make > /dev/null || exit 1

# Cleanup old output files
rm -vf data.txt

iterations=10
P=(4 16)
PPN=(1 8)
D=(16 256 2048)

for i in $(seq 1 $iterations); do
  for p in ${P[*]}; do
    for ppn in ${PPN[*]}; do
      printf "Iteration %d: P = %d, PPN = %d\nGenerating hostfile..." "$i" "$p" "$ppn"
      python script.py 4 $((p/4)) $ppn >& /dev/null || exit 1 
      printf "Done!\n"
      for d in ${D[*]}; do
        printf "Running for D = %d..." "$d"
        while : ; do
          mpirun -np $((p*ppn)) -f ./hostfile ./src.o $d  $ppn> "tmp.txt" && break
          printf "Iteration %d: P = %d, PPN = %d\n Generating hostfile not stuck..." "$i" "$p" "$ppn"
          python script.py 4 $((p/4)) $ppn >& /dev/null || exit 1 
          printf "\rRetrying for D = %d..." "$d"
        done
        cat tmp.txt >> "data.txt"
        printf "Done!\n"
      done
    done
  done
done

echo -n "Plotting data..."
python3 plot.py
printf "Done!\n"
