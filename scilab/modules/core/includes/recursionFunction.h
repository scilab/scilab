/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __RECURSION_FUNCTION_H__
#define __RECURSION_FUNCTION_H__

/**
* PLEASE DO NOT USE RECURSION CALL TO A FUNCTION.
* ONLY FOR COMPATIBILITY
* see also callinter.h (fortran part)
*/

#include "machine.h"
#include "BOOL.h"

typedef enum { 
	RECURSION_ERROR = -1,
	RECURSION_CALL_COMP = 1,
	RECURSION_CALL_EXEC1 = 2,
	RECURSION_CALL_EXECSTR = 3,
	RECURSION_CALL_GETF = 4,
	RECURSION_CALL_SAVE = 5,
	RECURSION_CALL_LOAD = 6,
	RECURSION_CALL_DEFF = 7,
	RECURSION_CALL_DISP = 8,
	RECURSION_CALL_EXEC2 = 9,
	RECURSION_OVERLOAD = 10
} recursion_function_called;

#define GW_IO_ID 5
#define GW_CORE_ID 13
#define GW_USER_ID 14
#define GW_USER2_ID 24
#define GW_FUNCTIONS_ID 31
#define GW_OUTPUT_STREAM_ID 33
#define ERROR_GW_ID -1
#define END_OVERLOAD -2

/**
* check if there a recursion call from a function
* @return TRUE or FALSE
*/
BOOL isRecursionCallToFunction(void);

/**
* check if there a recursion call from a function (fortran)
* @return  0 (FALSE) or 1 (TRUE)
*/
int C2F(isrecursioncalltofunction)(void);

/**
* Get Gateway ID from a recursion call
* @return a gateway ID or ERROR_GW_ID
*/
int getRecursionGatewayToCall(void);

/**
* Get Gateway ID from a recursion call (fortran)
* @return gateway ID or ERROR_GW_ID
*/
int C2F(getrecursiongatewaytocall)(int *callID);

/**
* get Function ID from a recursion call 
* @return ID or RECURSION_ERROR
*/
recursion_function_called getRecursionFunctionToCall(void);

#endif /* __RECURSION_FUNCTION_H__ */
/*--------------------------------------------------------------------------*/
