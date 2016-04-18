#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

uint8_t make_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
uint16_t make_vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}
 
void terminal_initialize() {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}
 
void terminal_putchar(char c) {
	// Handle a backspace, by moving the cursor back one space
	if (c == 0x08 && terminal_column){
	    terminal_column--;
	}
	
	// Handle a tab by increasing the cursor's X, but only to a point
	// where it is divisible by 8.
	else if (c == 0x09){
	    terminal_column = (terminal_column+8) & ~(8-1);
	}
	
	// Handle carriage return
	else if (c == '\r'){
	    terminal_column = 0;
	}
	
	// Handle newline by moving cursor back to left and increasing the row
	else if (c == '\n'){
		terminal_column = 0;
		terminal_row++;
	}
	// Handle any other printable character.
	else if(c >= ' '){
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT) {
 				terminal_row = 0;
			}
		}	
	}
	
	if (terminal_row == VGA_HEIGHT){
		terminal_scroll(1);
	}
}
 
void terminal_scroll(size_t n){
	//todo
}

void terminal_clear(){
	terminal_initialize();
}

void terminal_writestring(const char* data) {
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++)
		terminal_putchar(data[i]);
}

