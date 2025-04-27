#include <serial.h>

static u16* vga_buf = (u16*) 0xb8000;

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static u8 terminal_color = 0;
static u8 text_color = 15;

static inline u16 new_vga_entry(char c, u8 color) {
	return (u16)c | (u16)color << 8;
}

void clear_screen() {
	for (int i = 0; i < VGA_HEIGHT * VGA_WIDTH * 2; i++) {
		vga_buf[i] = new_vga_entry(' ', terminal_color);
	}
}

void vga_write(char *msg) {
	while (*msg != '\0') {
		*vga_buf++ = new_vga_entry(*msg++,  text_color);
	}
}

int kernel_main() {
	char *msg = "Hello World.";
	clear_screen();
	vga_write(msg);
	init_serial();
	serial_write(msg);
}