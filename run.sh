#!/bin/sh
cd $(dirname $0)
qemu-kvm -m 32 -kernel boot/kernel.bin
