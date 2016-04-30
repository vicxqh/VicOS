#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>

void kernel_early(void)
{
	terminal_initialize();
    init_gdt();
    printf("GDT Loaded!\n");
    init_idt();
    printf("IDT Loaded!\n");
}

void kernel_main(void)
{
	printf("Hello, kernel World!\n");
    register_i_handler(3, default_i_handler);
    printf("Sending int 3...\n");
    asm volatile ("int $0x3");
    printf("Sending int 4...\n");
    asm volatile ("int $0x4");
    unregister_i_handler(3);
    printf("Sending int 3...\n");
    asm volatile ("int $0x3");
}
