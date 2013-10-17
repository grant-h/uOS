global _start                           ; make the entry point visible to linker
extern kmain                            ; kmain is defined in kernel.cpp
extern ebss
extern sbss

PAGE_SIZE  equ 0x1000
STACK_SIZE equ 4*PAGE_SIZE ; initial kernel stack space size of 16k
;BSS_SIZE   equ (ebss-sbss) 

; constants for setting up the Multiboot header
MODULEALIGN equ  1<<0                   ; align loaded modules on page boundaries
MEMINFO     equ  1<<1                   ; provide memory map (BIOS e820)
FLAGS       equ  MODULEALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC       equ  0x1BADB002             ; magic number lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum required

section .__mbHeader ; explicitly give multiboot header a section

align 4
  dd MAGIC
  dd FLAGS
  dd CHECKSUM

section .text

_start:
  ; save our multiboot information from grub before messing with registers
  ; these can't be saved on the stack as we are about to zero it
  mov [multiboot_magic], eax
  mov [multiboot_info],  ebx

  ; zero the BSS to start things off well
zerobss:
  ; TODO: ecx = BSS_SIZE (there is a better way to do this)
  mov ecx, ebss
  sub ecx, sbss
  xor al, al
  mov edi, sbss         ; start of the BSS
  rep stosb

  mov  esp, stack + STACK_SIZE        ; set up the stack
  push dword [multiboot_magic]        ; Multiboot magic number
  push dword [multiboot_info]         ; Multiboot info structure

  call kmain                          ; call kernel entry point 

  catchfire:
  hlt                                 ; halt machine if the kernel returns
  jmp short catchfire

section .data

multiboot_magic:
  resd 1
multiboot_info:
  resd 1

section .bss

align PAGE_SIZE      ; page align the page directory (is this the right way?)
page_directory:
  resd 1024          ; reserve 1024 DWORDs for our page table pointers

align 4
stack:
  resb STACK_SIZE    ; reserve 16k stack on a DWORD boundary
