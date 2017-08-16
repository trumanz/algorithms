#!/bin/sh
sudo apt-get update
sudo apt-get install -y python-setuptools g++

sudo apt-get install -y git bison flex python-setuptools python-dev autoconf libtool zlib1g-dev texinfo help2man gawk g++ curl texlive cmake subversion

#or-tools c++
tar xf or-tools_Ubuntu-14.04-64bit_v6.2.4392.tar.gz
ln -s or-tools_Ubuntu-14.04-64bit_v6.2.4392 or-tools
