obj-m := lab5fs.o

all: module mkfs

mkfs:
	gcc lab5mkfs.c -o mkfs

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm mkfs
