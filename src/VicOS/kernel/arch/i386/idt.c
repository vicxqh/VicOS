#include <kernel/idt.h>
#include <string.h>
#include <stdio.h>

static idt_entry idt_entrys[256];
static idt_descriptor idtd;
static i_handler i_handlers[256];

extern void _flush_idt(uint32_t idtd);

static idt_entry create_entry(uint32_t addr, uint16_t selector, uint8_t flag);

void init_idt(){
    printf("Loading IDT ...\n");

    // Clear all entrys 
    memset(idt_entrys, 0, sizeof(idt_entry) * 256);
    // Clear all hanlders
    memset(i_handlers, 0, sizeof(i_handler) * 256);

    idtd.limit= sizeof(idt_entry) * 256 - 1;
    idtd.base = (uint32_t) idt_entrys;

    printf("\tIDT DESCRIPTOR ADDRESS: %x\n", &idtd);
    printf("\tLimit: %x\n", idtd.limit);
    printf("\tLase: %x\n", idtd.base);

    //Should remap pic

    idt_entrys[0]= create_entry((uint32_t)isr0, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[1]= create_entry((uint32_t)isr1, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[2]= create_entry((uint32_t)isr2, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[3]= create_entry((uint32_t)isr3, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[4]= create_entry((uint32_t)isr4, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[5]= create_entry((uint32_t)isr5, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[6]= create_entry((uint32_t)isr6, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[7]= create_entry((uint32_t)isr7, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[8]= create_entry((uint32_t)isr8, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[9]= create_entry((uint32_t)isr9, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[10]= create_entry((uint32_t)isr10, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[11]= create_entry((uint32_t)isr11, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[12]= create_entry((uint32_t)isr12, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[13]= create_entry((uint32_t)isr13, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[14]= create_entry((uint32_t)isr14, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[15]= create_entry((uint32_t)isr15, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[16]= create_entry((uint32_t)isr16, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[17]= create_entry((uint32_t)isr17, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[18]= create_entry((uint32_t)isr18, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[19]= create_entry((uint32_t)isr19, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[20]= create_entry((uint32_t)isr20, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[21]= create_entry((uint32_t)isr21, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[22]= create_entry((uint32_t)isr22, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[23]= create_entry((uint32_t)isr23, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[24]= create_entry((uint32_t)isr24, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[25]= create_entry((uint32_t)isr25, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[26]= create_entry((uint32_t)isr26, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[27]= create_entry((uint32_t)isr27, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[28]= create_entry((uint32_t)isr28, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[29]= create_entry((uint32_t)isr29, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[30]= create_entry((uint32_t)isr30, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[31]= create_entry((uint32_t)isr31, 0x8, KERNEL_32_BIT_INT_GATE);

    //Call asm to flush idt
    _flush_idt((uint32_t)&idtd);

}

static idt_entry create_entry(uint32_t addr, uint16_t selector, uint8_t flag){
    uint64_t entry = 0;
    entry |= (addr & 0xFFFF0000);
    entry >>= 8;
    entry |= flag;
    entry <<= 8;
    entry <<=16;
    entry |= selector;
    entry <<= 16;
    entry |= (addr & 0xFFFF);
    return entry;
}

void isr_dispatcher(registers_t *regs){
    printf("Received interrupt : %d\n", regs->int_no);

}

void register_i_handler(int num, i_handler h){
    if (num < 0 || num > 255){
        printf("Cannot register interrupt handler for index %d", num);
        return;
    }   
    //TODO
}
