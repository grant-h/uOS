#!/bin/sh
dd if=/dev/zero of=boot/pad bs=1 count=750 2>/dev/null
cat boot/grub/stage1 boot/grub/stage2 boot/pad kernel.bin > boot/kernel.img
