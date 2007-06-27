/*------------------------------------------------------------------------*/
/* file: set_axes_size_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the axes_size field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "Xcall1.h"
#include "InitObjects.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_axes_size_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  double * newWindowSize = getDoubleMatrixFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint("Incompatible type for property axes_size.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    sciprint("axes_size property undefined for this object") ;
    return SET_PROPERTY_ERROR ;
  }

  sciSetDimension(pobj, (int) newWindowSize[0], (int) newWindowSize[1] ) ;

  /*pFIGURE_FEATURE(pobj)->windowdimwidth = (int) newWindowSize[0] ; 
  pFIGURE_FEATURE(pobj)->windowdimheight= (int) newWindowSize[1] ;

  if ( pobj != getFigureModel() )
  {
    int verbose   = 0 ;
    int na        = 0 ;
    int curFigNum = 0 ;
    int figNum = sciGetNum( pobj ) ;

    C2F(dr)("xget","window",&verbose,&curFigNum,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
    C2F(dr)("xset","window",&figNum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xset","wdim",
      &(pFIGURE_FEATURE(pobj)->windowdimwidth),
      &(pFIGURE_FEATURE(pobj)->windowdimheight),
      PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xset","window",&curFigNum,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  }*/

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
