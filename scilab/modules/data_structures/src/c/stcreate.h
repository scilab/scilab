/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Ledru Sylvestre */
/*--------------------------------------------------------------------------*/
#ifndef  __STCREATE_H__
#define __STCREATE_H__

#include "machine.h"
int C2F(stcreate)(integer *lw, integer *nz, integer *sz, integer *nf, char *fnames[], integer *retval);

int sttyp(int *ivt, char *job, int *nf, char *fnames[]);

int creonevoid(integer *slw,integer *lw);

int crelistofvoids(integer *slw,integer *lw,integer *nels);

#endif /* __STCREATE_H__ */
