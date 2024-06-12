#!/bin/bash

clang -O2 main.c list.c trie.c -o demo && ./demo Data.txt
