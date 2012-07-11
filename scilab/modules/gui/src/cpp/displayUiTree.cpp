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
#include <vector>
#include <sstream>
#include <iostream> 
#include <string>
#include "CallScilabBridge.hxx"

extern "C" 
{
#include "stack-c.h"
#include "gw_gui.h"
#include "stdlib.h"
#include "sciprint.h"
#include "MALLOC.h"
#include "getScilabJavaVM.h"
}

#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "displaytree.hxx"
#include "ScilabDisplayTree.hxx"

using namespace std;

extern "C" int displayUiTree(int hashMapIndex, int index)
{
	int iItemCount = 0;

	vector < string > StructList;
	int *piCurrentItem = NULL;
	string szCurLevel = "";


	iGetListItemType(index, piCurrentItem, &iItemCount, NULL);
	int *piItemType = (int*)MALLOC(iItemCount * sizeof(int));
	iGetListItemType(index, piCurrentItem, &iItemCount, piItemType);

	if(iItemCount < 2)
	{
		sciprint("Invalid size");
		return 1;
	}

	if(piItemType[0] != sci_strings && piItemType[1] != sci_mlist) //type
	{
		sciprint("Invalid tree");
		FREE(piItemType);
		return 1;
	}

	FREE(piItemType);
	/*check tree structure*/
	if(bIsTreeStructure(index, piCurrentItem, 1) == false)
	{
		sciprint("Invalid structure");
		return 1;
	}
	//Add node level
	if(szCurLevel != "")
	{
		szCurLevel + ".";
	}
	szCurLevel += "1";
	StructList.push_back(szCurLevel);

	//get label name
	char *szLabel = NULL;
	int iRet	= iGetNodeLabel(index, piCurrentItem, szLabel);
	if(iRet == -1)
	{
		return false;
	}

	szLabel		= (char*)MALLOC((iRet + 1) * sizeof(char));
	iRet			= iGetNodeLabel(index, piCurrentItem, szLabel);
	StructList.push_back(szLabel);
	FREE(szLabel);

	//get Icon name
	char *szIcon = NULL;
	iRet					= iGetNodeIcon(index, piCurrentItem, szIcon);
	if(iRet == -1)
	{
		return false;
	}

	szIcon		= (char*)MALLOC((iRet + 1) * sizeof(char));
	iRet			= iGetNodeIcon(index, piCurrentItem, szIcon);
	StructList.push_back(szIcon);
	FREE(szIcon);

	//get callback name
	char *szCallBack	= NULL;
	iRet							= iGetNodeCallBack(index, piCurrentItem, szCallBack);
	if(iRet == -1)
	{
		return false;
	}

	szCallBack		= (char*)MALLOC((iRet + 1) * sizeof(char)); //new char[iRet + 1]; replace later
	iRet			= iGetNodeCallBack(index, piCurrentItem, szCallBack);
	StructList.push_back(szCallBack);
	FREE(szCallBack); //delete[] szCallBack; replace later

	if(iRet == -1)
	{
		return false;
	}

	// Conversion Vector<string> to char **	
	char **tab = NULL;
	size_t i = 0;

	size_t struct_size = StructList.size();
	tab = new char*[struct_size];

	for(i = 0; i < struct_size; ++i)
	{
		tab[i] = strdup(StructList.at(i).c_str());
	}

	//Java
	org_scilab_modules_gui_bridge::CallScilabBridge::setUiTreeData(getScilabJavaVM(), hashMapIndex, tab, (int)struct_size);

	//Free
	for(i = 0; i < struct_size; ++i)
	{
		FREE(tab[i]);
	}

	delete [] tab;
	tab = NULL;

	return 0;
}
