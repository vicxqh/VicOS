#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

static void terminal_scroll(){
    //todo
}
void terminal_putchar(char c)
{
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
		terminal_scroll();
	}

}

void terminal_write(const char* data, size_t size)
{
	for ( size_t i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}
