ENTRY (loader)

SECTIONS
{
    /* Base address starting at 1MB */
    . = 0x00100000;

    /* Let the C source know about the kernel start */
    _KERNEL_START = .;

    .__mbHeader : 
    {
      *(.__mbHeader)
    }

    /* make sure the sections are page aligned */
    .text ALIGN (0x1000) :
    {
        *(.text)
    }

    .rodata ALIGN (0x1000) :
    {
        *(.rodata*)
    }

    .data ALIGN (0x1000) :
    {
        *(.data)
    }

    .bss :
    {
        sbss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
    }

    /* Let the C source know about the kernel end */
    _KERNEL_END = .;
}
