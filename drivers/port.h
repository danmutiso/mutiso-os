#ifndef PORT_H
#define PORT_H

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);
void io_wait(void);

#endif
