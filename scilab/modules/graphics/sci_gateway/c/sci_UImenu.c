/*------------------------------------------------------------------------*/
/* file: sci_UImenu.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Fabrice Leray, Jean-Baptiste Silvy             */
/* desc : interface for sci_UImenu routine                                */
/*------------------------------------------------------------------------*/

#include <stdio.h> 
#include <string.h> 
/*-----------------------------------------------------------------------------------*/
#include "sci_UImenu.h"
#include "MALLOC.h" /* MALLOC */
#include "ObjectStructure.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "CurrentObjectsManagement.h"
/*-----------------------------------------------------------------------------------*/
int sci_UImenu( char *fname,unsigned long fname_len )
{
  int numrow,numcol,outindex;
  int m1,n1,l1;
  int m2,n2,l2;
  int m3,n3,l3;

  char *labelmenu=NULL;
  char *callbackmenu=NULL;

  sciPointObj *pParent=NULL;

  unsigned long GraphicHandle=0;

  CheckRhs(2,3);
  CheckLhs(0,1);

  if (Rhs == 2)
    {
      if ( (VarType(1) == sci_strings) && (VarType(2) == sci_strings) )
	{
	  SciWin();
	  pParent= (sciPointObj *) sciGetCurrentFigure(); 
			

	  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	  labelmenu = cstk(l1); 

	  GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
	  callbackmenu = cstk(l2); 
	}
      else
	{
	  Scierror(999,"Incorrect parameter(s)");
	  return 0;
	}
    }
  else
    {
      /* Rhs == 3 */
      if ( (VarType(1) == sci_handles) && (VarType(2) == sci_strings) && (VarType(3) == sci_strings) )
	{
	  GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&m1,&n1,&l1);
	  pParent=sciGetPointerFromHandle((long)*hstk(l1));
	  if ( (sciGetEntityType (pParent) != SCI_FIGURE) && (sciGetEntityType (pParent) != SCI_UIMENU) )
	    {
	      Scierror(999,"Must be a figure or a uimenu parent.");
	      return 0;
	    }


	  GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
	  labelmenu = cstk(l2); 

	  GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
	  callbackmenu = cstk(l3); 

	}
      else
	{
	  Scierror(999,"Incorrect parameter(s)");
	  return 0;
	}

    }


  sciprint("%s %s\n",labelmenu,callbackmenu);

  GraphicHandle=sciGetHandle(ConstructUimenu (pParent,labelmenu,callbackmenu,TRUE));
	

  numrow   = 1;
  numcol   = 1;
  CreateVar(Rhs+1,GRAPHICAL_HANDLE_DATATYPE,&numrow,&numcol,&outindex);
  *hstk(outindex) = GraphicHandle;
	

  LhsVar(1)=Rhs+1;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
