#!/bin/bash

# �]�m���W
EXECUTABLE="a.out"

./build.sh

test_files=(
    "variable_declaration_assignment.txt"
    "if_statement.txt"
    "while_statement.txt"
    "for_statement.txt"
)

for file in "${test_files[@]}"; do
    ./${EXECUTABLE} < ./test/${file} > ./test_result/${file}
done