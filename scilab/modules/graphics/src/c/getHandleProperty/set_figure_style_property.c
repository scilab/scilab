/*------------------------------------------------------------------------*/
/* file: set_figure_style_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the figure_style field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "InitObjects.h"
#include "Xcall1.h"
#include "DrawObjects.h"
#include "DestroyObjects.h"
#include "BuildObjects.h"
#include "SetPropertyStatus.h"

void updateMenus( struct BCG * XGC );
/*-----------------------------------------------------------------------------------*/
/* removeNewStyleMenu                                                                */
/* remove the menu and toolbar which can not be used in old style                    */
/*-----------------------------------------------------------------------------------*/
void updateMenus( struct BCG * XGC )
{
#if  _MSC_VER
  {
    extern void RefreshGraphToolBar(struct BCG * ScilabGC);
    extern void RefreshMenus(struct BCG * ScilabGC);

    RefreshMenus(XGC);
    RefreshGraphToolBar(XGC);
  }
#else
  {

    extern void refreshMenus( struct BCG * ScilabGC ) ;

    refreshMenus( XGC ) ;
    /* no toolbar under linux */

  }
#endif

}
/*------------------------------------------------------------------------*/
int set_figure_style_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  struct BCG * XGC = NULL ;
  int    verb = 0   ;
  int    v    = 1   ;
  double dv   = 0.0 ;
  int    num        ;
  int    na         ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint("Incompatible type for property figure_style.\n") ;
    return SET_PROPERTY_ERROR ;
  }


  if ( pobj == getFigureModel() )
  {
    sciprint("Can not set the style of a model.\n");    
    return SET_PROPERTY_ERROR ;
  }
    
  if ( isStringParamEqual( stackPointer, "old" ) )
  {
    if ( version_flag() == 0 )
    {
      sciXClearFigure();

      C2F(dr)("xget","gc",&verb,&v,&v,&v,&v,&v,(double *)&XGC,&dv,&dv,&dv,5L,10L);

      if (XGC->mafigure != (sciPointObj *)NULL) {
        DestroyAllGraphicsSons(XGC->mafigure);
        DestroyFigure (XGC->mafigure);
        XGC->mafigure = (sciPointObj *)NULL; 
      }

      XGC->graphicsversion = 1; /* Adding F.Leray 23.07.04 : we switch to old graphic mode */

      /* remove the Insert menu and purge the Edit menu in old style */
      /* A.Cornet, JB Silvy 12/2005 */
      updateMenus( XGC ) ;

      C2F(dr1)("xset","default",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);

      /* Add xclear to refresh toolbar for Windows */
      C2F (dr) ( "xclear", "v", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L ) ;
      return SET_PROPERTY_SUCCEED ;
    }
    else
    {
      /* nothing to do */
      return SET_PROPERTY_UNCHANGED ;
    }
  }
  else if ( isStringParamEqual( stackPointer, "new" ) )
  {   
    if ( version_flag() == 1 ) 
    {
      sciPointObj * figure = NULL ;
      sciPointObj * subwin = NULL ;

      C2F(dr1)("xset","default",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);      

      C2F(dr)("xget","window",&verb,&num,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L); 
      C2F(dr)("xstart","v",&num,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,2L);

      XGC=(struct BCG *) sciGetCurrentScilabXgc ();
      if ( (figure = ConstructFigure (XGC)) != NULL )
      {
        /* Adding F.Leray 25.03.04*/
        sciSetCurrentObj(figure);
        XGC->mafigure = figure;
        XGC->graphicsversion = 0;   /* new graphic mode */
        set_cf_type(1);
        /* Add xclear to refresh toolbar for Windows */
        C2F(dr1)("xclear","v",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,2L);
        if ((subwin = ConstructSubWin (figure, XGC->CurWindow)) != NULL)
        {
          sciSetCurrentObj( subwin );
          sciSetOriginalSubWin ( figure, subwin ) ;
        }
        /* Refresh toolbar and Menus */
        updateMenus( XGC ) ;
        return SET_PROPERTY_SUCCEED ;
      }
    }
    else
    {
      /* nothing to do */
      return SET_PROPERTY_UNCHANGED ;
    }
  }
  else
  {
    sciprint("Figure style must be 'old' or 'new'.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
