TOPDIR = $(CURDIR)

SUBDIR = libc drivers FreeRTOS
SOURCES = startup.c trap.c main.c

all: build

include $(TOPDIR)/build/build.vbcc.mk

build-here: bootloader.bin freertos.adf

freertos.exe: $(OBJECTS) drivers/drivers.lib libc/c.lib FreeRTOS/freertos.lib

freertos.adf: freertos.exe

vbcc:
	make -C external/vbcc DESTDIR=$(PWD)/toolchain

run: freertos.adf
	./launch $^

clean-here:
	$(RM) bootloader.bin
	$(RM) *.adf *.exe *~

.PHONY: vbcc run
