# Creating your own JavaScript runtime tutorial

- [Gitpod snapshot](https://gitpod.io#snapshot/c39ad9cc-4a4c-4dc4-8d9c-2e027f0ec376)

## Running

- In the GitPod env you can run the hello world example by running the executable [./start.sh](./start.sh) script. 

It'll download and install `nodemon` (for hot reload) and `ccache` for cpp code caching.


## Examples

- I left some examples on [examples](./examples/) folder in case you wanna check out how to create [uv timers](./examples/uv-timers.cpp), [uv threads](./examples/uv-threads.cpp), [native cpp threads](./examples/cpp-native-threads.cpp) or the [v8 custom print function](./examples/v8-print-hello.cpp).

## if you wanna compile binaries it by yourself (NOT RECOMMENDED!)

I left some scripts in case you wanna compile all binaries by yourself

- [Installing libuv](./scripts/libuv-env-script-x86.sh)
- [Installing v8](./scripts/v8-env-script-x86.sh)

## Author

- [ErickWendel](https://github.com/erickwendel)