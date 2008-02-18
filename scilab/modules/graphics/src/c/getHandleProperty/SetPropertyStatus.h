/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: SetPropertySatus.h                                               */
/* desc : define the different type of return status for the sci_set      */
/*        routine                                                         */
/*------------------------------------------------------------------------*/

#ifndef _SET_PROPERTY_STATUS_
#define _SET_PROPERTY_STATUS_

/**
 * define the diffrent type of return for a set function :
 * - SET_PROPERTY_ERROR when an error occured during the set
 * - SET_PROPERTY_UNCHANGED when the property was already set to the value,
 *   then nothing should be done
 * - SET_PROPERTY_SUCCEED when the property was successfully modified
 */
typedef enum
{ SET_PROPERTY_ERROR     = -1,
  SET_PROPERTY_SUCCEED   =  0 ,
  SET_PROPERTY_UNCHANGED =  1  } SetPropertyStatus ;

/**
 * when two properties are set at the same time
 * return only one which can be used after
 */
SetPropertyStatus sciSetFinalStatus( SetPropertyStatus status1, SetPropertyStatus status2 ) ;

#endif /* _SET_PROPERTY_STATUS_ */
