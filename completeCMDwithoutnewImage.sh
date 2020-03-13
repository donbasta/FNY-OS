bcc -ansi -c -o shell.o shell.c
bcc -ansi -c -o kernel_lib.o kernel_lib.c
nasm -f as86 kernel.asm -o kernel_asm.o
ld86 -o shell -d shell.o kernel_lib.o kernel_asm.o

bcc -ansi -c -o kernel.o kernel.c
nasm -f as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o
dd if=kernel of=system.img bs=512 conv=notrunc seek=1

./loadFile milestone1
./loadFile app

bochs -f if2230.config