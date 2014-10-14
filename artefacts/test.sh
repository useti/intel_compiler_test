#!/bin/bash
pwd

echo ===Generating Task File===
echo $1
echo $2
./GCC/OMP/IntellBenchmark -G 320000 -I 10000 -c 100 -r 25 $2

echo ===Intell=== > $1
echo ===Intell===
echo ===TBB=== >> $1
echo ===TBB===
./Intel/TBB/IntellBenchmark $2 >> $1
echo ===OMP=== >> $1
echo ===OMP===
./Intel/OMP/IntellBenchmark $2 >> $1

echo ===GCC=== >> $1
echo ===GCC===
echo ===TBB=== >> $1
echo ===TBB===
./GCC/TBB/IntellBenchmark $2 >> $1
echo ===OMP=== >> $1
echo ===OMP===
./GCC/OMP/IntellBenchmark $2 >> $1
