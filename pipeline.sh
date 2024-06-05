#!/bin/bash

# 設置文件名
EXECUTABLE="a.out"

./build.sh

test_files=(
    "main_function.txt"
    "declaration_assignment.txt"
    "variable_usable.txt"
)

for file in "${test_files[@]}"; do
    ./${EXECUTABLE} < ./test/${file} > ./test_result/${file}
done