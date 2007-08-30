/*------------------------------------------------------------------------*/
/* file: get_log_flags_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the log_flags field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_log_flags_property( sciPointObj * pobj )
{
  char logFlagsString[4];
  if ( sciGetEntityType (pobj) != SCI_SUBWIN )
  {
    sciprint( "log_flag property undefined for this object\n." ) ;
    return -1 ;
  }

  /* Set the three first character of log flags.*/
  sciGetLogFlags(pobj, logFlagsString);
  logFlagsString[4] = 0; /* 0 terminating character */
  return sciReturnString( logFlagsString ) ;
}
/*------------------------------------------------------------------------*/
