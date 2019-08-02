#!/bin/bash
gcc -shared -fPIC zffqueue.c -lrt -o libzffqueue.so
