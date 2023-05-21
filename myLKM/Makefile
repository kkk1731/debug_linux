KDIR		:= /lib/modules/`uname -r`/build
MODNAME		:= my_dlink_test

obj-m		:= $(MODNAME).o
$(MODNAME)-objs	:= main.o


all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` clean

dlink_test:
	sudo insmod $(MODNAME).ko func=$(FUNC)
