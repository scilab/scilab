/* Allan CORNET */
/* INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#ifndef __PRINTF_H__
#define __PRINTF_H__
/*-----------------------------------------------------------------------------------*/
#include "../machine.h"
/*-----------------------------------------------------------------------------------*/
/* replacement stdio routines that use Text Window for stdin/stdout */
/* WARNING: Do not write to stdout/stderr with functions not listed 
   in win/wtext.h */
#undef kbhit
#undef getche
#undef getch
#undef putch

#undef fgetc
#undef getchar
#undef getc
#undef fgets
#undef gets

#undef fputc
#undef putchar
#undef putc
#undef fputs
#undef puts

#undef fprintf
#undef printf
#undef vprintf
#undef vfprintf

#undef fwrite
#undef fread
/*-----------------------------------------------------------------------------------*/
int MyPutCh (int ch);
int MyGetCh (void);
char * MyFGetS (char *str, unsigned int size, FILE * file);
int MyFPutC (int ch, FILE * file);
int MyFPutS (char *str, FILE * file);
void Scistring (char *str);
void sciprint (char *fmt,...);
void sciprint_nd (char *fmt,...);
int sciprint2 (int iv, char *fmt,...);
size_t MyFWrite (const void *ptr, size_t size, size_t n, FILE * file);
size_t MyFRead (void *ptr, size_t size, size_t n, FILE * file);
void Xputchar (c);
void Xputstring (str, n);
void Scisncr (str);
void C2F(diary) __PARAMS((char *str,int *n));
void diary_nnl __PARAMS((char *str,int *n));
#endif /* __PRINTF_H__ */
/*-----------------------------------------------------------------------------------*/
