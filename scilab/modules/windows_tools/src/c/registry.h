/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#ifndef __REGISTY_H__
#define __REGISTY_H__

#include <Windows.h>
#include "BOOL.h" /* BOOL */

/**
* WindowsQueryRegistry
* Query a value in a registry key
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[in] ParamIn3 (value name)
* @param[out] ParamOut1 (value REG_SZ format)
* @param[out] ParamOut2 (value int)
* @param[out] OuputIsREG_SZ
* @return TRUE or FALSE
*/
BOOL WindowsQueryRegistry(char *ParamIn1, char *ParamIn2, char *ParamIn3, char *ParamOut1, int *ParamOut2, BOOL *OuputIsREG_SZ);

/**
* WindowsQueryRegistryValuesList
* Query a list of values in a registry key
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[in] dimMax
* @param[out] ListKeys (values)
* @return TRUE or FALSE
*/
BOOL WindowsQueryRegistryValuesList(char *ParamIn1, char *ParamIn2, int dimMax, char **ListKeys);

/**
* WindowsQueryRegistryKeysList
* Query a list of values in a registry key
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[in] dimMax
* @param[out] ListKeys (values)
* @return TRUE or FALSE
*/
BOOL WindowsQueryRegistryKeysList(char *ParamIn1, char *ParamIn2, int dimMax, char **ListKeys);


/**
* WindowsQueryRegistryNumberOfElementsInList
* get numbers of elements in a list
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[out] Number
* @return TRUE or FALSE
*/
BOOL WindowsQueryRegistryNumberOfValuesInList(char *ParamIn1, char *ParamIn2, int *Number);

/**
* WindowsQueryRegistryNumberOfKeysInList
* get numbers of Key in a list
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[out] Number
* @return TRUE or FALSE
*/
BOOL WindowsQueryRegistryNumberOfKeysInList(char *ParamIn1, char *ParamIn2, int *Number);

/**
* GetHkeyrootFromString
* convert string value to HKEY
* @param[in] string example "HKEY_CLASSES_ROOT"
* @return HKEY
*/
HKEY GetHkeyrootFromString(char *string);


#endif /* __REGISTY_H__ */
/*--------------------------------------------------------------------------*/
