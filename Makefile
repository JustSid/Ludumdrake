KERNEL_DIR  = sys
LIBKERN_DIR = libkernel

ALLDIRS = $(KERNEL_DIR) $(LIBKERN_DIR)

all:
	for i in $(ALLDIRS); do make -C $$i; done

install: all
	./boot/CreateImage.sh

kernel:
	make -C $(KERNEL_DIR)

driver:
	make all -C $(LIBKERN_DIR)

clean:
	for i in $(ALLDIRS); do make clean -C $$i; done

help:
	@echo "List of valid targets:"
	@echo "	-all (default configuration, compiles everything)"
	@echo "	-install (like all, but also creates a mountable iso with Firedrake)"
	@echo "	-kernel (builds the kernel)"
	@echo "	-driver (builds all drivers and the driver framework libio)"

.PHONY: clean
.PHONY: driver
.PHONY: help
