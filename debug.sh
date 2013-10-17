#!/bin/sh
QEMUPID="/tmp/uosqemu.pid"
TARGET="boot/kernel.bin"

# all commands now relative to script location
cd $(dirname $0)

# start qemu in the background with the CPU halted and waiting for GDB on TCP 1234 
qemu-kvm $@ -daemonize -pidfile $QEMUPID -s -S -kernel $TARGET 

# wait for qemu to start up
sleep 1 

# run GDB and attach to the qemu instance
gdb $TARGET -ex 'target remote 127.0.0.1:1234'

# kill qemu once GDB exits
kill `cat $QEMUPID` > /dev/null 2>&1

exit 0
