#!/bin/sh
cd $(dirname $0)
# troll my kernel with low memory
qemu-kvm -m 1.1 -kernel boot/kernel.bin
