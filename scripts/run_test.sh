#!/bin/bash

thread_counts=(1 4 6 8)
file_limits=(100 250 500 10000 20000 60000)
cd ..

xtrainFile="./files/xtrain_100k.txt"
ytrainFile="./files/ytrain_100k.txt"
xtestFile="./files/xtest_10k.txt"


for threads in "${thread_counts[@]}"; do
    for limits in "${file_limits[@]}" ; do
        echo "==================================================================="
        ./cmake-build-release/EP2 "$threads" "$xtrainFile" "$ytrainFile" $xtestFile $limits
        echo ""
    done
done


cd scripts
