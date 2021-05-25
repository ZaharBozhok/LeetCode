#!/bin/bash
clang++ -o simpc -std=c++20 -g ./simplec/simplec.cpp || exit;
for testFile in $(ls -l1 tests/*.in); do 
    ./simpc "$testFile" > "$testFile.c" &&                              \
        clang -std=c99 -c "$testFile.c" -o testc &&                       \
        echo "Generated C file from $testFile compiled successfully" ||   \
        echo "Impossible to generate C file from $testFile";
    rm -rf "$testFile.c" testc;
done;
rm -rf simpc
