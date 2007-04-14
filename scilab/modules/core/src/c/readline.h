/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*-----------------------------------------------------------------------------------*/
#ifndef __READLINE_H__
#define __READLINE_H__

#include "machine.h"
int LineRead(FILE *fd,char buf[],int n,int *cnt,int *nr);
void C2F(readnextline)(integer *fd,char buf[],integer *n,integer *count,integer *nr,integer *ierr);

#endif /* __READLINE_H__ */
