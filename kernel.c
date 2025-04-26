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

void vga_write(char *msg) {
	while (*msg != '\0') {
		*vga_buf++ = new_vga_entry(*msg++,  text_color);
	}
}

extern inline unsigned char inb (int portnum)
{
  unsigned char data=0;
  __asm__ __volatile__ ("inb %%dx, %%al" : "=a" (data) : "d" (portnum));
  return data;
}

extern inline void outb (int portnum, unsigned char data)
{
  __asm__ __volatile__ ("outb %%al, %%dx" :: "a" (data),"d" (portnum));
}

#define PORT 0x3f8          // COM1

static int init_serial() {
	outb(PORT + 1, 0x00);    // Disable all interrupts
	outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(PORT + 1, 0x00);    //                  (hi byte)
	outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
	outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
	outb(PORT + 0, 0xAE);    // Send a test byte

	// Check that we received the same test byte we sent
   	if(inb(PORT + 0) != 0xAE) {
      return 1;
   	}

	// If serial is not faulty set it in normal operation mode:
	// not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled
	outb(PORT + 4, 0x0F);
	return 0;
}

void serial_write(char *msg) {
	while (*msg != '\0') {
		outb(PORT, *msg++);
	}
}

kernel_main() {
	char *msg = "Hello World.";
	clear_screen();
	vga_write(msg);
	init_serial();
	serial_write(msg);
}