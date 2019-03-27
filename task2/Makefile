obj-m := onebyte.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:	
	$(MAKE) -C $(KDIR) M=`pwd` clean

# `pwd` can be used as a shorcut of $(shell pwd)


