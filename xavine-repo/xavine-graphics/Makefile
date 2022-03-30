NAME:=xavine_graphics_third_party

all: packages
packages: debug release
	rsync --recursive --progress --delete --update build/packages/ ../packages/

debug:
	xmake f -p linux -a x86_64 -m debug -P
	xmake package -P

release:
	xmake f -p linux -a x86_64 -m release -P
	xmake package -P

clean:
	rm -rf build
