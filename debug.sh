#!/bin/sh
QEMUPID="/tmp/uosqemu.pid"
TARGET="boot/kernel.bin"

exithook() {
  # kill qemu once GDB exits
  kill `cat $QEMUPID` > /dev/null 2>&1
}

# all commands now relative to script location
cd $(dirname $0)

# start qemu in the background with the CPU halted and waiting for GDB on TCP 1234 
qemu-system-i386 $@ -daemonize -pidfile $QEMUPID -s -S -kernel $TARGET 
trap exithook EXIT

# run GDB and attach to the qemu instance
gdb $TARGET -nx -ex 'target remote 127.0.0.1:1234'


exit 0
