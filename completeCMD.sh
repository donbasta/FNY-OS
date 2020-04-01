#!/bin/bash

dd if=/dev/zero of=system.img bs=512 count=2880
dd if=/dev/zero of=sectors.img bs=512 count=1
dd if=/dev/zero of=dir.img bs=512 count=2

nasm bootloader.asm -o bootloader
dd if=bootloader of=system.img bs=512 count=1 conv=notrunc
dd if=map.img of=system.img bs=512 count=1 seek=256 conv=notrunc
dd if=dir.img of=system.img bs=512 count=2 seek=257 conv=notrunc
dd if=sectors.img of=system.img bs=512 count=1 seek=259 conv=notrunc

#loading shell application file into the kernel

bcc -ansi -c -o kernel.o kernel.c
bcc -ansi -c -o divmod.o divmod.c
bcc -ansi -c -o teks.o teks.c
bcc -ansi -c -o fileIO.o fileIO.c
bcc -ansi -c -o lib_lol.o lib_lol.c
bcc -ansi -c -o util.o util.c
bcc -ansi -c -o shell.o shell.c
# bcc -ansi -c -o exec.o exec.c
nasm -f as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o divmod.o teks.o fileIO.o lib_lol.o shell.o util.o
# ld86 -o kernel -d kernel.o kernel_asm.o teks.o fileIO.o
dd if=kernel of=system.img bs=512 conv=notrunc seek=1


# bcc -ansi -c -o kernel_lib.o kernel_lib.c

# nasm -f as86 kernel.asm -o kernel_asm.o
# ld86 -o shell -d shell.o divmod.o teks.o fileIO.o kernel_asm.o util.o lib_lol.o
# ld86 -o shell -d shell.o kernel_asm.o divmod.o teks.o fileIO.o

# ./loadFile milestone1
# ./loadFile app
# ./loadFile shell
./loadFile hahaha
./loadFile bismillah

bochs -f if2230.config