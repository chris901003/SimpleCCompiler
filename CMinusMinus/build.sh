EXECUTABLE="a.out"

files=(
    "./Lexer/lexer.cpp"
    "./Parser/parser.cpp"
    "./LLVMController/llvm_controller.cpp"
)
files_string=$(printf " %s" "${files[@]}")

clang++ -I/opt/homebrew/opt/llvm/include -L/opt/homebrew/opt/llvm/lib -lLLVM main.cpp $files_string -o $EXECUTABLE --std=c++17