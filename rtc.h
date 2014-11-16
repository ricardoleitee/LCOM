#ifndef __RTC_H
#define __RTC_H

unsigned long getDate(unsigned long *day, unsigned long *month, unsigned long *year);
int bcd2binary(int bcd_num);

#endif /* __RTC_H */
