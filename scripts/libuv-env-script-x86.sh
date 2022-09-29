#!/bin/bash

APP_DIR=../../app

git clone https://github.com/libuv/libuv
cd libuv && git checkout v1.x

sh autogen.sh
./configure
make
make check
make install

./build/uv_run_tests

cp -r include $APP_DIR/libuv/
cp -r .libs/libuv.a $APP_DIR/libuv/