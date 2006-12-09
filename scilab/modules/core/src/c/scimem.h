/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Sylvestre Ledru */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCIMEM_H_
#define  __SCIMEM_H_

#include "machine.h"

integer C2F(scimem)(integer *n, integer *ptr);
integer C2F(scigmem)(integer *n, integer *ptr);

void C2F(freegmem)(void);
void C2F(freemem)(void);
#endif /* __SCIMEM_H_ */
/*-----------------------------------------------------------------------------------*/