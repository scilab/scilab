/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2005 */
/* UImenu interface */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h> 
#include <string.h> 
/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

#include "intcommongraphics.h"
#include "../graphics/ObjectStructure.h"
/*-----------------------------------------------------------------------------------*/
extern void *sciGetPointerToFeature (sciPointObj * pobj); /* GET */
extern sciPointObj *sciGetCurrentFigure ();
extern sciPointObj *sciGetPointerFromHandle (long handle);
extern sciEntityType sciGetEntityType (sciPointObj * pobj);
extern long sciGetHandle (sciPointObj * pobj);
extern sciPointObj * ConstructUimenu (sciPointObj * pparentfigure, char *label,char *callback,BOOL handle_visible);
extern int version_flag();
/*-----------------------------------------------------------------------------------*/
int intUImenu(char *fname,unsigned long fname_len)
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

  if (version_flag()!=0)
    {
      Scierror(999,"Not valid under old graphics mode.");
      return 0;
    }

  if (Rhs == 2)
    {
      if ( (VarType(1) == sci_strings) && (VarType(2) == sci_strings) )
	{
	  SciWin();
	  pParent= (sciPointObj *) sciGetCurrentFigure(); 
			

	  GetRhsVar(1,"c",&m1,&n1,&l1);
	  labelmenu = cstk(l1); 

	  GetRhsVar(2,"c",&m2,&n2,&l2);
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
	  GetRhsVar(1,"h",&m1,&n1,&l1);
	  pParent=sciGetPointerFromHandle((long)*hstk(l1));
	  if ( (sciGetEntityType (pParent) != SCI_FIGURE) && (sciGetEntityType (pParent) != SCI_UIMENU) )
	    {
	      Scierror(999,"Must be a figure or a uimenu parent.");
	      return 0;
	    }


	  GetRhsVar(2,"c",&m2,&n2,&l2);
	  labelmenu = cstk(l2); 

	  GetRhsVar(3,"c",&m3,&n3,&l3);
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
  CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
  *hstk(outindex) = GraphicHandle;
	

  LhsVar(1)=Rhs+1;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
