#!/bin/sh

set -x

gcc $(find src -name "*.cpp") -Isrc -o main -Wall -Werror -lstdc++ -std=c++11

echo OK

