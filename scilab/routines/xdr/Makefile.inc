

CC= gcc 
CFLAGS = -g -DWIN32 -Dgcwin32 -DREADLINE

SRCS =	xdr.c xdr_array.c xdr_float.c xdr_mem.c \
	xdr_rec.c xdr_reference.c xdr_stdio.c

OBJS =	xdr.o xdr_array.o xdr_float.o xdr_mem.o \
	xdr_rec.o xdr_reference.o xdr_stdio.o

all	: $(OBJS) 
