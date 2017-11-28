#!/bin/bash

# update & install secp256k1
git submodule update --init --recursive
cd secp256k1
sh autogen.sh
./configure
make
cd ..

# make
make
