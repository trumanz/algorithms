#!/bin/sh
sudo apt-get install git bison flex python-setuptools python-dev autoconf \
libtool zlib1g-dev texinfo help2man gawk g++ curl texlive subversion && \
wget https://github.com/google/or-tools/archive/v6.4.tar.gz && \
tar xf v6.4.tar.gz && \
cd or-tools-6.4 && make third_party   && make cc
