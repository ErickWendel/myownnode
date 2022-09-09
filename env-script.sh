ccache --version
ccache g++ --version
cmake --version

git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git

# add patch
mkdir v8 && cd v8
gclient
fetch v8

cd v8
git checkout 4ec5bb4f26

tools/dev/v8gen.py arm64.release.sample
echo 'v8_target_cpu = "arm64"' >> out.gn/arm64.release.sample/args.gn 
echo 'cc_wrapper="ccache"' >> out.gn/arm64.release.sample/args.gn 

# gn args out.gn/arm64.release.sample

time ninja -C out.gn/arm64.release.sample v8_monolith

cp out.gn/arm64.release.sample/obj/libv8_monolith.a ../../app3/v8/
cp out.gn/arm64.release.sample/icudtl.dat ../../app3/v8/

cp -r include ../../app3/v8/