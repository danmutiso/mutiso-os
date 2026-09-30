#ifndef PIC_H
#define PIC_H

#define PICM_COMMAND 0x20
#define PICM_DATA 0x21

#define PICS_COMMAND 0xA0
#define PICS_DATA 0xA1

#define PICM_OFFSET 0x20
#define PICS_OFFSET 0x28

#define PIC_INITIALIZE 0x11
#define PIC_MODE 0x01

#define PIC_EOI 0x20

void pic_remap(void);
void pic_send_eoi(unsigned char irq);

#endif
