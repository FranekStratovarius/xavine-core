.PHONY: all clean LINUX WINDOWS WIN32 WIN64

OSX_CMAKE_PATH=PATH="/Applications/CMake.app/Contents/bin":"$PATH"

all: LINUX WINDOWS

LINUX:
	@mkdir -p build/linux
	@echo "#### building linux ####"
	@cd build/linux && cmake -DLINUX=ON ../.. && cmake --build .
	@cp -r src/assets build/linux

OSX:
	@mkdir -p build/osx
	@echo "#### building osx ####"
	@cd build/osx && $(OSX_CMAKE_PATH) cmake -DLINUX=ON ../.. && $(OSX_CMAKE_PATH) cmake --build .
	@cp -r src/assets build/osx

WINDOWS: WIN32 WIN64

WIN32:
	@mkdir -p build/windows32
	@echo "#### building win32 ####"
	@cd build/windows32 && cmake -DCMAKE_TOOLCHAIN_FILE=cmake/i686-w64-mingw32.cmake -DWINDOWS=ON ../.. && cmake --build .
	@cp -r src/assets build/windows64

WIN64:
	@mkdir -p build/windows64
	@echo "#### building win64 ####"
	@cd build/windows64 && cmake -DCMAKE_TOOLCHAIN_FILE=cmake/x86_64-w64-mingw32.cmake -DWINDOWS=ON ../.. && cmake --build .
	@cp -r src/assets build/windows64

clean:
	rm -rf build
#	@cd build && cmake .. && cmake --build . --target clean
