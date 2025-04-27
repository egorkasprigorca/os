#include <serial.h>
  
int init_serial() {
    outb(SERIAL_PORT + 1, 0x00);    // Disable all interrupts
    outb(SERIAL_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(SERIAL_PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(SERIAL_PORT + 1, 0x00);    //                  (hi byte)
    outb(SERIAL_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(SERIAL_PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(SERIAL_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(SERIAL_PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(SERIAL_PORT + 0, 0xAE);    // Send a test byte

    if(inb(SERIAL_PORT + 0) != 0xAE) {
        return 1;
    }

    outb(SERIAL_PORT + 4, 0x0F);
    return 0;
}

void serial_write(char *msg) {
    while (*msg != '\0') {
        outb(SERIAL_PORT, *msg++);
    }
}

void serial_writeln(char *msg) {
	serial_write(msg);
	outb(SERIAL_PORT, '\n');
}