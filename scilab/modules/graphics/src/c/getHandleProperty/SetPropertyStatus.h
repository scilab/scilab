/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
 * - SET_PROPERTY_ERROR when an error occurred during the set
 * - SET_PROPERTY_UNCHANGED when the property was already set to the value,
 *   then nothing should be done
 * - SET_PROPERTY_SUCCEED when the property was successfully modified
 */
typedef enum
{
    SET_PROPERTY_ERROR     = -1,
    SET_PROPERTY_SUCCEED   =  0 ,
    SET_PROPERTY_UNCHANGED =  1
} SetPropertyStatus;

/**
 * when two properties are set at the same time
 * return only one which can be used after
 */
SetPropertyStatus sciSetFinalStatus(SetPropertyStatus status1, SetPropertyStatus status2);

/**
 * Some operation might not need a redraw. Use this function to transform
 * all success status into unchanged status.
 */
SetPropertyStatus sciSetNoRedrawStatus(SetPropertyStatus status);

#endif /* _SET_PROPERTY_STATUS_ */
