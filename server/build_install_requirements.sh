#! /bin/bash
sudo apt install libevent-2.1-7 libevent-core-2.1-7 libevent-dev libevent-extra-2.1-7 libevent-openssl-2.1-7 libevent-pthreads-2.1-7 libssl-dev

cd ./lib/libasyncd/lib/qlibc
cmake .
make -j8

cd ../..
./configure
make -j8
sudo make install

cd ../..


sudo cp ./lib/libasyncd/lib/qlibc/lib/libqlibc.so.2 /usr/lib/libqlibc.so.2
sudo cp ./lib/libasyncd/lib/qlibc/lib/libqlibc.so /usr/lib/libqlibc.so
sudo cp ./lib/libasyncd/lib/qlibc/lib/libqlibcext.so.2 /usr/lib/libqlibcext.so.2
sudo cp ./lib/libasyncd/lib/qlibc/lib/libqlibcext.so /usr/lib/libqlibcext.so
sudo ln -s /usr/local/lib/libasyncd.so.1 /usr/lib/libasyncd.so.1

chmod +x update_database.sh
chmod +x count_branches.sh
