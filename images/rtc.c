#include "rtc.h"
#include <stdio.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <minix/com.h>

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_REG_A 0x0A
#define RTC_REG_B 0x0B
#define RTC_REG_C 0x0C
#define RTC_REG_D 0x0D
#define RTC_IRQF 8
#define RTC_HOOK_BIT 2
#define RTC_HOUR 0x04
#define RTC_MIN 0x02
#define RTC_SEC 0x00
#define RTC_DAY 0x07
#define RTC_MONTH 0x08
#define RTC_YEAR 0x09
#define BIT(n) (0x01<<(n))

#define MAX_ATTEMPTS 4

static int hook_id;

int rtc_subscribe() {
	hook_id = RTC_HOOK_BIT;

	if (sys_irqsetpolicy(RTC_IRQF, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK
			|| sys_irqenable(&hook_id) != OK)
		return 0;

	return 1 << RTC_HOOK_BIT;
}

int rtc_unsubscribe() {
	if(sys_irqrmpolicy(&hook_id) != OK)
		return 1;

	return 0;
}

int test_conf(void) {
	unsigned int i = 0;
	unsigned long regA = 0, regB = 0, regC = 0, regD = 0;


	//Read Register A
	sys_outb(RTC_ADDR_REG, RTC_REG_A);
	sys_inb(RTC_DATA_REG, &regA);

	printf("REG A: %X\n", regA);

	//Read Register B
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_inb(RTC_DATA_REG, &regB);

	printf("REG B: %X\n", regB);

	//Read Register C
	sys_outb(RTC_ADDR_REG, RTC_REG_C);
	sys_inb(RTC_DATA_REG, &regC);

	printf("REG C: %X\n", regC);

	//Read Register D
	sys_outb(RTC_ADDR_REG, RTC_REG_D);
	sys_inb(RTC_DATA_REG, &regD);

	printf("REG D: %X\n", regD);
}

int test_date(void) {
	unsigned long regHour=0, regMin=0, regSec=0, regDay=0, regMonth=0, regYear=0, regB=0;
	int irq_set_rtc, ipc_status;
	message msg;
	unsigned long r=0;

	if ((irq_set_rtc = rtc_subscribe()) == 0)
		return 1;

	//Read Register B
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_inb(RTC_DATA_REG, &regB);

	//Write Register B
	regB |= BIT(4);
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_outb(RTC_DATA_REG, regB);

	int i = 0;


	while (i!=1) {
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_rtc) {
					//Read Hour
					sys_outb(RTC_ADDR_REG, RTC_HOUR);
					sys_inb(RTC_DATA_REG, &regHour);
					printf("REG HOUR: %X\n", regHour);

					//Read Minute
					sys_outb(RTC_ADDR_REG, RTC_MIN);
					sys_inb(RTC_DATA_REG, &regMin);
					printf("REG MIN: %X\n", regMin);

					//Read Second
					sys_outb(RTC_ADDR_REG, RTC_SEC);
					sys_inb(RTC_DATA_REG, &regSec);
					printf("REG SEC: %X\n", regSec);

					//Read Day
					sys_outb(RTC_ADDR_REG, RTC_DAY);
					sys_inb(RTC_DATA_REG, &regDay);
					printf("REG DAY: %X\n", regDay);

					//Read Month
					sys_outb(RTC_ADDR_REG, RTC_MONTH);
					sys_inb(RTC_DATA_REG, &regMonth);
					printf("REG MONTH: %X\n", regMonth);

					//Read Year
					sys_outb(RTC_ADDR_REG, RTC_YEAR);
					sys_inb(RTC_DATA_REG, &regYear);
					printf("REG YEAR: %X\n", regYear);

					return 0;
				}
				break;
			default:

				break;
			}
		}
	}

	//Read Register B
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_inb(RTC_DATA_REG, &regB);

	//Write Register B
	regB |= ~BIT(4);
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_outb(RTC_DATA_REG, regB);

	rtc_unsubscribe();

	return 0;
}

int test_int(/* to be defined in class */) {
	/* To be completed */
}
