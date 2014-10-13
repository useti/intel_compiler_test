@echo off

echo ===Generating Task File===

intell\tbb\IntelBenchmark.exe -G 320000 -I 10000 -c 100 -r 25 %2

echo ===Intell=== > %1
echo ===Intell===
echo ===TBB=== >> %1
echo ===TBB===
intell\tbb\IntelBenchmark.exe %2 >> %1
echo ===OMP=== >> %1
echo ===OMP===
intell\omp\IntelBenchmark.exe %2 >> %1

echo ===MSVC=== >> %1
echo ===MSVC===
echo ===TBB=== >> %1
echo ===TBB===
msvc\tbb\IntelBenchmark.exe %2 >> %1
echo ===OMP=== >> %1
echo ===OMP===
msvc\omp\IntelBenchmark.exe %2 >> %1