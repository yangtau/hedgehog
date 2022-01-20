all: hg test

hg:
	make -C src

test:
	make -C src/test

clean:
	make -C src $@
	make -C src/test $@
