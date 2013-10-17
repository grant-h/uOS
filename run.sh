#!/bin/sh
TARGET="boot/kernel.bin"

# all commands now relative to script location
cd $(dirname $0)

# start qemu!
qemu-kvm $@ -kernel $TARGET 

exit 0
