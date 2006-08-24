/*------------------------------------------------------------------------*/
/* file: sci_delete.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for delete routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_delete.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "Xcall1.h"
#include "Interaction.h"

/*-----------------------------------------------------------------------------------*/
int sci_delete(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,num,cur,na,verb=0, lw;
  unsigned long hdl;
  sciPointObj *pobj, *pparentfigure;

  CheckRhs(0,1);
  CheckLhs(0,1);
  /*SciWin();*/
  switch(VarType(1))
  {
  case 9: /* delete Entity given by a handle */
    GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument */
    if (m1!=1||n1!=1) {
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"delete",6);return 0;}
    if (Rhs == 2)
      GetRhsVar(2,"c",&m2,&n2,&l2); /* Gets the command name */
    hdl = (unsigned long)*hstk(l1); /* Puts the value of the Handle to hdl */
    break;
  case 10: /* delete("all") */
    CheckRhs(1,1);
    GetRhsVar(1,"c",&m2,&n2,&l2);
    if (strcmp(cstk(l2),"all") == 0)
    {
      SciWin();
      sciXbasc();return 0;
    }
    else
    {
      Scierror(999,"%s :Incorrect argument\r\n",fname);
      return 0;
    }
    break;
  default:
    CheckRhs(0,0);
    hdl = (unsigned long ) sciGetHandle((sciPointObj *)sciGetCurrentObj());
    break;
  }

  pobj = sciGetPointerFromHandle(hdl);
  if (pobj == (sciPointObj *)NULL) {
    Scierror(999,"%s :the handle is not valid\r\n",fname);
    return 0;
  }
  num= sciGetNumFigure (pobj);
  SciWin();
  C2F (dr) ("xget", "window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F (dr) ("xset", "window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  if ((Rhs == 2) && (strcmp(cstk(l2),"callback") == 0))
  {
    sciDelCallback((sciPointObj *)pobj);
  }
  else {

    sciEntityType objType    = sciGetEntityType( pobj ) ;
    BOOL          isSelected = sciGetIsSelected( pobj ) ;

    if (sciGetParentFigure(pobj) != NULL && sciGetEntityType(pobj) != SCI_FIGURE)
    { /* I added && sciGetEntityType(pobj) != SCI_FIGURE at last !!!!!!!! F.Leray 09.04.04 */
      pparentfigure = sciGetParentFigure(pobj);
      sciSetCurrentObj((sciPointObj *)sciGetParent((sciPointObj *)pobj)); /* A LAISSER F.Leray 25.03.04*/
      sciDelGraphicObj((sciPointObj *)pobj);

      /* test here: we could have deleted the selected subwindow, we must choose an other */
      /* We must always have one selected subwindow (if at least one subwindow exists) */
      if ( objType  == SCI_SUBWIN && isSelected )
      {
        /* we have to select antoher subwindow if one exists at least */
        sciSelectFirstSubwin( pparentfigure ) ;
      }

      /* redraw the window */
      sciDrawObj((sciPointObj *)pparentfigure);
    }
    else if(sciGetEntityType(pobj) == SCI_FIGURE) /* F.Leray 13.04.04: We delete the special object Figure !!*/
    {
      sciDeleteWindow( num );
    }

  }

  if(cur != num)
  {
    C2F (dr) ("xset", "window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }
  LhsVar(1)=0;
  return 0;
}