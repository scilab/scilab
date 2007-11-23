/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Sylvestre Ledru */
/*--------------------------------------------------------------------------*/

#ifndef __FILEIO_H__
#define __FILEIO_H__

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "do_xxscanf.h"

int StringConvert __PARAMS((char *str));
void Free_Scan __PARAMS((int rowcount, int ncol, sfdir *type_s, entry **data));
int Sci_Store __PARAMS((int nrow,int ncol,entry* data,sfdir *type,int retval));
int Store_Scan __PARAMS((int *nrow,int *ncol,sfdir *type_s,sfdir *type,int *retval, int*retval_s, rec_entry *buf, entry **data,int rowcount,int n));
int NumTokens __PARAMS((char *str));
int SciStrtoStr __PARAMS((int *Scistring,int *nstring,int *ptrstrings,char **strh));

#endif /* __FILEIO_H__ */
