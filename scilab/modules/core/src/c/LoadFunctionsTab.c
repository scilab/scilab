/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MALLOC.h"
#include "hashtable_core.h"
#include "getmodules.h"
#include "readGateway.h"
#include "LoadFunctionsTab.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/  
static int firstentry = 0;
/*--------------------------------------------------------------------------*/  
extern int C2F(cvname)(int *,char *,int *, unsigned long int);
/*--------------------------------------------------------------------------*/  
static int Add_a_Scilab_primitive_in_hashtable(char *str, int *dataI, int *data);
static BOOL Load_primitives_from_gateway_xml_file(char *modulename);
/*--------------------------------------------------------------------------*/  
void LoadFunctionsTab(void)
{
	struct MODULESLIST *Modules=NULL;
	int j=0;

	if ( firstentry != 0 ) return;

	Modules = getmodules();
	/**
	 * We are not freeing Modules in order to speed up the next call of 
	 * getmodule freed in sciquit.c 
	 */

	for (j=0;j<Modules->numberofModules;j++)
	{
		Load_primitives_from_gateway_xml_file(Modules->ModuleList[j]);
	}
	firstentry = 1;

}
/*--------------------------------------------------------------------------*/
static int Add_a_Scilab_primitive_in_hashtable(char *str, int *dataI, int *data)
{
	int ldata;
	int id[nsiz];
	int zero=0;
	C2F(cvname)(id,str,&zero,(unsigned long)strlen(str));
	ldata= (*dataI)*100+*data; /* @TODO: what is 100 ? */
	return( action_hashtable_scilab_functions(id,str,&ldata,SCI_HFUNCTIONS_ENTER));
}
/*--------------------------------------------------------------------------*/
static BOOL Load_primitives_from_gateway_xml_file(char *modulename)
{
	BOOL bOK = FALSE;

	if (modulename)
	{
		struct gateway_struct *currentGateway = NULL;
		currentGateway = readGateway(modulename);

		if (currentGateway)
		{
			int i = 0;
			for (i = 0;i < currentGateway->dimLists; i++)
			{
				if (currentGateway->primitivesList[i])
				{
					int GATEWAY_ID = currentGateway->gatewayIdList[i];
					int PRIMITIVE_ID = currentGateway->primiviteIdList[i];

					Add_a_Scilab_primitive_in_hashtable(currentGateway->primitivesList[i],
													    &GATEWAY_ID,
														&PRIMITIVE_ID);
				}
			}

			/* FREE struct currentGateway */
			freeArrayOfString(currentGateway->primitivesList,currentGateway->dimLists);
			if (currentGateway->primiviteIdList)
			{
				FREE(currentGateway->primiviteIdList);
				currentGateway->primiviteIdList = NULL;
			}
			if (currentGateway->gatewayIdList)
			{
				FREE(currentGateway->gatewayIdList);
				currentGateway->gatewayIdList = NULL;
			}
			FREE(currentGateway);
			currentGateway = NULL;

			bOK = TRUE;
		}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/
