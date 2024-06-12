#!/bin/bash

clang -O2 main.c list.c trie.c -o demo_live_preview
clang -O2 bench.c list.c trie.c -o demo_bench_time
