/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 

#ifndef __DISPLAY_TREE__
#define __DISPLAY_TREE__

#include <vector>
#include <sstream>
#include <string>

using namespace std;

#define TREE_REF_NAME			"Tree"
#define TREE_REF_LABEL		"label"
#define TREE_REF_ICON			"icon"
#define TREE_REF_CALL			"callback"

bool bIsTreeStructure(int _iVar, int* _piCurrentItem, int _iItemNumber);
bool bParseListItem(int _iVar, int *_piCurrentItem, std::vector<std::string> *_pvStructList, std::string _szLevel);
int iGetFieldValue(int _iVar, int* _piCurrentItem, char* _pszFieldName, char * _pszValue);
int iGetFieldIndex(int _iVar, int* _piCurrentItem, char* _pszFieldName);
int iGetNodeCallBack(int _iVar, int* _piCurrentItem, char* _pzValue);
int iGetNodeIcon(int _iVar, int* _piCurrentItem, char* _pzValue);
int iGetNodeLabel(int _iVar, int* _piCurrentItem, char* _pzValue);

/**
* Give to Java the Tree data structure
* @param[in] vecteur the tree data structure
*/
void displayTree(vector<string> vecteur);

#endif /* __DISPLAY_TREE__ */