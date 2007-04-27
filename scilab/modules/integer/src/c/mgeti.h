/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Sylvestre Ledru */
/*-----------------------------------------------------------------------------------*/
#ifndef __MGETI_H__
#define __MGETI_H__

#include "machine.h"

/**
 * <long-description>
 *
 * @param char type[]   
 * @param fd    
 * @return <ReturnValue>
 */
int SWAP(char type[],integer *fd); 

/**
 * <long-description>
 *
 * @param fd    
 * @param res   
 * @param n 
 * @param char type[]   
 * @param ierr  
 */
void C2F(mgeti) (integer *fd,integer *res,integer *n,char type[],integer *ierr);

#endif /* __MGETI_H__ */

