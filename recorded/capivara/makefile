CXX = ccache g++
OUTPUT_FILE=bin/capivara

define INCLUDE
	v8/include/
endef

define INCLUDEUV
	libuv/include/
endef

define APP
	app/index.cc
endef
 
define OBJ
	v8/libv8_monolith.a
	libuv/libuv-x86_64-gitpod.a
endef

export INCLUDE
export INCLUDEUV
export OUTPUT_FILE

export APP
export LIB
export OBJ
export APP

examples=\
  cpp-native-threads\
  uv-threads\
  uv-timers\
  v8-print-hello

# will output the exectuable to ./bin folder
build:
	mkdir -p bin
	$(CXX) $$APP -I $$INCLUDE -I $$INCLUDEUV  -std=c++17 -pthread -o $$OUTPUT_FILE -DV8_COMPRESS_POINTERS $$OBJ -Wl,--no-as-needed -ldl

# make uv-threads (or any from examples)
$(examples): % : examples/%.cpp
	mkdir -p bin
	$(CXX) -I $$INCLUDE -I $$INCLUDEUV  -std=c++17 -pthread -o bin/$@  $< -DV8_COMPRESS_POINTERS $$OBJ -Wl,--no-as-needed -ldl
	./bin/$@
clean:
	rm -rf bin