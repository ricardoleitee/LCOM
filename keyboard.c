#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "timer.h"

#define DELAY_US    20000

unsigned char scancode;
static int hook_id = 0x00;

int keyboard_subscribe_int(void) {
	hook_id = KBC_IRQ;
	printf("Subscribe");
	if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK
			|| sys_irqenable(&hook_id) != OK) {
		printf("Erro!");
		return -1;
	}

	return 1 << KBC_IRQ;
}

int keyboard_unsubscribe_int() {
	if (sys_irqrmpolicy(&hook_id) != OK) {
		printf("Erro!");
		return 1;
	}

	return 0;
}

int keyboard_int_handler(int irq_set) {
	int ipc_status;
	message msg;
	unsigned long stat, data;
	unsigned long i = 0;

	while (1) {
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d", i);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					sys_inb(STAT_REG, &stat);
					if ((stat & IBF) == 0) {
						if ((stat & (PAR_ERR | TO_ERR)) == 0) {
							sys_inb(KBC_OUT_REG, &data);
							scancode = data;
							return 0;
						}
					}
					tickdelay(micros_to_ticks(DELAY_US));

				}
				break;
			default:
				printf("%lu - Notification not expected.\n", i);
				break;
			}
		} else {
			printf("%lu - Received a standard message.\n", i);
		}
	}
}

int test_scan(void) {
	/* To be completed */
	int irq_set;

	if ((irq_set = keyboard_subscribe_int()) == -1) {
		return 1;
	}

	do {
		if (keyboard_int_handler(irq_set) == 0) {
			if (IS_BREAKCODE(scancode)) {
				printf("Breakcode: ");
			} else {
				printf("Makecode: ");
			}
			printf("0x%X\n", scancode);
		}
	} while (scancode != 0x81);

	if (keyboard_unsubscribe_int() != 0) {
		return 1;
	}
	return 0;
}

int kbc_write(unsigned char in_buf, unsigned short cmd) {
	unsigned long stat;

	while (1) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 input buffer is not empty */
		if ((stat & IBF) == 0) {
			sys_outb(in_buf, cmd); /* no args command */
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return 1;
}

int kbc_read() {
	unsigned long stat, data;

	while (1) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 output buffer is empty */
		if ((stat & IBF) == 0) {
			sys_inb(KBC_OUT_REG, &data); /* assuming it returns OK */
			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return 0;
}

void wait_seconds(unsigned long time) {
	int ipc_status;
	message msg;
	unsigned int i=0;
	unsigned int irq_set;

	irq_set = timer_subscribe_int();

	while(i < time*60) {
		/* Get a request message. */
		if ( driver_receive(ANY, &msg, &ipc_status) == 0 ) {
			if (is_ipc_notify(ipc_status)) { /* received notification */
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
						i++;
					}
					break;
				default:
					break;
				}
			}
		}
	}

	timer_unsubscribe_int();
}
