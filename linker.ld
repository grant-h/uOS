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
        sbss = .; /* sbss and ebss allow us to easily zero the bss */
        *(.bss)
        *(COMMON)
        ebss = .;
    }

    /* Let the source know about the kernel end */
    _KERNEL_END = .;
}
