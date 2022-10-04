# CapivaraBR - Creating your own JavaScript runtime tutorial
![capivara logo](./capivara.jpeg)

## Overview
CapivaraBR is a JavaScript runtime built on top of [V8 Javascript Engine](https://github.com/v8/v8) and [Libuv](https://github.com/libuv/libuv) 

- Use the environment to follow the tutorial on a [Gitpod snapshot](https://gitpod.io#snapshot/448f92e9-efaf-4356-805e-6215c87b76c2)

## About
Welcome, this repo is part of my youtube video about Recreating the Node.js project from Scratch using V8, Libuv and C++

First of all, leave your star 🌟 on this repo.

Access our [**exclusive telegram channel**](https://bit.ly/ErickWendelContentHub) so I'll let you know about all the content I've been producing 

## Running

- In the GitPod env you can run the hello world example by running the executable [./start.sh](./recorded/capivara/start.sh) script. 

### Notes

- It comes with `live reload` enabled for `C++` and `JS` files using `nodemon`
- It comes with a simple `Print` function, a custom C++ function used as a `console.log` function
- The [./start.sh](./recorded/capivara/start.sh) script generates a `binary` called `capivara`. 
    - It works similarly to how `Node.js` works. 
        - By running `capivara index.js` it will read the `JavaScript` content from the file and use `V8` to interpret it nd `libuv` to wait for async events.

- Project uses `V8 v10.7.0`
- Project uses `Libuv v1.x`

- All other dependencies are listed on [scripts](./scripts) folder.

## Examples

I left some file exmaples on [examples](./recorded/capivara/examples/) folder in case you wanna check out how to create [uv timers](./recorded/capivara/examples/uv-timers.cpp), [uv threads](./examples/uv-threads.cpp), [native cpp threads](./recorded/capivara/examples/cpp-native-threads.cpp) or the [v8 custom print function](./recorded/capivara/examples/v8-print-hello.cpp).

### Running the examples
- You can compile and execute examples by simply executing 
```sh
make example-without-extension
```
For example executing the [v8-print-hello.cpp](./recorded/capivara/examples/v8-print-hello.cpp):
```sh
make v8-print-hello
# Hello, World!...
```
## Compiling binaries for other platforms

if you wanna compile binaries it by yourself, I left some scripts in case you wanna compile all binaries by yourself

- [Installing libuv](./recorded/capivara/scripts/libuv-env-script-x86.sh)
- [Installing v8](./recorded/capivara/scripts/v8-env-script-x86.sh)

## Author

- [ErickWendel](https://github.com/erickwendel)
