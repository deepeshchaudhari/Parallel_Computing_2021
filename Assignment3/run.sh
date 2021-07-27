#!/bin/bash
make
rm -vf "output.txt"
rm -vf "time_data.txt"

for np in {1,2}
do
	for ppn in {1,2,4} 
	do
		python script.py 1 $np $ppn
		echo " mpirun -np $((np*ppn)) -ppn $ppn ./code.o tdata.csv"
		mpirun -np $((np*ppn)) -f hostfile ./code.o tdata.csv >> "output.txt"
	done
done
# python plot.py
