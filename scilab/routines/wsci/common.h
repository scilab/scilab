/* common.h */

/*
 * Copyright Enpc (Jean-Philippe Chancelier)
 */

#ifndef WSCI_COMMON
#define WSCI_COMMON

#include "../machine.h"
#include <stdarg.h>

/* wmenu.c - Menu */

extern void sciprint(char *fmt, ...);
extern void sciprint_nd(char *fmt, ...);
extern int sciprint2(int v,char *fmt,...);
extern void WinExit(void);
extern void Scistring(char *str);
extern int C2F(clearexit)(integer *n);
extern int ReadKey(void);
int WriteStr(char *s, FILE *file);
int WriteKey(char c, FILE *file);

/* winmain */ 

int InteractiveMode(void);

#endif 
