/*------------------------------------------------------------------------*/
/* file: set_sub_tics_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the sub_tics field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_sub_tics_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint("Incompatible type for property sub_tics.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType (pobj) == SCI_AXES)
  {
    pAXES_FEATURE(pobj)->subint= (int) getDoubleFromStack( stackPointer ) ;
  }
  else if ( sciGetEntityType(pobj) == SCI_SUBWIN ) 
  {
    int i ;
    double * values = getDoubleMatrixFromStack( stackPointer ) ;
    sciSubWindow * ppSubWin = pSUBWIN_FEATURE (pobj) ;
    if ( (nbCol != 3 ) && (nbCol != 2) )
    {
      sciprint( "Value must have two elements (three if in 3D).\n" ) ;
      return  SET_PROPERTY_ERROR ;
    }
    ppSubWin->flagNax = TRUE;
    for ( i = 0; i < nbCol ; i++ )
    {
      char logflag = ppSubWin->logflags[i] ;
      int  nbTicks ;

      if(logflag == 'l')
      {
        /* sciprint("Subtics number can not be set while using logarithmic scaling\n"); */
        continue ;
      }

      nbTicks = (int) values[i] ;
      if( nbTicks >= 0 )
      {
        ppSubWin->axes.nbsubtics[i] = nbTicks ;
      } 
      else
      {
        ppSubWin->axes.nbsubtics[i] = 0 ;
      }
    }
    return SET_PROPERTY_SUCCEED ;
  }
  else
  {
    sciprint( "sub_ticks property does not exist for this handle.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
