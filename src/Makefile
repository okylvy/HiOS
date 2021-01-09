QEMU=qemu-system-i386
QEMU_ARGS=\
	-fda os.img
#    -rtc base=localtime \
#	-drive file=os.img,format=raw
#	-boot order=c
NASM=nasm
GCC=gcc
LD=ld

.PHONY: clean

nasmhead.bin: nasmhead.asm
	$(NASM) -o nasmhead.bin nasmhead.asm

nasmfunc.o: nasmfunc.asm
	$(NASM) -f elf32 -o nasmfunc.o nasmfunc.asm

bootpack.o: bootpack.c
	$(GCC) -c -m32 -fno-pic -o bootpack.o bootpack.c

fonts.o: fonts.c
	$(GCC) -c -m32 -o fonts.o fonts.c

bootpack.bin: bootpack.o fonts.o nasmfunc.o
	$(LD) -m elf_i386 -e HiosMain -o bootpack.bin -T os.ls bootpack.o fonts.o nasmfunc.o

os.sys: nasmhead.bin bootpack.bin
	cat nasmhead.bin bootpack.bin > os.sys

ipl.bin: ipl.asm
	$(NASM) -o ipl.bin ipl.asm

img: ipl.bin os.sys
	mformat -f 1440 -C -B ipl.bin -i os.img ::
	mcopy -i os.img os.sys ::

run: img
	$(QEMU) $(QEMU_ARGS)

clean:
	rm -rf *.bin
	rm -rf *.o
	rm -rf *.sys
	rm -rf *.img
	rm -rf fonts.c
