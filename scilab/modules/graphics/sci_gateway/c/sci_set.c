/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan Cornet
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
/* file: sci_set.h                                                        */
/* desc : interface for sci_set routine                                   */
/*------------------------------------------------------------------------*/
#include <stdio.h>
/*------------------------------------------------------------------------*/
#include "gw_graphics.h"
#include "stack-c.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "GraphicSynchronizerInterface.h"
#include "freeArrayOfString.h"

#include "SetHashTable.h"
#include "SetPropertyStatus.h"


#include "MALLOC.h" /* MALLOC */
#include "DrawingBridge.h"
#include "localization.h"
#include "stricmp.h"

/*--------------------------------------------------------------------------*/
static int sciSet(sciPointObj *pobj, char *marker, size_t *value, int valueType, int *numrow, int *numcol) ;
/*--------------------------------------------------------------------------*/
/**@name int sciset(sciPointObj *pobj,char *marker, long *x, long *y, long *w, long *h)
* Sets the value to the object
*/
static int sciSet(sciPointObj *pobj, char *marker, size_t *value, int valueType, int *numrow, int *numcol)
{
	return callSetProperty( pobj, *value, valueType, *numrow, *numcol, marker ) ;
}
/*--------------------------------------------------------------------------*/

/*-----------------------------------------------------------
* sciset(choice-name,x1,x2,x3,x4,x5)
* or   xset()
*-----------------------------------------------------------*/
int sci_set(char *fname, unsigned long fname_len)
{
	int lw = 0;
	int isMatrixOfString = 0;
	CheckRhs(2,3);
	CheckLhs(0,1);


	if (VarType(1) == sci_matrix) /* tclsci handle */
	{
		lw = 1 + Top - Rhs;
		/* call "set" for tcl/tk see tclsci/sci_gateway/c/sci_set.c */
		C2F(overload)(&lw,"set",3);
		return 0;
	}
	else /* others types */
	{
		int m1,n1,l1,m2,n2,l2;
		int numrow3 = 0 ;
		int numcol3 = 0 ;
		size_t l3 = 0 ;
		unsigned long hdl;
		BOOL vis_save = FALSE ;
		sciPointObj *pobj = NULL;

		int valueType = 0 ; /* type of the rhs */

		int numrow[4],i;
		int numcol[4], lxyzcol[4];
		int ptrindex[2];
		int setStatus = 0 ; 
		/* after the call to sciSet get the status : 0 <=> OK,          */ 
		/*                                          -1 <=> Error,       */
		/*                                           1 <=> nothing done */

		/* F.Leray Init. to 0*/
		for(i=0;i<4;i++)
		{
			numrow[i] = 0;
			numcol[i] = 0;
			lxyzcol[i] = 0;
		}
		ptrindex[0] = 0;
		ptrindex[1] = 0;

		/*  set or create a graphic window */
		switch(VarType(1)) 
		{
		case sci_handles: 
			/* first is a scalar argument so it's a gset(hdl,"command",[param]) */
			/* F.Leray; INFO: case 9 is considered for a matrix of graphic handles*/
			CheckRhs(3,3);
			GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&m1,&n1,&l1); /* Gets the Handle passed as argument */
			if (m1 != 1 || n1 != 1) 
			{ 
				lw = 1 + Top - Rhs;
				C2F(overload)(&lw,"set",3);
				return 0;
			}

			hdl = (long)*hstk(l1);
			pobj = sciGetPointerFromHandle(hdl);

			GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2); /* Gets the command name */ 

			valueType = VarType(3) ;
			if ( (strcmp( cstk(l2), "user_data") == 0) || (stricmp( cstk(l2), "userdata") == 0)) {
				/* in this case set_user_data_property
				directly uses the  third position in the stack
				to get the variable which is to be set in
				the user_data property (any data type is allowed) S. Steer*/
				l3 = 3; /*position in the stack*/
				numrow3 = -1; /*unused */
				numcol3 = -1; /*unused */
			}
			else if ( valueType == sci_matrix )
			{
				GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&numrow3,&numcol3,&l3);
			}
			else if ( valueType == sci_boolean )
			{
				GetRhsVar(3,MATRIX_OF_BOOLEAN_DATATYPE,&numrow3,&numcol3,&l3);
			}
			else  if ( valueType == sci_handles )
			{
				GetRhsVar(3,GRAPHICAL_HANDLE_DATATYPE,&numrow3,&numcol3,&l3);
			}
			else if ( valueType == sci_strings ) 
			{ 
				if (    strcmp( cstk(l2), "tics_labels"  ) != 0
					&& strcmp( cstk(l2), "auto_ticks"   ) != 0
					&& strcmp( cstk(l2), "axes_visible" ) != 0
					&& strcmp( cstk(l2), "axes_reverse" ) != 0
					&& strcmp( cstk(l2), "text"         ) != 0 
					&& stricmp( cstk(l2), "string"       ) != 0) /* Added for uicontrols */
				{
					GetRhsVar(3,STRING_DATATYPE,&numrow3,&numcol3,&l3);
				} 
				else
				{
					isMatrixOfString = 1;
					GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&numrow3,&numcol3,&l3);
				}
			}
			else if ( valueType == sci_list ) /* Added for callbacks */
			{ 
				GetRhsVar(3,LIST_DATATYPE,&numrow3,&numcol3,&l3);
				l3 = 3; /* In this case l3 is the list position in stack */
			}
			break;

		case sci_strings:/* first is a string argument so it's a set("command",[param]) */ 
			CheckRhs(2,2);
			GetRhsVar(1,STRING_DATATYPE,&m2,&n2,&l2);
			hdl = 0;
			pobj = NULL;
			valueType = VarType(2) ;

			if (valueType == sci_matrix )
			{
				GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&numrow3,&numcol3,&l3);
			}
			else if ( valueType == sci_handles )
			{
				GetRhsVar(2,GRAPHICAL_HANDLE_DATATYPE,&numrow3,&numcol3,&l3);
			}
			else if ( valueType == sci_strings )
			{
				if (strcmp( cstk(l2), "tics_labels"  ) == 0
					|| strcmp( cstk(l2), "auto_ticks"   ) == 0
					|| strcmp( cstk(l2), "axes_visible" ) == 0
					|| strcmp( cstk(l2), "axes_reverse" ) == 0
					|| strcmp( cstk(l2), "text"      ) == 0 )
				{
					isMatrixOfString = 1;
					GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&numrow3,&numcol3,&l3);
				} 
				else
				{
					GetRhsVar(2,STRING_DATATYPE,&numrow3,&numcol3,&l3);
				}
			}
			break;

		default:
			Scierror(999,_("%s: Wrong type for input argument #%d: String or handle expected.\n"),fname,1);
			return 0;
			break;
		}

		if ( hdl != 0 ) 
		{
			sciPointObj * parentFigure;
			pobj = sciGetPointerFromHandle(hdl);

			if ( pobj == NULL )
		 {
			 Scierror(999,_("%s: The handle is not or no more valid.\n"),fname);
			 return 0;
		 }
			vis_save = sciGetVisibility(pobj) ; /*used not to redraw the figure is object remains invisible */

			parentFigure = sciGetParentFigure(pobj);

			if (!sciIsAutomaticallyRedrawn(pobj) && parentFigure != NULL)
			{
				/* try to protect figure if possible */
				startFigureDataWriting(parentFigure);
				setStatus = sciSet(pobj, cstk(l2), &l3, valueType, &numrow3, &numcol3);
				endFigureDataWriting(parentFigure);
			}
			else
			{
				setStatus = sciSet(pobj, cstk(l2), &l3, valueType, &numrow3, &numcol3);
			}

			if ( setStatus < 0 )
		 {
			 /* An error occured */
			 LhsVar(1)=0;
			 C2F(putlhsvar)();
			 return 0 ;
		 }

			if ( !( vis_save == 0 && sciGetVisibility(pobj)== 0) && setStatus == 0 ) 
		 {
			 /* do not redraw figure if object remains invisible */
			 /* and if its not a model object and if it's not a gui object */
			 if (   setStatus == SET_PROPERTY_SUCCEED
				   && (vis_save || sciGetVisibility(pobj))
					 && !isModelObject(pobj)
					 && !sciIsAutomaticallyRedrawn(pobj))
			 { 
				 sciDrawObj(pobj) ;
			 }
		 }
		}
		else
		{
			/* No object specified */
			sciSet( NULL, cstk(l2), &l3, valueType, &numrow3, &numcol3);
		}
		
		if(isMatrixOfString)
		{
			freeArrayOfString((char**)l3, numrow3*numcol3);
		}
		
		LhsVar(1) = 0;
		C2F(putlhsvar)();
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
