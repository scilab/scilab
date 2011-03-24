/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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
#include "stack-c.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

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

	BOOL status;

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

	status = setGraphicObjectProperty(pobj->UID, __GO_USER_DATA__, data_ptr, jni_int_vector, data_size);

	if (status == TRUE)
	{
		return SET_PROPERTY_SUCCEED;
	}
	else
	{
		return SET_PROPERTY_ERROR;
	}

}
/*------------------------------------------------------------------------*/
