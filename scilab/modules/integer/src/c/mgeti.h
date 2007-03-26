/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Sylvestre Ledru */
/*-----------------------------------------------------------------------------------*/
#ifndef __MGETI_H__
#define __MGETI_H__

#include "machine.h"

int SWAP(char type[],integer *fd); 

void C2F(mgeti) (integer *fd,integer *res,integer *n,char type[],integer *ierr);

#endif /* __MGETI_H__ */