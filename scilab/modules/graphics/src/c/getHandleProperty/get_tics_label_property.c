/*------------------------------------------------------------------------*/
/* file: get_tics_labels_property.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the tics_labels field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "Format.h"

/*------------------------------------------------------------------------*/
int get_tics_labels_property( sciPointObj * pobj )
{

  

  if ( sciGetEntityType (pobj) != SCI_AXES )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"tics_labels") ;
    return -1 ;
  }

  if ( pAXES_FEATURE(pobj)->str == NULL )
  {
    int status = -1 ;
    StringMatrix * tics_labels = NULL ; /* actually it is vector */

    /* tics_labels is allocatred here */
    tics_labels = computeDefaultTicsLabels( pobj ) ;

    if ( tics_labels == NULL )
    {
      sciprint("An error occured when allocating temporary tics.\n") ;
      return -1 ;
    }

    status = sciReturnRowStringVector( getStrMatData( tics_labels ), pAXES_FEATURE (pobj)->nb_tics_labels ) ;

    deleteMatrix( tics_labels ) ;

    return status ;
  }
  else
  {
    /* str has been previously set once */
    return sciReturnRowStringVector( pAXES_FEATURE(pobj)->str, pAXES_FEATURE (pobj)->nb_tics_labels ) ;
  }
}
/*------------------------------------------------------------------------*/
