#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

static int hook_id;

int timer_set_square(unsigned long timer, unsigned long freq)
{

	unsigned short divide;
	unsigned char LSBtimer, MSBtimer;
	if (timer > 2 || freq == 0)
		return 1;

	divide = TIMER_FREQ / freq;

	if (sys_outb(TIMER_CTRL,
			TIMER_CTRL_SEL(timer) | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN)
			!= OK || sys_outb(TIMER_0 + timer, divide) != OK
			|| sys_outb(TIMER_0 + timer, divide >> 8) != OK)
		return 1;

	return 0;
}

int timer_subscribe_int(void)
{
	hook_id = TIMER0_HOOK_BIT;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK
			|| sys_irqenable(&hook_id) != OK)
		return 0;

	return 1 << TIMER0_HOOK_BIT;
}

int timer_unsubscribe_int()
{
	if (sys_irqrmpolicy(&hook_id) != OK)
		return 1;
	return 0;
}

void timer_int_handler()
{
	int ipc_status;
	message msg;
	unsigned long i = 1;

	while(i < 10) {

		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d", i);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & IRQ0) {
					printf("Notification of interruption %lu received. \n", i);
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

int timer_test_square(unsigned long freq)
{
	return timer_set_square(0,freq);
}


int timer_test_int(unsigned long time)
{
	timer_subscribe_int();
	timer_set_square(0, 60);
	unsigned int timer_irq_set;
	timer_irq_set = timer_subscribe_int();
	int ipc_status;
	message msg;
	unsigned long i = 1;

	while (i <= time * 60)
	{

		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d", i);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & timer_irq_set) {
					printf("Notification of interruption %lu received. \n", i);
				}
				break;
			default:
				printf("%lu - Notification not expected.\n", i);
				break;
			}
		} else {
			printf("%lu - Received a standard message.\n", i);
		}
		i++;
	}
	timer_unsubscribe_int();
}
