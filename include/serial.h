#ifndef _SERIAL_H
#define _SERIAL_H

#include <utils.h>

#define SERIAL_PORT 0x3f8

static inline u8 inb(int portnum) {
    u8 data = 0;
    __asm__ __volatile__ ("inb %%dx, %%al" : "=a" (data) : "d" (portnum));
    return data;
}
  
static inline void outb(int portnum, u8 data) {
    __asm__ __volatile__ ("outb %%al, %%dx" :: "a" (data),"d" (portnum));
}
  
int init_serial();
void serial_write(char *msg);
void serial_writeln(char *msg);

#endif