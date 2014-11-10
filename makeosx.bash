#!/bin/bash

echo "Build of $1 start"
g++ -O3 -ftree-vectorizer-verbose "$1"
otp=$(echo "$1" | cut -d"." -f1)
mv a.out "osx""$otp"