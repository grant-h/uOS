#!/bin/sh
QEMU=qemu-system-i386
TARGET="boot/kernel.bin"

# all commands now relative to script location
cd $(dirname $0)

# start qemu!
${QEMU} $@ -kernel $TARGET

exit 0
