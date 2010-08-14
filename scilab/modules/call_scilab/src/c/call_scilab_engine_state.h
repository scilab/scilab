/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
