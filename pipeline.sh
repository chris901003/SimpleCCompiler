#!/bin/bash

# 設置文件名
EXECUTABLE="a.out"

./build.sh

test_files=(
    "variable_declaration_assignment.txt"
    "if_statement.txt"
    "while_statement.txt"
    "for_statement.txt"
    "function_declaration.txt"
    "function_declaration_with_parameters.txt"
    "call_function.txt"
    "include_header.txt"
)

for file in "${test_files[@]}"; do
    ./${EXECUTABLE} < ./test/${file} > ./test_result/${file}
done

error_test_files=(
    "variable_declaration_assignment_error.txt"
    "if_statement_error.txt"
    "while_statement_error.txt"
    "for_statement_error.txt"
    "function_declaration_error.txt"
    "function_definition_error.txt"
    "call_function_error.txt"
)

for file in "${error_test_files[@]}"; do
    ./${EXECUTABLE} < ./error_test/${file} > ./error_test_result/${file}
done