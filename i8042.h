#ifndef __I8042_H
#define __I8042_H


#define BIT(n) (0x01<<n)

#define KBC_IRQ	 1


#define KBC_OUT_REG 0x60
#define KBC_IN_BUF 0x60
#define STAT_REG 0x64
#define KBC_CMD_REG 0x64


#define CKBD_ACK 0xfa
#define CKBD_RESEND 0xfe
#define CKBD_ERROR 0xfc

#define KBD_TOGGLE_LEDS 0xED

#define KBD_LED(n) (1<<n)

#define KBD_WAIT_TIME 20000


#define OBF BIT(0)
#define IBF BIT(1)
#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)


#define IS_BREAKCODE(c) (c>>7)


#endif /* __I8042_H */
