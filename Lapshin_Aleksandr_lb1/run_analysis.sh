#!/bin/bash

g++ main.cpp -o program

echo "" > results.csv
for i in {2..20}
do
    echo "Запуск для N = $i"
    echo $i | ./program
done

python3 ./generate_graph.py

# rm results.csv