&micro;OS (Micro OS or uOS)
===
An in progress, learning kernel for x86 created by Grant Hernandez.
This whole thing started as a hobby project in 2012 in an effort to learn more about the x86 architecture and kernels in general.

Here is a snapshot from the beginning of 2013 of &micro;OS running on my Thinkpad T420.

<img src="http://i.imgur.com/gELSJ.jpg"  />

Current Features
====
* Multiboot compliant
* Basic VGA driver (characters only)
* Interrupt and Global Descriptor Tables (IDT and GDT)
* Exception handling
* Paging (virtual memory)
* Higher-half kernel (kernel base 0xC0000000)
* Early kmalloc (temporary bootstrap heap)
* Timing using the Programmable Interval Timer (PIT)
* Very basic IRQ framework (callbacks)

Features Needed (Next in the pipeline)
====
* Physical and virtual memory managers (page allocators)
* Task Scheduling and tasking framework
* Atomic operations for mutual exclusion
* User-mode (ring 3 transition)

Features Wanted
====
* Rudimentary driver framework
* Keyboard and mouse support
* Hardware detection
* Multiplatform support (x86_64, ARM, MIPS, etc)
* Virtual File System (VFS) design
