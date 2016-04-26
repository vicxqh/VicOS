#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>

void kernel_early(void)
{
	terminal_initialize();
    init_gdt();
    printf("GDT\n");
}

void kernel_main(void)
{
	printf("Hello, kernel World!\n");
}
