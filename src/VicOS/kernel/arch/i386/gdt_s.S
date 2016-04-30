#function for loading gdt
.global _flush_gdt
.type _flush_gdt, @function
_flush_gdt:
    movl 4(%esp), %eax #Get the first argument, which is the pointer to gdt descriptor

    lgdt (%eax) #load gdt descriptor

    movw $0x10, %ax #0x10 is the offset in the GDT to our data segment
    mov %ax, %ds #copy %ax to ds,es,fs,gs,ss
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    ljmp $0x08,$flush_lable #Using a long jump to set %cs to 0x8

flush_lable:
    ret
.size _flush_gdt, . - _flush_gdt

