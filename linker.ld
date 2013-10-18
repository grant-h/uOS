ENTRY (_start)

SECTIONS
{
    /* Base address starting at 1MB */
    . = 0x00100000;

    /* Let the source know about the kernel start */
    _KERNEL_START = .;

    /* Special multiboot compliant header */
    .__mbHeader : { *(.__mbHeader) }
    /* make sure the sections are page aligned */
    .text ALIGN (0x1000) : { *(.text) }
    .rodata ALIGN (0x1000) : { *(.rodata*) }
    .data ALIGN (0x1000) : { *(.data) }
    .bss ALIGN (0x1000) :
    {
        _BSS_START = .; /* sbss and ebss allow us to easily zero the bss */
        *(.bss)
        *(COMMON)
        _BSS_END = .;
    }

    /* Let the source know about the kernel end */
    _KERNEL_END = .;
    _BSS_SIZE    = _BSS_END    - _BSS_START;
    _KERNEL_SIZE = _KERNEL_END - _KERNEL_START;
}

ASSERT(_KERNEL_SIZE < 0x300000, "Kernel exceeds the 3 MB limit. asm/loader.s must be updated to work with a >3MB kernel");

