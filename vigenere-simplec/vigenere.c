// A simple implementation of a Vigenère cipher
//
// For information on the algorithm, see
// http://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher
//
// Written by Jonas Wagner
// (C) 2012 Dependable Systems Lab, EPFL

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// The range of printable characters
#define MIN_PRINTABLE 32
#define NUM_PRINTABLE (126 - MIN_PRINTABLE)

// A command, either to encrypt or decrypt text
typedef enum EncryptionCommand {
    EC_NONE,
    EC_ENCRYPT,
    EC_DECRYPT
} EncryptionCommand;


int test_isascii(int c) {
#ifdef __APPLE__
  return isascii(c);
#else
  return __isascii(c);
#endif
}

// Encrypts C using the Vigenère cypher, with key k.
//
// This function ensures that the input and output are printable ASCII
// characters.
char encrypt(char c, char k) {
    assert(isprint(c) && isprint(k) && "encrypt arguments must be printable!");

    int key = k - MIN_PRINTABLE;
    int clear = c - MIN_PRINTABLE;
    int cypher = (clear + key) % NUM_PRINTABLE + MIN_PRINTABLE;

    assert(isprint(cypher) && "cyphertext must be printable!");
    return cypher;
}


// Decrypts C using the Vigenère cypher, with key k.
//
// This function ensures that the input and output are printable ASCII
// characters.
char decrypt(char c, char k) {
    assert(isprint(c) && isprint(k) && "encrypt arguments must be printable!");

    int key = k - MIN_PRINTABLE;
    int clear = c - MIN_PRINTABLE;
    int cypher = (clear - key + NUM_PRINTABLE) % NUM_PRINTABLE + MIN_PRINTABLE;

    assert(isprint(cypher) && "cyphertext must be printable!");
    return cypher;
}


int main(int argc, char *argv[]) {
    // Arguments sanity check
    if (argc != 3) {
        fputs("usage: vigenere encrypt|decrypt <key>\n", stderr);
        exit(EXIT_FAILURE);
    }

    // Command sanity check
    EncryptionCommand command = EC_NONE;
    if (strcmp(argv[1], "encrypt") == 0) {
        command = EC_ENCRYPT;
    } else if (strcmp(argv[1], "decrypt") == 0) {
        command = EC_DECRYPT;
    } else {
        fputs("usage: vigenere encrypt|decrypt <key>\n", stderr);
        exit(EXIT_FAILURE);
    }

    // Key sanity check
    const char *key = argv[2];
    const size_t key_size = strlen(key);
    if (key_size == 0) {
        fputs("Key may not be empty.\n", stderr);
        exit(EXIT_FAILURE);
    }
    for (const char *i = key, *e = key + key_size; i != e; ++i) {
        if (!isprint(*i)) {
            fputs("Key may only contain printable ASCII characters.\n", stderr);
            exit(EXIT_FAILURE);
        }
    }

    // Encrypt
    int i = 0;
    char c;
    while ((c = getchar()) != EOF) {
        if (!test_isascii(c)) {
            fputs("Input may only contain ASCII characters.\n", stderr);
            exit(EXIT_FAILURE);
        }

        if (isprint(c)) {
            if (command == EC_ENCRYPT) {
                putchar(encrypt(c, key[i]));
            } else {
                putchar(decrypt(c, key[i]));
            }
            i = (i + 1) % key_size;
        } else {
            putchar(c);
        }
    }

    return 0;
}
