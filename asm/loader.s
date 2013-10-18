global _start                           ; make the entry point visible to linker

extern kmain                            ; kmain is defined in kernel.cpp

; used to relocate the kernel (virtually) and zero the BSS
extern _KERNEL_START
extern _KERNEL_END
extern _KERNEL_SIZE
extern _BSS_START
extern _BSS_SIZE

KERNEL_BASE equ 0xC0000000
LOWMEM_END  equ _KERNEL_END       ; lowmem ends at the 1st MB
PAGE_SIZE   equ 4096
PAGE_SHIFT  equ 12                ; 2^12 = 4096 = PAGE_SIZE
PAGE_PERM   equ 3                 ; default page permissions: present, read/write
STACK_SIZE  equ 4*PAGE_SIZE       ; initial kernel stack space size of 16k

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
  mov ecx, _BSS_SIZE
  ;mov eax, 3
  xor al, al
  mov edi, _BSS_START 
  rep stosb

  ; identity map from 0x00000000 -> KERNEL_END
  ; WARNING: code assumes that the kernel won't be greater than 3MB
  mov eax, lowmem_pt
  mov [page_directory], eax
  or dword [page_directory], PAGE_PERM ; mark the PT as present

  xor eax, eax ; eax is our starting physical address (0x00000000)
  .lowmem:
  mov ecx, eax
  shr ecx, PAGE_SHIFT     ; divide our current address by PAGE_SIZE
  and ecx, 0x3ff          ; mask of higher bits to create an index (mod 1024)
  mov [lowmem_pt+ecx*4], eax ; copy our physical address to the page entry
  or dword [lowmem_pt+ecx*4], PAGE_PERM ; write our permissions (present, etc)

  add eax, PAGE_SIZE ; move on to the next page
  cmp eax, LOWMEM_END ; are we done with lowmem? 
  jl .lowmem

  ; create virtual mappings for the kernel in the higher-half
  mov edx, KERNEL_BASE
  shr edx, 22          ; find which page table we need to use

  mov eax, kernel_pt
  mov [page_directory+edx*4], eax
  or dword [page_directory+edx*4], PAGE_PERM ; mark the PT as present

  mov eax, _KERNEL_START ; the kernel's current physical start
  .higher:
  mov ecx, eax
  shr ecx, PAGE_SHIFT
  and ecx, 0x3ff
  mov [kernel_pt+ecx*4], eax
  or dword [kernel_pt+ecx*4], PAGE_PERM

  add eax, PAGE_SIZE    ; move on to the next page
  cmp eax, _KERNEL_END  ; are we done mapping in the kernel?
  jl .higher

  mov eax, page_directory
  mov cr3, eax ; load CR3 with our page directory

  mov eax, cr0
  or eax, 0x80000000
  mov cr0, eax ; enable paging! make sure the next instruction fetch doesnt page fault

  ; adjust the stack in to the virtual area
  ; setup and adjust the stack 
  mov  esp, stack + STACK_SIZE + KERNEL_BASE

  push dword [multiboot_magic]        ; Multiboot magic number
  push dword [multiboot_info]         ; Multiboot info structure
  call kmain+KERNEL_BASE              ; call virtual kernel entry point 

  .catchfire:
  hlt                                 ; halt machine if the kernel returns (it won't)
  jmp short .catchfire

section .data

multiboot_magic:
  dd 0x0
multiboot_info:
  dd 0x0

section .bss

; NOTE THE USAGE OF ALIGNB (MUST BE USED IN BSS)
alignb 4096          ; page align the page directory and tables
page_directory:      ; should also be page aligned (hopefully)
  resd 1024          ; reserve 1024 DWORDs for our page table pointers
lowmem_pt:
  resd 1024          ; lowmem identity mappings 
kernel_pt:
  resd 1024          ; our kernel page table mappings 

align 4
stack:
  resb STACK_SIZE    ; reserve 16k stack on a DWORD boundary
