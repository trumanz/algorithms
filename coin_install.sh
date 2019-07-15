#!/bin/sh
DIR_3PP=$(pwd)/3pp
DIR_3PP_INSTALL=${DIR_3PP}/install_dir

mkdir  -p  ${DIR_3PP_INSTALL} && cd  ${DIR_3PP} \
wget https://www.coin-or.org/download/source/Bcp/Bcp-1.4.3.zip && \
unzip Bcp-1.4.3.zip  && cd Bcp-1.4.3 && mkdir build && cd build &&  ../configure --prefix=${DIR_3PP_INSTALL} && \
make && make test && make install
