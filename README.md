# &micro;OS (Micro OS or uOS)
An in progress, learning kernel for x86 created by Grant Hernandez.

This project started as a hobby in early 2012 in an effort to learn more about the x86 architecture and kernels in general. All of my knowledge on this subject has been from the OSDev wiki and the Intel reference manuals. All code included is my own, including the lower-level library functions, such as `printf()`.

Here is a snapshot from the beginning of 2013 of &micro;OS running on my bare-metal Thinkpad T420.

<img src="http://i.imgur.com/gELSJ.jpg"  />

While the kernel currently just a toy, it serves as my testbed for kernel programming and fundamental Operating System concepts.

## Building and Environment Setup
&micro;OS currently only supports the 32-bit x86 architecture, but there are future plans to support x86\_64 and ARM. Testing is done using QEMU for x86 system emulation, but the kernel should be able to run on bare metal at *any* time.

The build tools required are GNU Make, NASM, and GCC.
I opted to use NASM instead of GNU AS as I prefer its syntax. This may change in the future as AS has better support within the GCC toolchain.

* **Distribution:** Ubuntu 16.04.3 LTS
* **Operating System:** Linux 4.10.0-33
* **Compiler:** GCC 5.4.0 (clang NOT supported)
* **Emulator:** QEMU emulator version 2.5.0

```
~/code/uOS $ make
mkdir -p build boot
gcc -m32 -I/home/digital/code/uOS/include/ -ggdb -Wall -Wextra -nostdlib -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -c src/kernel.c -o build/kernel.o
...
nasm -f elf asm/gdt_x86.s -o build/gdt_x86.o
nasm -f elf asm/idt_x86.s -o build/idt_x86.o
ld -m elf_i386 -g -T linker.ld ... -o boot/kernel.bin -lgcc
```

Once the kernel has built succesfully it will output an ELF executable which can be loaded
into QEMU for testing:
```
~/code/uOS $ ./run.sh
```

## Current Features
* Multiboot compliant
* Descriptor table setup (GDT and IDT)
* Interrupt and exception handling via PIC
* Very basic IRQ framework (callbacks)
* Basic VGA driver (characters only)
* Timing using the Programmable Interval Timer (PIT)
* Paging (virtual memory)
* Higher-half kernel (kernel base `0xC0000000`)
* Early kmalloc (temporary bootstrap heap)

## Features Needed (Next in the pipeline)
* Physical and virtual memory managers (page allocators)
* Task Scheduling and tasking framework
* Atomic operations for mutual exclusion
* User-mode (ring 3 transition)

## Long Term Features Wanted
* Rudimentary driver framework
* Keyboard and mouse support
* More hardware detection
* Virtual File System (VFS) design
* Multiplatform support (x86\_64, ARM)
