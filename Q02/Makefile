# Makefile for Cross Compilation 

obj-m := driver02.o 
KERN_DIR = /lib/modules/5.15.84-v7l+/build/

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERN_DIR) M=$(PWD) modules 

clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERN_DIR) M=$(PWD) clean 
