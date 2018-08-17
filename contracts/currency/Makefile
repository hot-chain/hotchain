currency.wast: currency.cpp Makefile ../hotclib/hotc.hpp currency.hpp
	 /usr/local/Cellar/llvm/4.0.0_1/bin/clang-4.0 -emit-llvm -O3 --std=c++14 --target=wasm32 -c currency.cpp  -I.. -fno-threadsafe-statics -fno-rtti -fno-exceptions -o .currency.bc -I /usr/local/Cellar/llvm/4.0.0_1/include/c++/v1/
	 /Users/dlarimer/Downloads/llvm/build/bin/llc -asm-verbose=false .currency.bc
	 /Users/dlarimer/hotc/libraries/binaryen/bin/s2wasm -s 1024 .currency.s > currency.wast
	 wc -l currency.wast
	 echo '#pragma once' > currency.wast.hpp
	 echo 'const char* currency_wast = R"=====(' >> currency.wast.hpp
	 cat currency.wast >> currency.wast.hpp
	 echo ')=====";' >> currency.wast.hpp

#	 /usr/local/Cellar/llvm/4.0.0_1/bin/clang-4.0 -emit-llvm -O3 --std=c++14 --target=wasm32 -nostdinc -c currency.cpp  -I.. -fno-threadsafe-statics -fno-rtti -fno-exceptions -o .currency.bc
#	 /usr/local/Cellar/llvm/4.0.0_1/bin/clang-4.0 -emit-llvm  --std=c++14 --target=wasm32 -nostdinc -c currency.cpp  -I.. -fno-threadsafe-statics -fno-rtti -fno-exceptions -o .currency.bc

test.wast: test.cpp Makefile hotc.hpp
	 /usr/local/Cellar/llvm/4.0.0_1/bin/clang-4.0 -emit-llvm -O3 --std=c++14 --target=wasm32 -nostdinc -c test.cpp  -I.. -fno-threadsafe-statics -fno-rtti -fno-exceptions
	 /Users/dlarimer/Downloads/llvm/build/bin/llc -asm-verbose=false test.bc
	 /Users/dlarimer/hotc/libraries/binaryen/bin/s2wasm test.s -s 1024  > test.wast
	 cat test.wast

