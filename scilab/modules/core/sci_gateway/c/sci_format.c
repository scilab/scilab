/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "gw_core.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "core_math.h"
/*--------------------------------------------------------------------------*/
#define e_type_format "e"
#define v_type_format "v"
#define format_MIN 2
#define format_MAX 25
#define format_e_MAX 8
#define mode_variable 1
#define mode_e 0
/*--------------------------------------------------------------------------*/
static void setVariableFormat(int numberDigits);
static void set_e_Format(int numberDigits);
static void getFormat(int *e_mode, int *numberDigits);
static void setVariableMode(void);
static int sci_format_norhs(char *fname);
static int sci_format_onerhs(char *fname);
static int sci_format_tworhs(char *fname);
/*--------------------------------------------------------------------------*/
int C2F(sci_format)(char *fname,unsigned long fname_len)
{
	Rhs = Max(0,Rhs);
	CheckRhs(0,2);
	CheckLhs(0,1);

	switch (Rhs)
	{
		case 2:
			return sci_format_tworhs(fname);
		case 1:
			return sci_format_onerhs(fname);
		break;
		case 0: default:
			return sci_format_norhs(fname);
	}
}
/*--------------------------------------------------------------------------*/
static void setVariableFormat(int numberDigits)
{
	int numberDigitsAdjusted = numberDigits;

	C2F(iop).lct[5] = mode_variable; /* set 'v' mode */

	numberDigitsAdjusted = Min( Max(format_MIN, numberDigits), format_MAX );
	C2F(iop).lct[6] = numberDigitsAdjusted;
}
/*--------------------------------------------------------------------------*/
static void setVariableMode(void)
{
	C2F(iop).lct[5] = mode_variable; /* set 'v' mode */
}
/*--------------------------------------------------------------------------*/
static void set_e_Format(int numberDigits)
{
	 int numberDigitsAdjusted = numberDigits;

	 C2F(iop).lct[5] = mode_e; /* set 'e' mode */

	 numberDigitsAdjusted = Min( Max(format_MIN, numberDigits), format_MAX );
     numberDigitsAdjusted = Max( numberDigitsAdjusted, format_e_MAX );

     C2F(iop).lct[6] = numberDigitsAdjusted;
}
/*--------------------------------------------------------------------------*/
static void getFormat(int *e_mode, int *numberDigits)
{
	*e_mode = C2F(iop).lct[5];
	*numberDigits = C2F(iop).lct[6];
}
/*--------------------------------------------------------------------------*/
static int sci_format_norhs(char *fname)
{
	int n1 = 0, m1 = 0;
	int *paramoutINT = NULL;
	paramoutINT = (int*)MALLOC(sizeof(int)*2);

	getFormat(&paramoutINT[0], &paramoutINT[1]);

	n1 = 1; m1 = 2;
	CreateVarFromPtr(Rhs+1, MATRIX_OF_INTEGER_DATATYPE, &n1, &m1, &paramoutINT);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	if (paramoutINT) {FREE(paramoutINT); paramoutINT = NULL;}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_format_onerhs(char *fname)
{
	if ( GetType(1) == sci_strings )
	{
		int n1 = 0, m1 = 0, l1 = 0;
		char *param = NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		param = cstk(l1);

		if ( ( strcmp(e_type_format, param) == 0 ) || ( strcmp(v_type_format, param) == 0 ) )
		{
			int previous_mode = 0;
			int previous_numberDigits = 0;

			getFormat(&previous_mode, &previous_numberDigits);

			if ( strcmp(e_type_format, param) == 0 )
			{
				set_e_Format(previous_numberDigits);
			}
			else /* v_type_format */
			{
				setVariableFormat(previous_numberDigits);
			}

			LhsVar(1) = 0;
			C2F(putlhsvar)();
		}
		else
		{
			Scierror(999,_("%s: Wrong values for input argument #%d: '%s' or '%s' expected.\n"),fname,1,e_type_format,v_type_format);
			return 0;
		}
	}
	else if ( GetType(1) == sci_matrix )
	{
		int n1 = 0, m1 = 0, l1 = 0;
		double dvalue;
		int value = 0;

		int mode = 0;
		int previous_numberDigits = 0;

		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);

		if ( (m1  != 1) || (n1  != 1) )
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,2);
			return 0;
		}

		dvalue = *stk(l1);
		value = (int) dvalue;

		if (dvalue != (double)value)
		{
			Scierror(999,_("%s: Wrong values for input argument #%d: A int expected.\n"),fname,1);
			return 0;
		}

		if ( (value < format_MIN) || (value > format_MAX) )
		{
			Scierror(999,_("%s: Wrong values for input argument #%d: Must be in the interval [%d, %d].\n"),fname,1,format_MIN,format_MAX);
			return 0;
		}

		getFormat(&mode, &previous_numberDigits);

		if (mode == mode_e)
		{
			set_e_Format(value);
		}
		else /* mode_variable */
		{
			setVariableFormat(value);
		}

		LhsVar(1) = 0;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string or a scalar expected.\n"),fname,1);
		return 0;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_format_tworhs(char *fname)
{
	/* format(1,10) */
	if ((GetType(1) == sci_matrix) && (GetType(2) == sci_matrix))
	{
		int n1 = 0, m1 = 0, l1 = 0, m1n1 = 0;
		int n2 = 0, m2 = 0, l2 = 0, m2n2 = 0;

		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
		m1n1 = m1 * n1;

		GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
		m2n2 = m2 * n2;

		if ( (m1n1 == 1) && (m2n2 == 1) )
		{
			double type_value_d = *stk(l2);
			double v_value_d = *stk(l1);
			int v_value = (int)v_value_d;

			if ( (type_value_d != (double)0) && (type_value_d != (double)1) )
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: '0' or '1' expected.\n"),fname,2);
				return 0;
			}

			if (v_value_d != (double)v_value)
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: A int expected.\n"),fname,1);
				return 0;
			}

			if ( (v_value < format_MIN) || (v_value > format_MAX) )
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"),fname,2,format_MIN,format_MAX);
				return 0;
			}

			if (type_value_d == 0)
			{
				setVariableFormat(v_value);
			}
			else
			{
				set_e_Format(v_value);
			}

			setVariableMode();

			LhsVar(1) = 0;
			C2F(putlhsvar)();
		}
		else
		{
			Scierror(999,_("%s: Wrong size for inputs arguments.\n"),fname);
		}
	}
	/* format('e',10) & format(10,'e') syntax */
	else if ( ((GetType(1) == sci_strings) && (GetType(2) == sci_matrix)) ||
		 ((GetType(1) == sci_matrix) && (GetType(2) == sci_strings)) )
	{
		int n1 = 0, m1 = 0, l1 = 0;
		char *param = NULL;

		int n2 = 0, m2 = 0, l2 = 0;
		int value = 0;
		

		if (GetType(1) == sci_strings)
		{
			double dvalue = 0;

			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
			param = cstk(l1);

			GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);

			if ( (m2  != 1) || (n2  != 1) )
			{
				Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,2);
				return 0;
			}

			dvalue = *stk(l2);
			value = (int)dvalue;
			if (dvalue != (double)value)
			{
				Scierror(999,_("%s: Wrong values for input argument #%d: A int expected.\n"),fname,2);
				return 0;
			}
		}
		else /* matrix */
		{
			double dvalue = 0;

			GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);

			if ( (m1  != 1) || (n1  != 1) )
			{
				Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
				return 0;
			}

			dvalue = *stk(l1);
			value = (int)dvalue;
			if (dvalue != (double)value)
			{
				Scierror(999,_("%s: Wrong values for input argument #%d: A int expected.\n"),fname,1);
				return 0;
			}

			GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
			param = cstk(l2);
		}

		if ( ( strcmp(e_type_format, param) == 0 ) || ( strcmp(v_type_format, param) == 0 ) )
		{
			if ( (value < format_MIN) || (value > format_MAX) )
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"),fname,1,format_MIN,format_MAX);
				return 0;
			}

			if ( strcmp(e_type_format, param) == 0 )
			{
				 set_e_Format(value);
			}
			else /* v_type_format */
			{
				 setVariableFormat(value);
			}

			LhsVar(1) = 0;
			C2F(putlhsvar)();
		}
		else
		{
			Scierror(999,_("%s: Wrong values for input argument #%d: '%s' or '%s' expected.\n"),fname,1,e_type_format,v_type_format);
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for inputs arguments.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
