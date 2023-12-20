#!/bin/bash

thread_counts=(1 4 6 8)
file_limits=(100 500 1000 5000 10000 20000 50000 100000 200000)
cd ..

#xtrainFile="./files/xtrain_100k.txt"
#ytrainFile="./files/ytrain_100k.txt"
xtrainFile="./files/xtrain200000.txt"
ytrainFile="./files/ytrain200000.txt"
xtestFile="./files/xtest_10k.txt"


for threads in "${thread_counts[@]}"; do
    for limits in "${file_limits[@]}" ; do
        echo "==================================================================="
        ./cmake-build-release/EP2 "$threads" "$xtrainFile" "$ytrainFile" $xtestFile $limits
        echo ""
    done
done


cd scripts
