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
bool WindowsQueryRegistry(const std::string& param1, const std::string& param2, const std::string& param3, std::string& out1, int& out2, bool& isStringKey);

/**
* WindowsQueryRegistryValuesList
* Query a list of values in a registry key
* @param[in] param1 (HKEY)
* @param[in] param2 (SUBKEY)
* @param[in] dimMax
* @param[out] ListKeys (values)
* @return true or false
*/
bool WindowsQueryRegistryValuesList(const std::string& param1, const std::string& param2, int dimMax, std::vector<std::string>& ListKeys);

/**
* WindowsQueryRegistryKeysList
* Query a list of values in a registry key
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[in] dimMax
* @param[out] ListKeys (values)
* @return true or false
*/
bool WindowsQueryRegistryKeysList(const std::string& param1, const std::string& param2, int dimMax, std::vector<std::string>& ListKeys);


/**
* WindowsQueryRegistryNumberOfElementsInList
* get numbers of elements in a list
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[out] Number
* @return true or false
*/
bool WindowsQueryRegistryNumberOfValuesInList(const std::string& param1, const std::string& param2, int& number);

/**
* WindowsQueryRegistryNumberOfKeysInList
* get numbers of Key in a list
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[out] Number
* @return true or false
*/
bool WindowsQueryRegistryNumberOfKeysInList(const std::string& param1, const std::string& param2, int &Number);

/**
* GetHkeyrootFromString
* convert string value to HKEY
* @param[in] string example "HKEY_CLASSES_ROOT"
* @return HKEY
*/
HKEY GetHkeyrootFromString(const std::string& param);


#endif /* __REGISTY_H__ */
/*--------------------------------------------------------------------------*/
