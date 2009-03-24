/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_user_data_property.c                                         */
/* desc : function to retrieve in Scilab the user_data field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "MALLOC.h"

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
