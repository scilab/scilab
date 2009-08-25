/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2004 - INRIA - Djalel Abdemouche
* Copyright (C) 2006 - INRIA - Fabrice Leray
* Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
* Copyright (C) 2006 - INRIA - Vincent Couvert
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*------------------------------------------------------------------------*/
/* file: sci_get.c                                                        */
/* desc : interface for sci_get routine                                   */
/*------------------------------------------------------------------------*/
#include "sci_get.h"
/*--------------------------------------------------------------------------*/

#include "stack-c.h"
#include "HandleManagement.h"
#include "CurrentObjectsManagement.h"

#include "GetHashTable.h"
#include "BuildObjects.h"
#include "localization.h"
#include "Scierror.h"

#include "SetPropertyStatus.h"
#include "GetScreenProperty.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sciGet(sciPointObj *pobj,char *marker);
/*--------------------------------------------------------------------------*/
int sciGet(sciPointObj *pobj,char *marker)
{
	/* find the function in the hashtable relative to the property name */
	/* and call it */
	return callGetProperty( pobj, marker ) ;
}
/*--------------------------------------------------------------------------*/
int sci_get(char *fname,unsigned long fname_len)
{
	int m1,n1,numrow2,numcol2,l2 ;
	int l1;
	long hdl;

	int lw;
	sciPointObj *pobj;

	/* Root properties */
	char **stkAdr = NULL;
	int status = SET_PROPERTY_ERROR;

	CheckRhs(1,2);
	CheckLhs(0,1);

	/*  set or create a graphic window */

	/* 
	The first input argument can be an ID or a marker (in this case, get returns the value of the current object */
	switch(VarType(1))
	{
	case 1: /* tclsci handle */
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE , &m1, &n1, &l1);
		if ((int) *stk(l1) == 0) /* Root property */
		{
			CheckRhs(2,2);
			if (VarType(2) == sci_strings)
			{
				GetRhsVar(2,  MATRIX_OF_STRING_DATATYPE, &m1, &n1, &stkAdr);

				if (m1*n1 != 1)
				{
					freeArrayOfString(stkAdr, m1 * n1);
					Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "get",2);
					return SET_PROPERTY_ERROR;
				}

				status = GetScreenProperty(stkAdr[0]);
				freeArrayOfString(stkAdr, m1 * n1);

				if(status != SET_PROPERTY_SUCCEED) /* Return property */
				{
					Scierror(999, _("%s: Could not read property '%s' for root object.\n"), "get", stkAdr[0]);
					return FALSE;
				}
			}
			else
			{
				Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "get",2);
				return FALSE;
			}
			LhsVar(1) = Rhs+1;
			C2F(putlhsvar)();
		}
		else /* tclsci handle: should no more happen */
		{
			lw = 1 + Top - Rhs;
			C2F(overload)(&lw,"get",3);
		}
		return 0;
		break;
	case sci_handles: /* scalar argument (hdl + string) */
		CheckRhs(2,2);
		GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&m1,&n1,&l1);
		if (m1!=1||n1!=1) 
		{ 
			lw = 1 + Top - Rhs;
			C2F(overload)(&lw,"get",3);
			return 0;
		}
		GetRhsVar(2,STRING_DATATYPE,&numrow2,&numcol2,&l2);
		hdl = (long)*hstk(l1); /* on recupere le pointeur d'objet par le handle */
		break;
	case sci_strings:/* string argument (string) */
		CheckRhs(1,1);
		GetRhsVar(1,STRING_DATATYPE,&numrow2,&numcol2,&l2);
                if (strcmp(cstk(l2),"default_figure") != 0 && strcmp(cstk(l2),"default_axes") != 0)
                  {
                    if ( strcmp(cstk(l2),"current_figure") == 0 ||  strcmp(cstk(l2),"current_axes") == 0 ||  strcmp(cstk(l2),"current_entity") == 0 ||  strcmp(cstk(l2),"hdl") == 0)
                      {
                        hdl = 0;
                      }
                    else
                      {
                        /* Test debug F.Leray 13.04.04 */
                        if ((strcmp(cstk(l2),"children") != 0) && (strcmp(cstk(l2),"zoom_") !=0) && (strcmp(cstk(l2),"clip_box") !=0) && (strcmp(cstk(l2),"auto_") !=0)) 
                          {
                            SciWin();
                            hdl = sciGetHandle(sciGetCurrentObj());
                          }
                        else
                          {
                            hdl = sciGetHandle(sciGetCurrentSubWin());/* on recupere le pointeur d'objet par le handle */
                          }
                      }/* DJ.A 08/01/04 */
                  }
                else
                  {
                    hdl = 0;
                  }
		break;
	default:
		lw = 1 + Top - Rhs;
		C2F(overload)(&lw,"get",3);
		return 0;
		break;
	}
	/* cstk(l2) est la commande, l3 l'indice sur les parametres de la commande */
	CheckLhs(0,1);

	if (hdl == 0) 
	{
		/* No handle specified */
		if (sciGet(NULL, cstk(l2)) != 0) 
		{
			/* An error has occured */
			C2F(putlhsvar)();
			return 0;
		}
	}
	else
	{
		pobj = sciGetPointerFromHandle(hdl);
		if (pobj != NULL)
		{

			if (sciGet(pobj, cstk(l2)) != 0)
			{
				/* An error has occured */
				C2F(putlhsvar)();
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s: The handle is not or no more valid.\n"),fname);
			return 0;
		}
	}

	LhsVar(1) = Rhs + 1;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
