EXECUTABLE="a.out"

files=(
    "./Lexer/lexer.cpp"
)
files_string=$(printf " %s" "${files[@]}")

g++ -o $EXECUTABLE main.cpp $files_string