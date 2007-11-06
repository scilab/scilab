/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*-----------------------------------------------------------------------------------*/
#ifndef __TEXMACS_H__
#define __TEXMACS_H__

#include "machine.h"

void settexmacs(void);
int  C2F(intexmacs)(void);
void next_input (void);
void C2F(texmacsin)(char buffer[],int *buf_size, int *len_line,int *eof,long int dummy1);

#endif /* __TEXMACS_H__ */
