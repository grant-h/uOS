# uOS Primary Makefile
BUILD_MODE="DEBUG"

CC=$(PREFIX)gcc
LD=$(PREFIX)gcc
ASM=nasm
QEMUOPT=-name uOS -m 32 -no-reboot -d all

VPATH=src asm
BUILD_DIR=build

LINKING_INFO=linker.ld
ASMFLAGS=-f elf
GCC_KERNEL=-nostdlib \
	   -nostdinc \
	   -fno-builtin \
	   -nostartfiles \
	   -nodefaultlibs

ifeq ($(BUILD_MODE), "DEBUG")
LDFLAGS=-Wl,-m,elf_i386 \
	-Wl,-Map,kernel.map \
	-ggdb \
	-T $(LINKING_INFO) \
	-L$(CURDIR)/lib/ \
	$(GCC_KERNEL)

CFLAGS=-m32 \
       -I$(CURDIR)/include/ \
       -ggdb \
       -Wall \
       -Wextra \
       $(GCC_KERNEL)
else ifeq ($(BUILD_MODE), "QUICK")
LDFLAGS=-O1 \
	-Wl,-m,elf_i386 \
	-T $(LINKING_INFO) \
	-L$(CURDIR)/lib/ \
	$(GCC_KERNEL)

CFLAGS=-O1 \
       -m32 \
       -I$(CURDIR)/include/ \
       -Wall \
       -Wextra \
       $(GCC_KERNEL)
else ifeq ($(BUILD_MODE), "RELEASE_LTO")
LDFLAGS=-O2 \
	-flto \
	-fwhole-program \
	-Wl,-m,elf_i386 \
	-Wl,-Map,kernel.map \
	-T $(LINKING_INFO) \
	-L$(CURDIR)/lib/ \
	$(GCC_KERNEL)
CFLAGS=-O2 \
       -flto \
       -fwhole-program \
       -m32 \
       -I$(CURDIR)/include/ \
       -Wall \
       -Wextra \
       -Werror \
       -nostdlib -nostdinc -fno-builtin -nostartfiles -nodefaultlibs
else
$(error "Unknown build mode")
endif

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

.PHONY: all clean run debug
all: $(EXECUTABLE)

#linking stage. All objects required to be compiled.
#Linking info required as well

$(EXECUTABLE) : $(BUILD_DIR) $(OBJECTS) 
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE) -lgcc

#Make sure our build and output directories are created
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p boot/

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
	./run.sh $(QEMUOPT)

debug : all
	./debug.sh $(QEMUOPT)
