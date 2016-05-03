#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h>
// Each define here is for a specific flag bit(s) for a idt entry
#define FLAG_PRESENT(x) ((x) << 0x07)   //Set to 0 for unused interrupts.
#define FLAG_DPL(x) ((x & 0x3) << 0x5)  //Specifies which privilege Level the calling Descriptor minimum should have
#define FLAG_STRSGMT(x) ((x) << 0x4)    //Storage Segment. Set to 0 for interrupt gates 

//define gate type here
#define GATE_32_TASK    0x5 //80386 32 bit task gate
#define GATE_16_INT 0x6 //80286 16-bit interrupt gate
#define GATE_16_TRAP    0x7 //80286 16-bit trap gate
#define GATE_32_INT 0xe //80386 32-bit interrupt gate
#define GATE_32_TRAP    0xf //80386 32-bit trap gate

//Used for our idt
#define KERNEL_32_BIT_INT_GATE (FLAG_PRESENT(1) | FLAG_DPL(0) | FLAG_STRSGMT(0) | GATE_32_INT)

//Fllowing mapping relationships are for 8259 pic
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

//Ports for PIC
#define PIC1 0x20
#define PIC2 0xa0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

// End Of Interrupt
#define PIC_EOI 0x20

// For reinitialization PIC
#define ICW1_ICW4 0X1   //ICW4 (not) needed
#define ICW1_SINGLE 0x2 //Single (cascade) mode
#define ICW1_INTERVAL4 0x4  //Call address interval 4 (8)
#define ICW1_LEVEL 0x8  //Level triggered (edge) mode
#define ICW1_INIT 0x10  //Initialization - required! 

#define ICW2_PIC1 IRQ0  //ICW2: Master PIC vector offset
#define ICW2_PIC2 IRQ8  //ICW2: Slave PIC vector offset

#define ICW4_8086 0x1   //8086/88 (MCS-80/85) mode
#define ICW4_AUTO 0x2   //Auto (normal) EOI
#define ICW4_BUF_SLAVE 0x8  //Buffered mode/slave
#define ICW4_BUF_MASTER 0xc //Buffered mode/master
#define ICW4_SFNM 0x10  //Special fully nested (not)


typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_descriptor;

typedef struct {
    uint32_t ds; //Saved data segment selector
    uint32_t edi, esi, ebp, useless_esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code;    // Interrupt number and error code 
    uint32_t eip, cs, eflags, esp, ss; // Pushed by the processor automatically.
} registers_t;

//Dispath an interrupt to a i_handler
void isr_dispatcher(registers_t *regs);

//The union entry for pic irq
void irq_handler(registers_t *regs);

//An interrupt handler. It is a pointer to a function which takes a pointer 
//to a structure containing register values.
typedef void (*i_handler)(registers_t *);

//Allows us to register an interrupt handler.
void register_i_handler(int num, i_handler h);

void unregister_i_handler(int num);

void default_i_handler(registers_t *regs);

typedef uint64_t idt_entry;

void init_idt();

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr255();

//Routines for IQR
extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif
