#!/bin/bash

APP_DIR=../../app

git clone https://github.com/libuv/libuv
cd libuv && git checkout v1.x

mkdir -p build

(cd build && cmake .. -DBUILD_TESTING=ON)
cmake -j8 --build build


./build/uv_run_tests

cp -r include $APP_DIR/libuv/
cp -r build/libuv_a.a $APP_DIR/libuv/