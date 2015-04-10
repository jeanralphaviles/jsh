#!bin/bash
READLINELINK="ftp://ftp.cwru.edu/pub/bash/readline-6.3.tar.gz"

if [ ! -d "readline" ]; then
    # Control will enter here if $DIRECTORY doesn't exist.
    if [ ! -e "readline-6.3.tar.gz" ]; then
      wget $READLINELINK
    fi
    tar -xvf readline-6.3.tar.gz && mv readline-6.3 readline
fi

cd readline && ./configure --prefix=$PWD && make && make install
cd ..
make
