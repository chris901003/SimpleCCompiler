#!/bin/bash

# �]�m���W
EXECUTABLE="a.out"

./build.sh

test_files=(
    "variable_declaration_assignment.txt"
)

for file in "${test_files[@]}"; do
    ./${EXECUTABLE} < ./test/${file} > ./test_result/${file}
done