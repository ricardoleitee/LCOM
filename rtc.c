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

unsigned long getDate(unsigned long *day, unsigned long *month, unsigned long *year)
{
	//Read Day
	sys_outb(RTC_ADDR_REG, RTC_DAY);
	sys_inb(RTC_DATA_REG, day);

	//Read Month
	sys_outb(RTC_ADDR_REG, RTC_MONTH);
	sys_inb(RTC_DATA_REG, month);

	//Read Year
	sys_outb(RTC_ADDR_REG, RTC_YEAR);
	sys_inb(RTC_DATA_REG, year);

	/*printf("Data: %X-%X-%X\n", *day, *month, *year);
	printf("divisao regDay %d\n", (bcd2binary(*day)) / 10);*/
}

int bcd2binary (int bcd_num)
{
	return ((( bcd_num & 0xF0 ) >> 4 )* 10 + ( bcd_num & 0x0F ));
}
