EXECUTABLE="a.out"

./build.sh

./$EXECUTABLE < file.txt

llvm-as a.ll -o a.bc
llc a.bc -o a.s
gcc a.s -o run.out