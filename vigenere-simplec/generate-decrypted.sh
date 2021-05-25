#!/bin/bash
echo "Running decrypt with embedded found hash"
while read hash; do ./vigenere decrypt $hash < challenge2.rst; done < keys.txt
time ./decryptor
while read hash; do ./vigenere decrypt $hash < challenge2.rst; done < keys.txt > decrypted.txt