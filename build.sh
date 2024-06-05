#!/bin/bash

# �]�m���W
LEXER_FILE="lexer.l"
PARSER_FILE="parser.y"
LEXER_OUTPUT="lex.yy.c"
PARSER_OUTPUT="parser.tab.c"
PARSER_HEADER="parser.tab.h"
EXECUTABLE="a.out"

# �ˬd Flex �M Bison �O�_�w��
if ! command -v flex &> /dev/null; then
    echo "Flex ���w�ˡA�Цw�� Flex ��A�աC"
    exit 1
fi

if ! command -v bison &> /dev/null; then
    echo "Bison ���w�ˡA�Цw�� Bison ��A�աC"
    exit 1
fi

# �B�� Flex �M Bison
flex $LEXER_FILE
bison -d $PARSER_FILE

# �ˬd Flex �M Bison �ͦ������O�_�s�b
if [[ ! -f $LEXER_OUTPUT ]] || [[ ! -f $PARSER_OUTPUT ]] || [[ ! -f $PARSER_HEADER ]]; then
    echo "�ͦ����k���R���λy�k���R���ɥX�{���~�C"
    exit 1
fi

# �sĶ�ͦ��� C �����
files=()
files_string=$(printf " %s" "${files[@]}")

gcc -o $EXECUTABLE $PARSER_OUTPUT $LEXER_OUTPUT $files_string

# �ˬd�O�_���\�ͦ��i������
if [[ ! -f $EXECUTABLE ]]; then
    echo "�sĶ�L�{���X�{���~�C"
    exit 1
fi

echo "Build success ./$EXECUTABLE to start."