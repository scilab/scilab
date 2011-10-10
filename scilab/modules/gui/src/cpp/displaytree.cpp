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

extern "C" 
{
	#include <string.h>
	#include "sci_types.h"
    #include "stack3.h"
	#include "stdlib.h"
	#include "sciprint.h"
	#include "MALLOC.h"
}

#include "displaytree.hxx"

using namespace std;

bool bParseListItem(int _iVar, int *_piCurrentItem, vector<string> *_pvStructList, string _szLevel)
{
	int iItemCount = 0;
	iGetListItemType(_iVar, _piCurrentItem, &iItemCount, NULL);
	int *piItemType = (int*)MALLOC(iItemCount * sizeof(int));
	iGetListItemType(_iVar, _piCurrentItem, &iItemCount, piItemType);

	//parse item
	for(int i = 2 ; i < iItemCount ; i++) //tlist
	{
		if(piItemType[i] != sci_tlist)//potential tree
		{//go up, it is finish for this node
			return 0;
		}

		/*retrieve next item*/
		int *piChildItem = iGetListItemList(_iVar, _piCurrentItem, i);
		if(piChildItem == NULL || piChildItem[0] != sci_tlist)
		{
			sciprint("can get node item");
		}

		int iItemCount = 0;

		string szCurLvl = _szLevel;
		iGetListItemType(_iVar, piChildItem, &iItemCount, NULL);
		int *piItemTypeChild = (int*)MALLOC(iItemCount * sizeof(int));
		iGetListItemType(_iVar, piChildItem, &iItemCount, piItemTypeChild);

		if(iItemCount < 2)
		{
			sciprint("Invalid size");
			return 1;
		}

		if(piItemTypeChild[0] != sci_strings && piItemTypeChild[1] != sci_mlist) //type
		{
			sciprint("Invalid tree");
			FREE(piItemTypeChild);
			return 1;
		}
		FREE(piItemTypeChild);

		/*check tree structure*/
		if(bIsTreeStructure(_iVar, piChildItem, 1) == false)
		{
			sciprint("Invalid structure");
			return 1;
		}
		//Add node level
		szCurLvl += ".";
		std::ostringstream out;
		out << i-1;
		szCurLvl += out.str();

		_pvStructList->push_back(szCurLvl);

		//get label name
		char *szLabel = NULL;
		int iRet	= iGetNodeLabel(_iVar, piChildItem, szLabel);
		if(iRet == -1)
		{
			return false;
		}

		szLabel		= (char*)MALLOC((iRet + 1) * sizeof(char));
		iRet			= iGetNodeLabel(_iVar, piChildItem, szLabel);
		if(iRet == -1)
		{
			return false;
		}
		_pvStructList->push_back(szLabel);
		FREE(szLabel);

		//get Icon name
		char *szIcon = NULL;
		iRet					= iGetNodeIcon(_iVar, piChildItem, szIcon);
		if(iRet == -1)
		{
			return false;
		}

		szIcon		= (char*)MALLOC((iRet + 1) * sizeof(char));
		iRet			= iGetNodeIcon(_iVar, piChildItem, szIcon);
		if(iRet == -1)
		{
			return false;
		}
		_pvStructList->push_back(szIcon);
		FREE(szIcon);

		//get callback name
		char *szCallBack	= NULL;
		iRet							= iGetNodeCallBack(_iVar, piChildItem, szCallBack);
		if(iRet == -1)
		{
			return false;
		}

		szCallBack		= (char*)MALLOC((iRet + 1) * sizeof(char));
		iRet			= iGetNodeCallBack(_iVar, piChildItem, szCallBack);
		if(iRet == -1)
		{
			return false;
		}
		_pvStructList->push_back(szCallBack);
		FREE(szCallBack);

		bParseListItem(_iVar, piChildItem, _pvStructList, szCurLvl);
	}
	return true;
}

bool bIsTreeStructure(int _iVar, int* _piCurrentItem, int _iItemNumber)
{
	bool bRet			= false;
	int iRows			= 0;
	int iCols			= 0;
	int iLen			= 0;
	int *piLen		= NULL;
	char *pszType	= NULL;
	int iRet = iGetListSubItemString(_iVar, _piCurrentItem, 0, &iRows, &iCols, piLen, pszType);
	if(iRet != 0)
	{
		return 1;
	}
	piLen = (int*)MALLOC(sizeof(int) * iRows * iCols);

	iRet = iGetListSubItemString(_iVar, _piCurrentItem, 0, &iRows, &iCols, piLen, pszType);
	if(iRet != 0)
	{
		return 1;
	}

	iLen = iArraySum(piLen, 0, iRows * iCols) + 1; // +1 for null terminated
	pszType = (char*)MALLOC(iLen * sizeof(char));
	iRet = iGetListSubItemString(_iVar, _piCurrentItem, 0, &iRows, &iCols, piLen, pszType);
	if(iRet != 0)
	{
		return 1;
	}
	pszType[iLen - 1] = 0;

	if(strcmp(TREE_REF_NAME, pszType) == 0)
	{
		bRet = true;
	}

	FREE(piLen);
	FREE(pszType);
	return bRet;
}

/*
returns values :
0  -> OK
-1 -> failed
>0 -> size needed for _pzValue ( _pszValue must be NULL to returns needed length )
*/
int iGetNodeInfo(int _iVar, int* _piParentItem, char* _pszInfo, char* _pszValue)
{
	int iRet = 0;
	int *pStruct = iGetListItemList(_iVar, _piParentItem, 1);
	if(pStruct == NULL)
	{
		return 1;
	}

	int iItemCount = 0;
	iGetListItemType(_iVar, pStruct, &iItemCount, NULL);
	int *piItemType = (int*)MALLOC(iItemCount * sizeof(int));
	iGetListItemType(_iVar, pStruct, &iItemCount, piItemType);

	if(	iItemCount < 2 || 
			piItemType[2] != sci_strings || 
			piItemType[3] != sci_strings || 
			piItemType[4] != sci_strings)
	{//bad structure
		return 1;
	}

	/*retrieve field value*/
	iRet = iGetFieldValue(_iVar, pStruct, _pszInfo, _pszValue);
	if(iRet != 0)
	{
		return iRet;
	}

	FREE(piItemType);
	return 0;
}

/*
returns values :
0  -> OK
-1 -> failed
>0 -> size needed for _pzValue ( _pszValue must be NULL to returns needed length )
*/
int iGetNodeLabel(int _iVar, int* _piCurrentItem, char* _pszValue)
{
	return iGetNodeInfo(_iVar, _piCurrentItem, TREE_REF_LABEL, _pszValue);
}

/*
returns values :
0  -> OK
-1 -> failed
>0 -> size needed for _pzValue ( _pszValue must be NULL to returns needed length )
*/
int iGetNodeIcon(int _iVar, int* _piCurrentItem, char* _pszValue)
{
	return iGetNodeInfo(_iVar, _piCurrentItem, TREE_REF_ICON, _pszValue);
}

/*
returns values :
0  -> OK
-1 -> failed
>0 -> size needed for _pzValue ( _pszValue must be NULL to returns needed length )
*/
int iGetNodeCallBack(int _iVar, int* _piCurrentItem, char* _pszValue)
{
	return iGetNodeInfo(_iVar, _piCurrentItem, TREE_REF_CALL, _pszValue);
}

int iGetFieldIndex(int _iVar, int* _piCurrentItem, char* _pszFieldName)
{
	int  iIndex		= -1;
	int iRows			= 0;
	int iCols			= 0;
	int iLen			= 0;
	int *piLen		= NULL;
	char *pszField	= NULL;
	int iRet = iGetListSubItemString(_iVar, _piCurrentItem, 0, &iRows, &iCols, piLen, pszField);\
	if(iRet != 0)
	{
		return 1;
	}
	piLen = (int*)MALLOC(sizeof(int) * iRows * iCols);

	iRet = iGetListSubItemString(_iVar, _piCurrentItem, 0, &iRows, &iCols, piLen, pszField);
	if(iRet != 0)
	{
		return 1;
	}

	iLen = iArraySum(piLen, 0, iRows * iCols) + 1; // +1 for null terminated
	pszField = (char*)MALLOC(iLen * sizeof(char));
	iRet = iGetListSubItemString(_iVar, _piCurrentItem, 0, &iRows, &iCols, piLen, pszField);
	if(iRet != 0)
	{
		return 1;
	}
	pszField[iLen - 1] = 0;

	for(int i = 0 ; i < iRows * iCols ; i++)
	{
		char *szSubString = (char*)MALLOC(iLen * sizeof(char));
		strncpy(szSubString, pszField + iArraySum(piLen, 0, i), piLen[i]);
		szSubString[piLen[i]] = 0;
		if(strcmp(_pszFieldName, szSubString) == 0)
		{
			FREE(szSubString);
			iIndex = i;
			break;
		}
		FREE(szSubString);
	}

	FREE(piLen);
	FREE(pszField);
	return iIndex;
}

int iGetFieldValue(int _iVar, int* _piCurrentItem, char* _pszFieldName, char * _pszValue)
{
	int iIndex = iGetFieldIndex(_iVar, _piCurrentItem, _pszFieldName);
	if(iIndex == -1)
	{	
		return -1;
	}

	int iRows			= 0;
	int iCols			= 0;
	int iLen			= 0;
	int *piLen		= NULL;
	int iRet = iGetListSubItemString(_iVar, _piCurrentItem, iIndex, &iRows, &iCols, piLen, _pszValue);
	if(iRet != 0)
	{
		return -1;
	}

	piLen = (int*)MALLOC(sizeof(int) * iRows * iCols);
	iRet = iGetListSubItemString(_iVar, _piCurrentItem, iIndex, &iRows, &iCols, piLen, _pszValue);
	if(iRet != 0)
	{
		return -1;
	}

	iLen = iArraySum(piLen, 0, iRows * iCols);
	if(_pszValue == NULL)
	{
		return iLen;
	}

	iRet = iGetListSubItemString(_iVar, _piCurrentItem, iIndex, &iRows, &iCols, piLen, _pszValue);
	if(iRet != 0)
	{
		return -1;
	}
	_pszValue[iLen] = 0;


	FREE(piLen);
	return 0;
}

void displayTree(vector<string> vecteur)
{

}
