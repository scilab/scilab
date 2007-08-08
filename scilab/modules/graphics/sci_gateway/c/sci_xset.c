/*------------------------------------------------------------------------*/
/* file: sci_xset.c                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xset routine                                      */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_xset.h"
#include "stack-c.h"
#include "Xcall1.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "ObjectStructure.h"
#include "sci_demo.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "DrawObjects.h"
#include "InitObjects.h"
#include "XsetXgetParameters.h"
#include "periScreen.h"
#include "CurrentObjectsManagement.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
int sci_xset( char *fname, unsigned long fname_len )
{
  integer m1,n1,l1,m2,n2,l2, xm[5],xn[5],x[5], i, v, isdc;
  integer lr, mark[2], font[2], verb=0;
  double  xx[5],dv ;
  sciPointObj *subwin = NULL; 
  BOOL keyFound = FALSE ;

  if (Rhs <= 0) {int zero=0; sci_demo(fname,"xsetm();",&zero); return 0; }

  CheckRhs(1,6);
  CheckLhs(0,1);

  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

  for ( i = 0 ; i < NUMSETFONC ; i++ )
  {
    if ( strcmp(cstk(l1),KeyTab_[i]) == 0 )
    {
      keyFound = TRUE ;
      break ;
    }
  }

  if ( !keyFound )
  {
    i = 105;v=m1*n1;
    strncpy(C2F(cha1).buf,cstk(l1),v);
    C2F(msgs)(&i,&v);
    LhsVar(1)=0;
    return 0;
  }

  /* Allan CORNET Avril 2004 */
  /* Bloque la commande xset('window') sans numero de fenetre */
  if (Rhs == 1 && (strcmp(cstk(l1),"window") == 0) )
  {
    sciprint("xset(\"window\",window-number): window-number must be set\r\n");
    LhsVar(1)=0; return 0;
  }

  if (Rhs == 2 && VarType(2) != 1) 
  {
    /* second argument is not a scalar it must be a string */ 
    GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
    C2F(xsetg)(cstk(l1),cstk(l2),m1,m2);
    LhsVar(1)=0; return 0;
  }

  if (Rhs == 1 && strcmp(cstk(l1),"default") == 0) 
  {
    /* first treatment for xsetg : then we continue */
    C2F(xsetg)(cstk(l1),"void",m1,4L);
  }

  for ( i = 2 ; i <= Rhs ; i++ ) 
  {
    GetRhsVar(i,MATRIX_OF_DOUBLE_DATATYPE,&xm[i-2],&xn[i-2],&lr);
    x[i - 2] = (integer) *stk(lr); xx[i - 2] = *stk(lr);
  }
  /* initialisation of a window if argument is not xset('window') 
  * with special cases if xset('colormap') or xset('default') 
  * and window does not exists we want to get into set_default_colormap 
  * only once 
  */
  isdc = 0;
  if (strcmp(cstk(l1),"colormap") == 0 || strcmp(cstk(l1),"default") == 0) { C2F(sedeco)(&isdc); }
  if (strcmp(cstk(l1),"window") != 0) { SciWin(); }
  isdc = 1;
  if (strcmp(cstk(l1),"colormap") == 0 || strcmp(cstk(l1),"default") == 0) C2F(sedeco)(&isdc);
  if (strcmp(cstk(l1),"wdim") == 0 || strcmp(cstk(l1),"wpdim") == 0) {
    /* Xwindows limits dimensions to 2^16 */
    if ( (x[0]>65535)||(x[1]>65535)) {
      x[0]=Min(x[0],65535);
      x[1]=Min(x[1],65535);
      i = 106;v=0;
      C2F(msgs)(&i,&v);
    }
  }

  if (strcmp(cstk(l1),"clipping") == 0) 
    C2F(dr1)("xset",cstk(l1),&v,&v,&v,&v,&v,&v,&xx[0],&xx[1],&xx[2],&xx[3],5L,bsiz);
  else if ( strcmp(cstk(l1),"colormap") == 0)
  {
    sciSetColormap (sciGetCurrentFigure(), stk(lr), *xm, *xn);
    sciRedrawFigure();
  }
  else if ( strcmp(cstk(l1),"mark size") == 0) {
    C2F(dr1)("xget","mark",&verb,mark,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);

    subwin = sciGetCurrentSubWin() ;
    sciSetMarkSizeUnit(subwin,2); /* force switch to tabulated mode : old syntax */


    mark[1]=(int)xx[0];
    subwin = sciGetCurrentSubWin();
    sciSetMarkSize(subwin,mark[1]);
    
  }
  else if ( strcmp(cstk(l1),"mark") == 0)
  {
    subwin = sciGetCurrentSubWin();
    sciSetMarkSizeUnit(subwin,2); /* force switch to tabulated mode : old syntax */
    sciSetMarkStyle(subwin,(int) xx[0]);
    sciSetMarkSize(subwin,(int) xx[1]);
    
  }
  else if ( strcmp(cstk(l1),"font size") == 0) {
    verb=0;
    C2F(dr1)("xget","font",&verb,font,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);
    font[1]=(int)xx[0];
    C2F(dr1)("xset","font",&(font[0]),&(font[1]),&v,&v,&v,&v,stk(lr),&dv,&dv,&dv,5L,5L);
  } 
  /* NG beg */
  else if ( strcmp(cstk(l1),"old_style") == 0)
  {
    if (*stk(lr) == 1)
    {
      sciprint("Old graphic mode is no longer available. Please refer to the set help page.\n") ;
    }
    else
    {
      Scierror(999,"%s: Value must be 1 or 0",fname);
    }
  }/* NG end */
  else if( strcmp(cstk(l1),"default") == 0 )
  {
    sciPointObj * pfigure = sciGetCurrentFigure();
    ResetFigureToDefaultValues(pfigure);

    /*special treatement for xset default and old_style off F.Leray 23.09.04 */
    /* mimic clf(gcf(),'reset') behaviour here */
    sciXbasc();
  }
  else if( strcmp(cstk(l1),"clipgrf") == 0 ) {
    /* special treatement for xset("cligrf") */
    sciPointObj * psubwin = sciGetCurrentSubWin();
    sciSetIsClipping(psubwin,0);
  }
  else if( strcmp(cstk(l1),"clipoff") == 0 ) {
    /* special treatement for xset("clipoff") */
    sciPointObj * psubwin = sciGetCurrentSubWin();
    sciSetIsClipping(psubwin,-1);
  }
  else if( strcmp(cstk(l1),"hidden3d") == 0 ) {
    /* special treatement for xset("hidden3d") */
    sciPointObj * psubwin = sciGetCurrentSubWin();
    pSUBWIN_FEATURE(psubwin)->hiddencolor = x[0];
  }
  else 
    C2F(dr1)("xset",cstk(l1),&x[0],&x[1],&x[2],&x[3],&x[4],&v,&dv,&dv,&dv,&dv,5L,bsiz);
  /* NG beg */
  if ( strcmp(cstk(l1),"window") == 0 )
    if (sciSwitchWindow(&x[0]) != 0){
      Scierror(999,"%s: It was not possible to create the requested figure",fname);
    }

    if ( strcmp(cstk(l1),"wshow") != 0 )
    {
      subwin = sciGetCurrentSubWin();
      if (( strcmp(cstk(l1),"foreground") == 0) || (strcmp(cstk(l1),"color") == 0) ||( strcmp(cstk(l1),"pattern") == 0) ) {
        sciSetForeground(subwin, x[0]); 
        if (sciGetSons((sciPointObj *) subwin) == (sciSons *)NULL) {/* added SS 20.04.04 */
          /* if subwin is empty, set also the ticscolor and fontforeground */
          pSUBWIN_FEATURE (subwin)->axes.ticscolor= x[0];
          sciSetFontForeground(subwin,x[0]); 
        }
      } 
      else if ( strcmp(cstk(l1),"background") == 0) {
        sciSetBackground(subwin, x[0]); 
      }  
      else if ( strcmp(cstk(l1),"thickness") == 0) {
        sciSetLineWidth(subwin, x[0]); 
        sciSetLineWidth(sciGetParent(subwin), x[0]);   
      } 
      else if ( strcmp(cstk(l1),"line style") == 0) {
        sciSetLineStyle(subwin, x[0]); 
        sciSetLineStyle(sciGetParent(subwin), x[0]);   
      }  
      else if ( strcmp(cstk(l1),"mark") == 0) {
        sciSetIsMark(subwin,1);                  /* A REVOIR F.Leray 21.01.05 */
        sciSetIsMark(sciGetParent(subwin),1);
        sciSetMarkStyle(subwin,x[0]); 
        sciSetMarkStyle(sciGetParent(subwin),x[0]);   
      } 
      else if ( strcmp(cstk(l1),"colormap") == 0) {
        sciSetColormap(sciGetParent(subwin), stk(lr),xm[0], xn[0]);
      }
      else if ( strcmp(cstk(l1),"font size") == 0) {
        int fontsize_ = 100*font[1];
        sciSetFontDeciWidth(subwin, fontsize_); 
        sciSetFontDeciWidth(sciGetParent(subwin), fontsize_);
      }     
      else if ( strcmp(cstk(l1),"dashes") == 0) {
        sciSetLineStyle(subwin, x[0]); 
        sciSetLineStyle(sciGetParent(subwin), x[0]);   
      }  
      else if ( strcmp(cstk(l1),"font") == 0) {
        sciSetFontStyle(subwin, x[0]); 
        sciSetFontDeciWidth(subwin,  x[1]*100);  
        sciSetFontStyle(sciGetParent(subwin), x[0]); 
        sciSetFontDeciWidth(sciGetParent(subwin),x[1]*100);  
      } 
      else if ( strcmp(cstk(l1),"alufunction") == 0) {
        sciSetXorMode(subwin, x[0]); 
        sciSetXorMode(sciGetParent(subwin), x[0]);   
      }
      else if ( strcmp(cstk(l1),"auto clear") == 0) {
        if ( x[0] == 1 )
        {
          sciSetAddPlot(subwin, TRUE); 
          sciSetAddPlot(sciGetParent(subwin), TRUE);  
        }
        else  
        {
          sciSetAddPlot(subwin, FALSE); 
          sciSetAddPlot(sciGetParent(subwin), FALSE);  
        } 
      } 
      else if ( strcmp(cstk(l1),"auto scale") == 0) {
        if ( x[0] == 1 )
        {
          sciSetAutoScale(subwin, TRUE); 
          sciSetAutoScale(sciGetParent(subwin), TRUE);  
        }
        else  
        {
          sciSetAutoScale(subwin, FALSE); 
          sciSetAutoScale(sciGetParent(subwin), FALSE);  
        } 
      }
      else if ( strcmp(cstk(l1),"wresize") == 0) {
        if ( x[0] == 1 )
        {
          sciSetResize(subwin, TRUE); 
          sciSetResize(sciGetParent(subwin), TRUE);  
        }
        else  
        {
          sciSetResize(subwin, FALSE); 
          sciSetResize(sciGetParent(subwin), FALSE);  
        }
      }
      else if ( strcmp(cstk(l1),"wpos") == 0) {
        sciSetScreenPosition(sciGetParent(subwin), x[0], x[1]);

      }
      else if ( strcmp(cstk(l1),"wpdim") == 0) {
        sciSetDimension(sciGetParent(subwin), x[0], x[1] ) ;
      } 
      else if ( strcmp(cstk(l1),"wdim") == 0) {
        sciSetWindowDim(sciGetParent(subwin), x[0], x[1] ) ;

      } /*Ajout A.Djalel le 10/11/03 */
      else if ( strcmp(cstk(l1),"pixmap") == 0) {
        pFIGURE_FEATURE(sciGetParent(subwin))->pixmap=x[0];
      }  
      else if ( strcmp(cstk(l1),"wshow") == 0) { /* a supprimer ce n'est pas une propriete mais une action */
        pFIGURE_FEATURE(sciGetParent(subwin))->wshow=1;
        sciSetVisibility (subwin, TRUE); 
      }
      if(strcmp(cstk(l1),"window") != 0) sciRedrawFigure();   
    }
    /***/
    /* NG end */    
    LhsVar(1)=0;
    return 0;
}
/*-----------------------------------------------------------------------------------*/
