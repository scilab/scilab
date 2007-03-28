/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/* Francois VOGEL , sciprint_full */
/*-----------------------------------------------------------------------------------*/
#ifndef __SCIPRINT_H__
#define __SCIPRINT_H__

#include <stdarg.h>

/* any string of length greater than MAXPRINTF gets truncated in sciprint */
#define MAXPRINTF 512
/* MAXCHARSSCIPRINT_FULL is for sciprint_full - more than this gets truncated */
#define MAXCHARSSCIPRINT_FULL 5000  

void  sciprint(char *fmt,...) ;
int  sciprint2(int iv,char *fmt,...);
void  sciprint_full(char *fmt,...) ;

#endif /* __SCIPRINT_H__ */
/*-----------------------------------------------------------------------------------*/ 
