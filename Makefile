all: linux #windows
release: linux-release
debug: linux-debug

linux: linux-debug linux-release

windows:
	xmake f -p windows -a x86_64 -m debug
	xmake

linux-debug:
	xmake f -p linux -a x86_64 -m debug
	xmake

linux-release:
	xmake f -p linux -a x86_64 -m release
	xmake

clean:
	rm -rf build
