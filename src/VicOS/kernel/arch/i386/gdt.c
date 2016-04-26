#include <kernel/gdt.h>

gdt_entry entrys[5];
gdt_descriptor gdtd;

extern void _flush_gdt(uint32_t gdtp);

static gdt_entry create_entry(uint32_t base, uint32_t limit, uint16_t flag){
  uint64_t entry ;
  // Create the high 32 bit segment
  entry =  limit       & 0x000F0000;         // set limit bits 19:16
  entry |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
  entry |= (base >> 16) & 0x000000FF;         // set base bits 23:16
  entry |=  base        & 0xFF000000;        // set base bits 31:24 

  // Shift by 32 to allow for low part of segment
  entry <<= 32;

  // Create the low 32 bit segment
  entry |= base  << 16;                       // set base bits 15:0
  entry |= limit  & 0x0000FFFF;               // set limit bits 15:0

  return entry;
}

void init_gdt(){
    gdtd.limit = (sizeof(gdt_entry) * 5) - 1;
    gdtd.base = (uint32_t)entrys;

    //Fill data to gdt entries
    entrys[0] = create_entry(0, 0, 0); //Needed
    entrys[1] = create_entry(0, 0x000FFFFF, (GDT_CODE_PL0));   // Ring 0 code section
    entrys[2] = create_entry(0, 0x000FFFFF, (GDT_DATA_PL0));   // Ring 0 data section
    entrys[3] = create_entry(0, 0x000FFFFF, (GDT_CODE_PL3));   // Ring 3 code section
    entrys[4] = create_entry(0, 0x000FFFFF, (GDT_DATA_PL3));   // Ring 3 data section

    _flush_gdt((uint32_t)&gdtd);

}
