obj-m = lab3.o
PWD = $(shell pwd)
all:
	make -C /lib/modules/$(shell uname -r)/build M="$(PWD)" modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M="$(PWD)" clean
install: all
	sudo insmod lab3.ko
uninstall:
	sudo rmmod lab3.ko