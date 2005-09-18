/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTFILEASSOCIATIONBOX__
#define __INTFILEASSOCIATIONBOX__

#ifdef WIN32
  #include "../wsci/FilesAssociationBox.h"
  #include "../wsci/wgnuplib.h"
#endif

#include <stdio.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"


#ifndef NULL
  #define NULL 0
#endif

#define TRUE  1
#define FALSE 0


int C2F(intfilesassociationbox) _PARAMS((char *fname,unsigned long l));
#endif /* __INTFILEASSOCIATIONBOX__ */
/*-----------------------------------------------------------------------------------*/ 
