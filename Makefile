# Makefile for the mv driver (based on that of hello).

COMPILER_TYPE= gnu

CC=gcc

PROG=	lab2
SRCS=	lab2.c video_gr.c vbe.c game.c keyboard.c timer.c rtc.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS} liblm.a
LDADD+=	-llm -ldriver -lsys -ltimers

LDFLAGS+= -L .

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>