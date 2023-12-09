#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Uso: $0 <caminho_absoluto_para_pasta_files>"
    exit 1
fi

k_values=(1 3 5)
thread_counts=(1 2 4 8)
lineCount=(10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 10000)

pathToFiles=$1

cd ..

for k in "${k_values[@]}"; do
    for threads in "${thread_counts[@]}"; do
        echo "Executando com k=$k e $threads threads, pasta de arquivos em $pathToFiles"
        ./cmake-build-release/EP2 $k $threads $pathToFiles
        echo ""
    done
done

cd scripts
