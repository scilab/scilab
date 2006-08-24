/*------------------------------------------------------------------------*/
/* file: sci_addcb.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for addcb routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_addcb.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "Interaction.h"
#include "BuildObjects.h"

/*-----------------------------------------------------------------------------------*/
int sci_addcb(char *fname,unsigned long fname_len)
{  
  unsigned long hdl;
  integer m1, n1, l1,m2, n2, l2;
  sciPointObj *pobj;

  CheckRhs(2,3);
  CheckLhs(0,1);

  /*  set or create a graphic window*/
  SciWin();
  switch(VarType(1)) 
  {
  case 1: /* first is a scalar argument so it's a legend(hdl,"str1",...)*/
    CheckRhs(3,3);
    GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument*/
    hdl = (unsigned long)*hstk(l1); /* on recupere le pointeur d'objet par le handle*/
    GetRhsVar(2,"c",&m1,&n1,&l1); /* Gets the command name  */
    GetRhsVar(3,"i",&m2,&n2,&l2); /* Gets the mouse event */
    break;
  case 10:/* first is a string argument so it's a sciset("str1",....)*/
    hdl = (unsigned long ) sciGetHandle(sciGetSelectedSubWin(sciGetCurrentFigure())); /* Gets the figure handle value if it ones (the phather)*/
    CheckRhs(2,2);
    GetRhsVar(1,"c",&m1,&n1,&l1); /* Gets the command name    */
    GetRhsVar(2,"i",&m2,&n2,&l2); /* Gets the mouse event */
    break;
  default: 
    Scierror(999,"%s: Pad parameters\r\n",fname);
    return 0;
    break;

  }
  if ((pobj = sciGetPointerFromHandle(hdl)) != NULL )
    sciAddCallback((sciPointObj *)pobj, cstk(l1),m1*n1,*istk(l2));
  else {
    Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
    return 0;
  }
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
