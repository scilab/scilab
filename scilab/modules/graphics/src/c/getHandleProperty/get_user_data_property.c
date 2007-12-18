/*------------------------------------------------------------------------*/
/* file: get_user_data_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the user_data field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_user_data_property( sciPointObj * pobj )
{
  /* user_data */
  int *size_ptr, data_size;
  int **user_data_ptr,*data_ptr;
  sciGetPointerToUserData (pobj,&user_data_ptr, &size_ptr);

  data_ptr=*user_data_ptr;
  data_size=0;
  if ( *user_data_ptr == NULL || *size_ptr == 0 )
  {
    return sciReturnEmptyMatrix() ;
  }
  else
  {
    return sciReturnUserData( *user_data_ptr, *size_ptr ) ;
  }

}
/*------------------------------------------------------------------------*/
