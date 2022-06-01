#! /bin/bash
sudo apt install libevent-2.1-7 libevent-core-2.1-7 libevent-dev libevent-extra-2.1-7 libevent-openssl-2.1-7 libevent-pthreads-2.1-7

cd ./lib/libasyncd/lib/qlibc
cmake .
make -j8

cd ../..
./configure
make -j8
sudo make install