# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler 
# rule, as we use nasm instead of GNU as.

SOURCES=boot.o kernel.o vga.o util.o

CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-Tlink.ld

all: $(SOURCES) link 

clean:
	rm *.o kernel

link:
	i686-elf-gcc $(LDFLAGS) -ffreestanding -O2 -nostdlib -o kernel $(SOURCES)

boot.o: boot.s
	i686-elf-as boot.s -o boot.o	
