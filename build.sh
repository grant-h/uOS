#!/bin/sh
nasm -f elf -o loader.o loader.s
i686-pc-linux-gnu-gcc -o kernel.o -c kernel.c -Wall -Wextra -Werror \
    -nostdlib -fno-builtin -nostartfiles -nodefaultlibs
i686-pc-linux-gnu-ld -T linker.ld -o kernel.bin loader.o kernel.o

dd if=/dev/zero of=boot/pad bs=1 count=750 2>/dev/null
cat boot/grub/stage1 boot/grub/stage2 boot/pad kernel.bin > boot/kernel.img
