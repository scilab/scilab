/*------------------------------------------------------------------------*/
/* file: set_z_ticks_property.c                                           */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the z_ticks field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "CheckTicksProperty.h"
#include "MALLOC.h"
#include "BasicAlgos.h"
#include "DrawObjects.h"

/*------------------------------------------------------------------------*/
int set_z_ticks_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  AssignedList * tlist     = NULL ;
  sciSubWindow * ppSubWin  = NULL ;
  int            nbTicsRow = 0    ;
  int            nbTicsCol = 0    ;

  if ( !isParameterTlist( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"z_ticks") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"ticks");
    return SET_PROPERTY_ERROR ;
  }

  ppSubWin = pSUBWIN_FEATURE(pobj) ;

  tlist = createTlistForTicks() ;

  if ( tlist == NULL )
  {
    return SET_PROPERTY_ERROR ;
  }

  /* locations */
  FREE( ppSubWin->axes.u_zgrads ) ;
  ppSubWin->axes.u_zgrads = NULL ;

  destroyStringArray( ppSubWin->axes.u_zlabels, ppSubWin->axes.u_nzgrads ) ;
  ppSubWin->axes.u_zlabels = NULL ;

  ppSubWin->axes.u_nzgrads = 0 ;

  ppSubWin->axes.u_zgrads = createCopyDoubleMatrixFromList( tlist, &nbTicsRow, &nbTicsCol ) ;

  if ( ppSubWin->axes.u_zgrads == NULL )
  {
    if ( nbTicsRow == -1 )
    {
      sciprint("Unable to assigned tics, memory full.\n") ;
      return SET_PROPERTY_ERROR ;
    }
    /* empty matrix nothing to do*/
    return SET_PROPERTY_SUCCEED ;
  }

  if ( ppSubWin->logflags[2] == 'l' )
  {
    int  i ;
    for ( i = 0 ; i < nbTicsCol * nbTicsCol ; i++ )
    {
      ppSubWin->axes.u_zgrads[i] = log10( ppSubWin->axes.u_zgrads[i] ) ;
    }
  }
  else
  {
    /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    ppSubWin->axes.nbsubtics[2] = ComputeNbSubTics( pobj,ppSubWin->axes.u_nzgrads,'n',NULL,ppSubWin->axes.nbsubtics[2] ) ;
  }

  /*  labels */

  ppSubWin->axes.u_zlabels = createCopyStringMatrixFromList( tlist, &nbTicsRow, &nbTicsCol ) ;

  ppSubWin->axes.u_nzgrads = nbTicsRow * nbTicsCol ;
  ppSubWin->axes.auto_ticks[2] = FALSE ;

  destroyAssignedList( tlist ) ;

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
