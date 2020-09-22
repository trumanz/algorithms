#!/usr/bin/env bash

set -e 

sudo apt-get update
sudo apt-get install -y  libtool  cmake
sudo apt-get install -y libboost-all-dev
sudo apt-get install -y libjsoncpp-dev
sudo apt-get install -y  bison flex
sudo apt-get install -y libgtest-dev
sudo apt-get install -y doxygen 


cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
# copy or symlink libgtest.a and libgtest_main.a to your /usr/lib folder
sudo cp *.a /usr/lib
cd -

mkdir 3pp
cd 3pp
git clone https://github.com/p-ranav/tabulate.git
cd tabulate
git checkout v1.3
