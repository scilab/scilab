/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
#ifndef __REGISTY_H__
#define __REGISTY_H__

#include <string>
#include <vector>

/**
* WindowsQueryRegistry
* Query a value in a registry key
* @param[in] param1 (HKEY)
* @param[in] param2 (SUBKEY)
* @param[in] param3 (value name)
* @param[out] out1 (value REG_SZ format)
* @param[out] out2 (value int)
* @param[out] isStringKey
* @return true or false
*/
bool WindowsQueryRegistry(const std::wstring& param1, const std::wstring& param2, const std::wstring& param3, std::wstring& out1, int& out2, bool& isStringKey);

/**
* WindowsQueryRegistryValuesList
* Query a list of values in a registry key
* @param[in] param1 (HKEY)
* @param[in] param2 (SUBKEY)
* @param[in] dimMax
* @param[out] ListKeys (values)
* @return true or false
*/
bool WindowsQueryRegistryValuesList(const std::wstring& param1, const std::wstring& param2, int dimMax, std::vector<std::wstring>& ListKeys);

/**
* WindowsQueryRegistryKeysList
* Query a list of values in a registry key
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[in] dimMax
* @param[out] ListKeys (values)
* @return true or false
*/
bool WindowsQueryRegistryKeysList(const std::wstring& param1, const std::wstring& param2, int dimMax, std::vector<std::wstring>& ListKeys);


/**
* WindowsQueryRegistryNumberOfElementsInList
* get numbers of elements in a list
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[out] Number
* @return true or false
*/
bool WindowsQueryRegistryNumberOfValuesInList(const std::wstring& param1, const std::wstring& param2, int& number);

/**
* WindowsQueryRegistryNumberOfKeysInList
* get numbers of Key in a list
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[out] Number
* @return true or false
*/
bool WindowsQueryRegistryNumberOfKeysInList(const std::wstring& param1, const std::wstring& param2, int &Number);

/**
* GetHkeyrootFromString
* convert string value to HKEY
* @param[in] string example "HKEY_CLASSES_ROOT"
* @return HKEY
*/
HKEY GetHkeyrootFromString(const std::wstring& param);


#endif /* __REGISTY_H__ */
/*--------------------------------------------------------------------------*/
