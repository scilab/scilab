/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#include <math.h>
#include <stdio.h>
#include "XcosDiagram.hxx"

extern "C"
{
#include "scicos_block4.h"
#include "core_math.h"
#include "getScilabJavaVM.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
}

extern "C" double C2F(sciround)(double* x);
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xcos;
/*--------------------------------------------------------------------------*/
extern "C" void affich2(scicos_block *block,int flag)
{
	int i;
	int iRowsIn					= 0;
	int iColsIn					= 0;
	double* pdblReal		= NULL;
	char** pstValue			= NULL;
	char pstConv[128];

	//get xcos object ID
	int iBlock		= 0;
	int type = GetOparType(block, 1);
	if(type == 10)//SCSREAL_N
	{
		iBlock = (int)*(double*)GetOparPtrs(block,1);
	}
	else
	{
		return;
	}

	iRowsIn = GetInPortRows(block,1);
	iColsIn = GetInPortCols(block,1);

	pdblReal = (double*)GetInPortPtrs(block,1);

	pstValue = (char**)MALLOC(sizeof(char*) * iRowsIn * iColsIn);

	//functions
	switch(flag)
	{
	case 2 : //state evolution
		{
			int iOK = 1;
			for(i = 0 ; i < iRowsIn * iColsIn ; i++)
			{
				int iPrec				= GetIparPtrs(block)[5];
				double dblScale	= pow((double)10, iPrec);
				double dblTemp = pdblReal[i] * dblScale;
				double dblValue = C2F(sciround)(&dblTemp) / dblScale; 
				char pstFormat[10];


#if _MSC_VER
//"%0.2f"
				sprintf_s(pstFormat, 10, "%%0.%df", iPrec);
				sprintf_s(pstConv, 128, pstFormat, dblValue);
#else
				sprintf(pstFormat, "%%0.%df", iPrec);
				sprintf(pstConv, "%0.2f", dblValue);
#endif
				pstValue[i] = strdup(pstConv);
			}

			XcosDiagram::setBlockTextValue(getScilabJavaVM(), iBlock, pstValue, iRowsIn * iColsIn, iRowsIn, iColsIn);
		}
		break; 
	case 4 : //init
		for(i = 0 ; i < iRowsIn * iColsIn ; i++)
		{
#if _MSC_VER
			sprintf_s(pstConv, 128, "%0.2f", 0);
#else
			sprintf(pstConv, "%0.2f", 0);
#endif
			pstValue[i] = strdup(pstConv);
		}

		XcosDiagram::setBlockTextValue(getScilabJavaVM(), iBlock, pstValue, iRowsIn * iColsIn, iRowsIn, iColsIn);

		freeArrayOfString(pstValue, iRowsIn * iColsIn);
		break;
	default :
		FREE(pstValue);
		break;
	}
}

//	

