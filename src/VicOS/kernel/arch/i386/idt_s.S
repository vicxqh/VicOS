.global _flush_idt
.type _flush_idt, @function
_flush_idt:
    movl 4(%esp), %eax
    lidt (%eax)
    ret

.size _flush_idt, . - _flush_idt

.macro ISR_WITH_ERROR_CODE num
    .global isr\num
    .type isr\num, @function
    isr\num:
        cli #clear interrupt

        pushl $\num
        jmp isr_comman_stub
.endm

.macro ISR_NO_ERROR_CODE num
    .global isr\num
    .type isr\num, @function
    isr\num:
        cli #clear interrupt

        pushl $0
        pushl $\num
        jmp isr_comman_stub
.endm


ISR_NO_ERROR_CODE 0
ISR_NO_ERROR_CODE 1
ISR_NO_ERROR_CODE 2
ISR_NO_ERROR_CODE 3
ISR_NO_ERROR_CODE 4
ISR_NO_ERROR_CODE 5
ISR_NO_ERROR_CODE 6
ISR_NO_ERROR_CODE 7
ISR_WITH_ERROR_CODE 8
ISR_NO_ERROR_CODE 9
ISR_WITH_ERROR_CODE 10
ISR_WITH_ERROR_CODE 11
ISR_WITH_ERROR_CODE 12
ISR_WITH_ERROR_CODE 13
ISR_WITH_ERROR_CODE 14
ISR_NO_ERROR_CODE 15
ISR_NO_ERROR_CODE 16
ISR_WITH_ERROR_CODE 17
ISR_NO_ERROR_CODE 18
ISR_NO_ERROR_CODE 19
ISR_NO_ERROR_CODE 20
ISR_NO_ERROR_CODE 21
ISR_NO_ERROR_CODE 22
ISR_NO_ERROR_CODE 23
ISR_NO_ERROR_CODE 24
ISR_NO_ERROR_CODE 25
ISR_NO_ERROR_CODE 26
ISR_NO_ERROR_CODE 27
ISR_NO_ERROR_CODE 28
ISR_NO_ERROR_CODE 29
ISR_WITH_ERROR_CODE 30
ISR_NO_ERROR_CODE 31

isr_comman_stub:
    pusha  #Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    movw %ds, %eax
    pushl %eax  #Save ds

    mov $0x10, %ax     #Load kernel data segment descriptor
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    pushl %esp #Pointer to struct registers_t as argument

    call isr_dispatcher

    popl %esp

    popl %eax   # Restore data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    popa

    addl $8, %esp   #Cleans up the pushed error code and pushed ISR number
    iret    #pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP. **EFLAGS contais wheter we should set interrupt

