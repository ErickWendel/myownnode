# Capivara - Creating your own JavaScript runtime tutorial

Capivara is a JavaScript runtime built on top of [V8 Javascript Engine](https://github.com/v8/v8) and [Libuv](https://github.com/libuv/libuv) 

- [Gitpod snapshot](https://gitpod.io#snapshot/df662415-2459-4157-a23b-a6de1c462e14)

## Running

- In the GitPod env you can run the hello world example by running the executable [./start.sh](./capivara/start.sh) script. 

### Notes

- It comes with `live reload` enabled for C++ and JS files using `nodemon`
- It comes with a simple `Print` function, a custom C++ function used as a `console.log` function
- The [./start.sh](./capivara/start.sh) script generates a `binary` called `capivara`. 
    - It works similarly to how `Node.js` works. By running `capivara index.js` it will read the JavaScript content from the file and use `V8` to interpret it nd `libuv` to wait for async events.

- Project uses `V8 v10.7.0`
- Project uses `Libu v1.x`

- All other dependencies are listed on [scripts](./scripts) folder.

## Examples

- I left some examples on [examples](./examples/) folder in case you wanna check out how to create [uv timers](./examples/uv-timers.cpp), [uv threads](./examples/uv-threads.cpp), [native cpp threads](./examples/cpp-native-threads.cpp) or the [v8 custom print function](./examples/v8-print-hello.cpp).

## if you wanna compile binaries it by yourself (NOT RECOMMENDED!)

I left some scripts in case you wanna compile all binaries by yourself

- [Installing libuv](./scripts/libuv-env-script-x86.sh)
- [Installing v8](./scripts/v8-env-script-x86.sh)

## Author

- [ErickWendel](https://github.com/erickwendel)