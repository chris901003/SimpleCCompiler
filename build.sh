#!/bin/bash

# 設置文件名
LEXER_FILE="lexer.l"
PARSER_FILE="parser.y"
LEXER_OUTPUT="lex.yy.c"
PARSER_OUTPUT="parser.tab.c"
PARSER_HEADER="parser.tab.h"
EXECUTABLE="a.out"

# 檢查 Flex 和 Bison 是否安裝
if ! command -v flex &> /dev/null; then
    echo "Flex 未安裝，請安裝 Flex 後再試。"
    exit 1
fi

if ! command -v bison &> /dev/null; then
    echo "Bison 未安裝，請安裝 Bison 後再試。"
    exit 1
fi

# 運行 Flex 和 Bison
flex $LEXER_FILE
bison -d $PARSER_FILE

# 檢查 Flex 和 Bison 生成的文件是否存在
if [[ ! -f $LEXER_OUTPUT ]] || [[ ! -f $PARSER_OUTPUT ]] || [[ ! -f $PARSER_HEADER ]]; then
    echo "生成詞法分析器或語法分析器時出現錯誤。"
    exit 1
fi

# 編譯生成的 C 源文件
files=()
files_string=$(printf " %s" "${files[@]}")

gcc -o $EXECUTABLE $PARSER_OUTPUT $LEXER_OUTPUT $files_string

# 檢查是否成功生成可執行文件
if [[ ! -f $EXECUTABLE ]]; then
    echo "編譯過程中出現錯誤。"
    exit 1
fi

echo "Build success ./$EXECUTABLE to start."