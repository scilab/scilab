#ifndef __DO_XXPRINTF_H__
#define __DO_XXPRINTF_H__

#include <stdio.h>

#define VPTR void * 
#define RET_END -2
#define RET_BUG -1 
#define FAIL 0
#define OK 1
#define MEM_LACK -3
#define MISMATCH -4
#define NOT_ENOUGH_ARGS -5

int do_xxprintf (char *fname, FILE *fp, char *format, int nargs, int argcnt, int lcount, char **strv);

#endif /* __DO_XXPRINTF_H__ */
