CXX = ccache g++

define INCLUDE
	v8/include/
endef

define INCLUDEUV
	libuv/include/
endef

define APP
	app/myownnode.cc
endef
 
define OBJ
	v8/libv8_monolith.a
	libuv/libuv_a.a
endef

export INCLUDE
export INCLUDEUV
export APP
export LIB
export OBJ
export APP


build:
	$(CXX) $$APP -I $$INCLUDE -I $$INCLUDEUV  -std=c++17 -pthread -o myapp -DV8_COMPRESS_POINTERS $$OBJ 
    
clean:
		rm -f hello_uv
		rm -f myapp