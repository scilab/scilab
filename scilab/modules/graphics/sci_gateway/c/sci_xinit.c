/*------------------------------------------------------------------------*/
/* file: sci_xinit.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xinit routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_xinit.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "Xcall1.h"
#include "DrawObjects.h"
#include "graphicSession.h"

/*-----------------------------------------------------------------------------------*/
int sci_xinit(char *fname, unsigned long fname_len )
{
  integer m1,n1,l1,v,v1=-1;
  double dv;
  CheckRhs(-1,1);

  if (Rhs <= 0 )
  {
    C2F(dr1)("xinit"," ",&v1,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,6L,2L);
    openGraphicSession() ; /* we do not draw now into the file/memory (depending on the driver type) */
  } 
  else 
  {
    char *param1=NULL;
    GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

    param1=cstk(l1);
    C2F(dr1)("xinit",cstk(l1),&v1,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,6L,m1);
    openGraphicSession() ; /* we do not draw now into the file/memory (depending on the driver type) */

  }
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
