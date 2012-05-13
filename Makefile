CC=i686-pc-linux-gnu-gcc
LD=i686-pc-linux-gnu-ld
ASM=nasm

LINKING_INFO=linker.ld

ASMFLAGS=-f elf
LDFLAGS=-T $(LINKING_INFO) 
CFLAGS=-I$(CURDIR) -Wall -Wextra -nostdlib -nostdinc -fno-builtin -nostartfiles -nodefaultlibs

CSRC=kernel.o kprint.o assembly.o string.o gdt.o idt.o isr.o kerror.o
ASRC=loader.o gdt_x86.o idt_x86.o
SOURCES=$(CSRC) $(ASRC)
OBJECTS=$(SOURCES)
#OBJECTS=$(CSRC:.c=.o) $(ASRC:.s=.o)
EXECUTABLE=kernel.bin


.PHONY: all
all: $(EXECUTABLE)

#linking stage. All objects required to be compiled.
#Linking info required as well
$(EXECUTABLE): $(SOURCES)
	$(LD) $(LDFLAGS) $(SOURCES) -o $(EXECUTABLE)

#Compiling C and Assembly files
#All C/Asm files required to be present
#A check for headers can be added later
.s.o:
	$(ASM) $(ASMFLAGS) $< 
.c.o:
	$(CC) $(CFLAGS) -c $< 

#BE CAREFUL!
.PHONY: clean
clean: 
	-rm -f $(OBJECTS) $(EXECUTABLE)
