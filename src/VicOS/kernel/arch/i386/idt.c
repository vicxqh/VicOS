#include <kernel/idt.h>
#include <string.h>
#include <stdio.h>
#include <kernel/cpu.h>


static idt_entry idt_entrys[256];
static idt_descriptor idtd;
static i_handler i_handlers[256];

extern void _flush_idt(uint32_t idtd);

static idt_entry create_entry(uint32_t addr, uint16_t selector, uint8_t flag);
static void remap_pic();

//Disable LAPIC if there is one
static void disable_local_apic();


// For more explanation of the following inline method 
//  see: http://wiki.osdev.org/Inline_Assembly/Examples#I.2FO_access
static inline void outb(uint16_t port, uint8_t val){
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port)  );
}
static inline uint8_t inb(uint16_t port){
    uint8_t ret;
    asm volatile ( "inb %1, %0" 
            : "=a"(ret)
            : "Nd"(port) );

    return ret;
}
static inline void io_wait(){
    asm volatile ( "outb %%al, $0x80" : : "a"(0)  );
}

void init_idt(){
    printf("Loading IDT ...\n");

    disable_local_apic();

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
    remap_pic();
    
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
    idt_entrys[255]= create_entry((uint32_t)isr255, 0x8, KERNEL_32_BIT_INT_GATE);

    //FOR PIC ISR
    idt_entrys[IRQ0] = create_entry((uint32_t)irq0, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ1] = create_entry((uint32_t)irq1, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ2] = create_entry((uint32_t)irq2, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ3] = create_entry((uint32_t)irq3, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ4] = create_entry((uint32_t)irq4, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ5] = create_entry((uint32_t)irq5, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ6] = create_entry((uint32_t)irq6, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ7] = create_entry((uint32_t)irq7, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ8] = create_entry((uint32_t)irq8, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ9] = create_entry((uint32_t)irq9, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ10] = create_entry((uint32_t)irq10, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ11] = create_entry((uint32_t)irq11, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ12] = create_entry((uint32_t)irq12, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ13] = create_entry((uint32_t)irq13, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ14] = create_entry((uint32_t)irq14, 0x8, KERNEL_32_BIT_INT_GATE);
    idt_entrys[IRQ15] = create_entry((uint32_t)irq15, 0x8, KERNEL_32_BIT_INT_GATE);
    //Call asm to flush idt
    _flush_idt((uint32_t)&idtd);
//    asm volatile ("int $0x20");

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
    if (!i_handlers[regs->int_no]){
        printf("No handler for interrupt 0x%x\n", regs->int_no);
    } else {
        i_handlers[regs->int_no](regs);
    }
}

void irq_handler(registers_t *regs){
    printf("Received irq : %d\n", regs->err_code);
    
    isr_dispatcher(regs);
    
    //Send EOI to PIC
    if (regs->err_code >= 8){ //Also send EOI to slave chip
        outb(PIC2_COMMAND,PIC_EOI);
    }
    outb(PIC1_COMMAND,PIC_EOI);

}


void register_i_handler(int num, i_handler h){
    if (num < 0 || num > 255){
        printf("Cannot register interrupt handler for index %d", num);
        return;
    }   
    i_handlers[num] = h;
}

void default_i_handler(registers_t *regs){
    printf("Default interrupt handler recevice interrupt 0x%x, but doing nothing\n", regs->int_no);
}

void unregister_i_handler(int num){
    memset(i_handlers + num, 0, sizeof(i_handler)); 
}

static void remap_pic(){
    unsigned char a1, a2;
    
    // save masks
    a1 = inb(PIC1_DATA);
   a2 = inb(PIC2_DATA);

    //reinitialize pic
    outb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);    //starts the initialization sequence (in cascade mode)
    io_wait();  //on older machines its necessary to give the PIC some time to react to commands as they might not be processed quickly
    outb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, ICW2_PIC1);     //ICW2: Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, ICW2_PIC2);   //ICW2: Slave PIC vector offset
    io_wait();
    outb(PIC1_DATA, 4);         //ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(PIC2_DATA, 2);     //ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, 1);

    // Restore masks
 //   outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

extern void _test();

static void disable_local_apic(){
    uint32_t eax;
    uint32_t edx;
    
    cpuid(1, &eax, &edx);

    if (edx & CPUID_FLAG_APIC ){
        printf("Deteced APIC, will disable it.\n");
        if (edx & CPUID_FLAG_MSR){
            _test();
        } else {
            printf("No MSR detected!\n");
        }
    }
}
