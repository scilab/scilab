/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#ifndef __CALL_SCILAB_STATE_H__
#define __CALL_SCILAB_STATE_H__

typedef enum
{
    CALL_SCILAB_ENGINE_STOP = 0,
    CALL_SCILAB_ENGINE_STARTED = 1,
} CALL_SCILAB_ENGINE_STATE;

/* internal functions to know call_scilab engine state */

/**
* set call_scilab engine state
* @param[in] state to set
* @return new state
*/
CALL_SCILAB_ENGINE_STATE setCallScilabEngineState(CALL_SCILAB_ENGINE_STATE state);

/**
* get call_scilab engine state
* @return current call_scilab engine state
*/
CALL_SCILAB_ENGINE_STATE getCallScilabEngineState(void);

#endif /* __CALL_SCILAB_STARTED_H__ */
/*--------------------------------------------------------------------------*/
