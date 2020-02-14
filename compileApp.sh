#!/bin/bash

bcc -ansi -c -o app.o app.c
bcc -ansi -c -o kernel_lib.o kernel_lib.c
nasm -f as86 kernel.asm -o kernel_asm.o
ld86 -o app -d app.o kernel_lib.o kernel_asm.o

bcc -ansi -c -o kernel.o kernel.c
ld86 -o kernel -d kernel.o kernel_asm.o
dd if=kernel of=system.img bs=512 conv=notrunc seek=3

./loadFile app
bochs -f if2230.config