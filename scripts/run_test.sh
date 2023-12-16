#!/bin/bash

thread_counts=(1 4 6 8)
file_limits=(100 250 500 10000 20000 60000)
xtrainFiles=("/Users/joao.paulo/dev/usp/oac2/OAC2-EP2/files/xtrain_100k.txt")
ytrainFiles=("/Users/joao.paulo/dev/usp/oac2/OAC2-EP2/files/ytrain_100k.txt")
xtestFile="/Users/joao.paulo/dev/usp/oac2/OAC2-EP2/files/xtest_10k.txt"
xtest10kFile="/Users/joao.paulo/dev/usp/oac2/OAC2-EP2/files/xtest_10k.txt"

cd ..

for threads in "${thread_counts[@]}"; do
    for limits in "${file_limits[@]}" ; do
        echo "==================================================================="
        ./cmake-build-release/EP2 "$threads" "${xtrainFiles[0]}" "${ytrainFiles[0]}" $xtestFile $limits
        echo ""
    done
done


cd scripts
