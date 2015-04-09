#!bin/bash
READLINELINK="ftp://ftp.cwru.edu/pub/bash/readline-6.3.tar.gz"
wget $READLINELINK
tar -xvf readline-6.3.tar.gz && mv readline-6.3 readline
cd readline && ./configure --prefix=$PWD && make && make install
cd ..
make
