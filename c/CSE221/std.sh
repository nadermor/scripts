#!/bin/sh

for i in $(seq 0 1000); do
	val=$(./main)
	echo $val >> res.txt
done

echo "(`paste -sd+ res.txt`)/1000" | bc -l
