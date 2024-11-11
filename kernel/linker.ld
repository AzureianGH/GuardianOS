/* Tell the linker that we want an x86_64 ELF64 output file */
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

/* We want the symbol _start to be our entry point */
ENTRY(_start)

/* Define the program headers we want so the bootloader gives us the right */
/* MMU permissions */
PHDRS
{
    requests PT_LOAD    FLAGS((1 << 1) | (1 << 2)) ; /* Write + Read */
    text     PT_LOAD    FLAGS((1 << 0) | (1 << 2)) ; /* Execute + Read */
    rodata   PT_LOAD    FLAGS((1 << 2)) ;            /* Read only */
    data     PT_LOAD    FLAGS((1 << 1) | (1 << 2)) ; /* Write + Read */
    dynamic  PT_DYNAMIC FLAGS((1 << 1) | (1 << 2)) ; /* Dynamic PHDR for relocations */
}

SECTIONS
{
    /* We want to place the kernel in the topmost 2GiB of the address space */
    . = 0xffffffff80000000;

    /* Define kernel start symbol */
    kernel_start = .;

    /* Define a section to contain the Limine requests and assign it to its own PHDR */
    .requests : {
        KEEP(*(.requests_start_marker))
        KEEP(*(.requests))
        KEEP(*(.requests_end_marker))
    } :requests

    /* Move to the next memory page for .text */
    . += CONSTANT(MAXPAGESIZE);

    .text : {
        *(.text .text.*)
    } :text

    /* Move to the next memory page for .rodata */
    . += CONSTANT(MAXPAGESIZE);

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    /* Define init_array section for global constructors */
    .init_array : {
        __init_array = .;
        *(.init_array .init_array.*)
        __init_array_end = .;
    } :rodata

    /* Move to the next memory page for .data */
    . += CONSTANT(MAXPAGESIZE);

    .data : {
        *(.data .data.*)
    } :data

    /* Dynamic section for relocations */
    .dynamic : {
        *(.dynamic)
    } :data :dynamic

    /* BSS section (uninitialized data) */
    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } :data

    /* Define kernel end symbol */
    kernel_end = .;

    /* Discard .note and .eh_frame sections */
    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
    }
}
