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
/* file: set_user_data_property.c                                         */
/* desc : function to modify in Scilab the user_data field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

/*------------------------------------------------------------------------*/
int set_user_data_property( sciPointObj * pobj, size_t stackPointer,  int valueType, int nbRow, int nbCol )
{

	/* set pobj->user_data*/
	/* the data to be assigned to the user data property is located in the Scilab Stack at the variable
	position given by stackPointer.
	nbRow, nbCol are not used */
	int *  size_ptr                                        ;
	int ** user_data_ptr                                   ;
	int    data_size     = GetDataSize( (int)stackPointer ) * 2 ; /*GetDataSize returns the size of the variable in double words */
	int *  data_ptr      = GetData( (int)stackPointer )         ;

	/* retrieve current user data matrix */
	sciGetPointerToUserData( pobj, &user_data_ptr, &size_ptr ) ;

	/* Assigning an empty matrix, free the user_data property. Check for an empty matrix */
	if ( valueType == 1 ) {
		int nr, nc, l ;
		GetRhsVar((int)stackPointer, MATRIX_OF_DOUBLE_DATATYPE, &nr, &nc, &l);
		if ( nr * nc == 0 ) /*an empty matrix */
		{
			FREE( *user_data_ptr ) ;
			*user_data_ptr = NULL ;
			*size_ptr = 0 ;
			return SET_PROPERTY_SUCCEED ;
		}
	}

	/* Assigning something else than an epty matrix*/
	if( user_data_ptr == NULL ) /* user_data property is currentlt empty */
	{
		*user_data_ptr = createIntArrayCopy( data_ptr, data_size ) ;
		*size_ptr      = data_size ;
	}
	else if( *size_ptr == data_size ) /* current user_data value as the same size than the value toassign */
	{
		intArrayCopy( *user_data_ptr, data_ptr, data_size ) ;
	}
	else  /* current user_data value as a different size than the value toassign */
	{
		FREE( *user_data_ptr ) ;
		*user_data_ptr = createIntArrayCopy( data_ptr, data_size ) ;
		*size_ptr      = data_size ;
	}

	if ( *user_data_ptr == NULL )
	{
		Scierror(999, _("%s: No more memory.\n"),"set_user_data_property") ;
		*size_ptr = 0 ;
		return SET_PROPERTY_ERROR ;
	}

	return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
