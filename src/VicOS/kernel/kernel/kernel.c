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
//    while(1);
 //   while(1);
    asm volatile ("int $0x3");
}
