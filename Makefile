# uOS Primary Makefile
CC=i686-pc-linux-gnu-gcc
LD=i686-pc-linux-gnu-ld
ASM=nasm

VPATH=src asm
BUILD_DIR=build

LINKING_INFO=linker.ld

ASMFLAGS=-f elf
LDFLAGS=-g -T $(LINKING_INFO) -L$(CURDIR)/lib/ -Map kernel.map
CFLAGS=-I$(CURDIR)/include/ -ggdb -Wall -Wextra -nostdlib -nostdinc -fno-builtin -nostartfiles -nodefaultlibs

CSRC=kernel.o \
     vga.o \
     assembly.o \
     string.o \
     gdt.o \
     idt.o \
     isr.o \
     kerror.o \
     print.o \
     irq.o \
     i8259.o \
     pit.o \
     kheap.o \
     memory.o
ASRC=loader.o gdt_x86.o idt_x86.o
SOURCES=$(CSRC) $(ASRC)
OBJECTS=$(addprefix $(BUILD_DIR)/, $(SOURCES)) 
EXECUTABLE=boot/kernel.bin


.PHONY: all clean run
all: $(EXECUTABLE)

#linking stage. All objects required to be compiled.
#Linking info required as well

$(EXECUTABLE) : $(BUILD_DIR) $(OBJECTS) 
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE) -lgcc

#Make sure our build directory is created
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

#Compiling C and Assembly files
#All C/Asm files required to be present
#A check for headers can be added later

$(BUILD_DIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s
	$(ASM) $(ASMFLAGS) $< -o $@

#BE CAREFUL!
clean: 
	-rm -f $(OBJECTS) $(EXECUTABLE)
	-rmdir $(BUILD_DIR)/
	-rm -f kernel.map
	-rm -f boot/kernel.img

# make sure everything is built first
run : all
	./run.sh	
