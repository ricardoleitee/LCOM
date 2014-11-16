#ifndef __KEYBOARD_H
#define __KEYBOARD_H

int keyboard_subscribe_int(void);
int keyboard_unsubscribe_int();
int test_scan(void);
int kbc_write(unsigned char in_buf, unsigned short cmd);
int kbc_read();
void wait_seconds(unsigned long time);

#endif /* __KEYBOARD_H */
