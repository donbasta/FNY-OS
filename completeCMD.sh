<<<<<<< HEAD
#!/bin/bash

dd if=/dev/zero of=system.img bs=512 count=2880

nasm bootloader.asm -o bootloader
dd if=bootloader of=system.img bs=512 count=1 conv=notrunc
dd if=map.img of=system.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=system.img bs=512 count=1 seek=2 conv=notrunc


bcc -ansi -c -o kernel.o kernel.c
nasm -f as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o
dd if=kernel of=system.img bs=512 conv=notrunc seek=3

./loadFile milestone1
=======
#!/bin/bash

dd if=/dev/zero of=system.img bs=512 count=2880

nasm bootloader.asm -o bootloader
dd if=bootloader of=system.img bs=512 count=1 conv=notrunc
dd if=map.img of=system.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=system.img bs=512 count=1 seek=2 conv=notrunc

#loading shell application file into the kernel
bcc -ansi -c -o shell.o shell.c
bcc -ansi -c -o kernel_lib.o kernel_lib.c
nasm -f as86 kernel.asm -o kernel_asm.o
ld86 -o shell -d shell.o kernel_lib.o kernel_asm.o

bcc -ansi -c -o kernel.o kernel.c
# nasm -f as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o
dd if=kernel of=system.img bs=512 conv=notrunc seek=3

#./loadFile milestone1
./loadFile shell
>>>>>>> 0a9cc3a06212326f74497f5213f6844c1e0688ea
bochs -f if2230.config