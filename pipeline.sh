#!/bin/bash

# �]�m���W
EXECUTABLE="a.out"

./build.sh

test_files=(
    "main_function.txt"
)

for file in "${test_files[@]}"; do
    ./${EXECUTABLE} < ./test/${file}
done