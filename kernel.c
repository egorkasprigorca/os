typedef unsigned int size_t;
typedef signed int ssize_t;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

static u16* vga_buf = (u16*) 0xb8000;

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static u8 terminal_color = 0;
static u8 text_color = 15;

u16 new_vga_entry(char c, u8 color) {
	u16 entry = c;
	u16 entry_color = color;
	return entry | entry_color << 8;
}

void clear_screen() {
	for (int i = 0; i < VGA_HEIGHT * VGA_WIDTH * 2; i++) {
		vga_buf[i] = new_vga_entry(' ', terminal_color);
	}
}

kernel_main() {
	clear_screen();
	char *msg = "Hello World";
	while (*msg != '\0') {
		*vga_buf++ = new_vga_entry(*msg++,  text_color);
	}
}
