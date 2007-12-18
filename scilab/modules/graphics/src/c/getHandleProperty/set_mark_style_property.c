/*------------------------------------------------------------------------*/
/* file: set_mark_style_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the mark_style field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_mark_style_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  int status1 ;
  int status2 ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"mark_style") ;
    return SET_PROPERTY_ERROR ;
  }

   status1 = sciSetIsMark( pobj, TRUE ) ;
   status2 = sciSetMarkStyle( pobj, (int) getDoubleFromStack(stackPointer) ) ;

  return sciSetFinalStatus( (SetPropertyStatus)status1, (SetPropertyStatus)status2 ) ;
}
/*------------------------------------------------------------------------*/
