all: windows linux

windows:
	xmake f -p windows -a x86_64 -m debug
	xmake

linux:
	xmake f -p linux -a x86_64 -m debug
	xmake

clean:
	rm -rf build
