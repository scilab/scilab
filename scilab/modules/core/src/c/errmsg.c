/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 - 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "errmsg.h"
#include "localization.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "stackinfo.h"
#include "core_math.h"
#include "inffic.h"
#include "../../../output_stream/src/c/msgstore.h" /* need to move errmsg in output_stream */
/*-----------------------------------------------------------------------------------*/
extern int C2F(showstack)(); /* used in error 115 */
extern int C2F(cvname)(); /* used to get function name */
extern int C2F(prntid)(); /* to print variables on stack */
/*-----------------------------------------------------------------------------------*/
static void strip_blank(char *source);
static void displayAndStoreError(const char *msg,...);
static void resetLastError(void);
static char *getConvertedNameFromStack(int cvnametype);
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif
/*-----------------------------------------------------------------------------------*/
typedef enum {
	CVNAME_READING_TYPE_1 = 0, 
	CVNAME_READING_TYPE_2 = 1, 
	CVNAME_READING_TYPE_3 = 2, 
	CVNAME_READING_TYPE_4 = 3 
} CVNAME_TYPE;
/*-----------------------------------------------------------------------------------*/
int C2F(errmsg)(integer *n,integer *errtyp)
{
	resetLastError();
	
	/* store error code (lasterror) */
	C2F(errstore)(n);

	*errtyp = 0; /* by default errors are recoverable */
	/* errors not recoverable aren't catchable by try,catch */ 
	/* errors 2,3,16,26,31,34,35,40,46,47,276*/

    switch ((int)*n)
    {
		case 1:
		{
			displayAndStoreError(_("Incorrect assignment\n"));

		}
		break;
		case 2:
		{
			displayAndStoreError(("Invalid factor\n"));
			*errtyp = 1;
		}
		break;
		case 3:
		{
			displayAndStoreError(_("Waiting for right parenthesis\n"));
			*errtyp = 1;
		}
		break;
		case 4:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Undefined variable : %s\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 5:
		{
			displayAndStoreError(_("Inconsistent column/row dimensions\n"));
		}
		break;
		case 6:
		{
			displayAndStoreError(_("Inconsistent row/column dimensions\n"));
		}
		break;
		case 7:
		{
			displayAndStoreError(_("Dot cannot be used as modifier for this operator\n"));
		}
		break;
		case 8:
		{
			displayAndStoreError(_("Inconsistent addition\n"));
		}
		break;
		case 9:
		{
			displayAndStoreError(_("Inconsistent subtraction\n"));
		}
		break;
		case 10:
		{
			displayAndStoreError(_("Inconsistent multiplication\n"));
		}
		break;
		case 11:
		{
			displayAndStoreError(_("Inconsistent right division \n"));
		}
		break;
		case 12:
		{
			displayAndStoreError(_("Inconsistent left division\n"));
		}
		break;
		case 13:
		{
			displayAndStoreError(_("Redefining permanent variable\n"));
		}
		break;
		case 14:
		{
			displayAndStoreError(_("Eye variable undefined in this context\n"));
		}
		break;
		case 15:
		{
			displayAndStoreError(_("Submatrix incorrectly defined\n"));
		}
		break;
		case 16:
		{
			displayAndStoreError(_("Incorrect command!\n"));
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 17:
		{
			int Memory_used_for_variables = 0;
			int Intermediate_Memory = 0;
			int Total_Memory_available = 0;

			Intermediate_Memory = getIntermediateMemoryNeeded();
			C2F(getstackinfo)(&Total_Memory_available,&Memory_used_for_variables);

			displayAndStoreError(_("stack size exceeded!\n"));
			displayAndStoreError(_("Use stacksize function to increase it.\n"));
			displayAndStoreError(_("Memory used for variables : %d\n"),Memory_used_for_variables);
			displayAndStoreError(_("Intermediate memory needed: %d\n"),Intermediate_Memory);
			displayAndStoreError(_("Total  memory available   : %d\n"),Total_Memory_available);
		}
		break;
		case 18:
		{
			displayAndStoreError(_("Too many variables!\n"));
		}
		break;
		case 19:
		{
			displayAndStoreError(_("Problem is singular\n"));
		}
		break;
		case 20:
		{
			if (Err == 1)
			{
				displayAndStoreError(_("first argument must be square matrix.\n"));
			}
			else
			{
				displayAndStoreError(_("%dth argument must be square matrix.\n"),Err);
			}
		}
		break;
		case 21:
		{
			displayAndStoreError(_("Invalid index\n"));
		}
		break;
		case 22:
		{
			displayAndStoreError(_("Recursion problems. Sorry....\n"));
		}
		break;
		case 23:
		{
			displayAndStoreError(_("Matrix norms available are 1, 2, inf, and fro\n"));
		}
		break;
		case 24:
		{
			displayAndStoreError(_("Convergence problem...\n"));
		}
		break;
		case 25:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("bad call to primitive : %s\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 26:
		{
			displayAndStoreError(_("too complex recursion! (recursion tables are full))"));
			/* break recursion */
			C2F(recu).pt = Min(C2F(recu).pt,psiz);
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 27:
		{
			displayAndStoreError(_("Division by zero...\n"));
		}
		break;
		case 28:
		{
			displayAndStoreError(_("Empty function...\n"));
		}
		break;
		case 29:
		{
			displayAndStoreError(_("Matrix is not positive definite\n"));
		}
		break;
		case 30:
		{
			displayAndStoreError(_("Invalid exponent\n"));
		}
		break;
		case 31:
		{
			displayAndStoreError(_( "incorrect string\n"));
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 32:
		{
			displayAndStoreError(_("Singularity of log or tan function\n"));
		}
		break;
		case 33:
		{
			displayAndStoreError(_("Too many :\n"));
		}
		break;
		case 34:
		{
			displayAndStoreError(_("incorrect control intruction syntax.\n"));
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 35:
		{
			switch (C2F(recu).ids[C2F(recu).pt * 6 - 6]) /* find instruction type */
			{
				case 1: /* if */
					displayAndStoreError(_("Syntax error in an if instruction.\n"));
				break;

				case 2: /* while */
					displayAndStoreError(_("Syntax error in a while instruction.\n"));
				break;

				default: /* select/case */
					displayAndStoreError(_("Syntax error in a select/case instruction.\n"));
				break;
			}
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 36:
		{
			if (Err == 1)
			{
				displayAndStoreError(_("first argument is incorrect.\n"));
			}
			else
			{
				displayAndStoreError(_("%dth argument is incorrect here.\n"),Err);
			}
		}
		break;
		case 37:
		{
			displayAndStoreError(_("incorrect function at line %d.\n"),Err);
		}
		break;
		case 38:
		{
			displayAndStoreError(_("File name incorrect\n"));
		}
		break;
		case 39:
		{
			displayAndStoreError(_("Incorrect number of arguments\n"));
		}
		break;
		case 40:
		{
			displayAndStoreError(_("waiting for end of command.\n"));
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 41:
		{
			displayAndStoreError(_("Incompatible output parameter (LHS)\n"));
		}
		break;
		case 42:
		{
			displayAndStoreError(_("Incompatible input parameter (RHS)\n"));
		}
		break;
		case 43:
		{
			displayAndStoreError(_("Not implemented in scilab....\n"));
		}
		break;
		case 44:
		{
			if (Err == 1)
			{
				displayAndStoreError(_("first argument is incorrect.\n"));
			}
			else
			{
				displayAndStoreError(_("%dth argument is incorrect.\n"),Err);
			}

		}
		break;
		case 45:
		{
			displayAndStoreError(_("null matrix (argument # %d).\n"),Err);
		}
		break;
		case 46:
		{
			displayAndStoreError(_("incorrect syntax.\n"));
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 47:
		{
			displayAndStoreError(_(" end or else is missing...\n"));
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 48:
		{
			displayAndStoreError(_(" input line longer than buffer size: %d\n"),bsiz);
		}
		break;
		case 49:
		{
			displayAndStoreError(_("Incorrect file or format\n"));
		}
		break;
		case 50:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_3);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("subroutine not found : %s\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 51:
		{
			/* no message  */
		}
		break;
		case 52:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument must be a real matrix.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument must be a real matrix.\n"));
			}
		}
		break;
		case 53:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth input is invalid (waiting for real or complex matrix.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("invalid input (waiting for real or complex matrix.\n"));
			}
		}
		break;
		case 54:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument type must be polynomial.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument type must be polynomial.\n"));
			}
		}
		break;
		case 55:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument type must be character string.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument type must be  character string.\n"));
			}
		}
		break;
		case 56:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument must be a list.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument must be a list.\n"));
			}
		}
		break;
		case 57:
		{
			displayAndStoreError(_("Problem with comparison symbol...\n"));
		}
		break;
		case 58:
		{
			if (Rhs == 0)
			{
				displayAndStoreError(_("function has no input argument...\n"));
			}
			else
			{
				displayAndStoreError(_("incorrect number of arguments in function call...\n"));
				displayAndStoreError(_("arguments are :\n"));
				/* print variables name on stack :( */
				C2F(prntid)(&((integer *)&C2F(stack))[C2F(recu).pstk[C2F(recu).pt - 1] - 1], &Rhs, &C2F(iop).wte);
			}

		}
		break;
		case 59:
		{
			if (Lhs == 0)
			{
				displayAndStoreError(_("function has no output\n"));
			}
			else
			{
				displayAndStoreError(_("incorrect # of outputs in the function\n"));
				displayAndStoreError(_("arguments are :\n"));
				/* print variables name on stack :( */
				C2F(prntid)(&((integer *)&C2F(stack))[C2F(recu).pstk[C2F(recu).pt - 1] - 1], &Lhs, &C2F(iop).wte);
			}
		}
		break;
		case 60:
		{
			displayAndStoreError(_("Argument with incompatible dimensions\n"));
		}
		break;
		case 61:
		{
			displayAndStoreError(_("Direct access : give format\n"));
		}
		break;
		case 62:
		{
			displayAndStoreError(_("end of file at line %d\n"),Err);
		}
		break;
		case 63:
		{
			displayAndStoreError(_("%d graphic terminal?\n"),Err);
		}
		break;
		case 64:
		{
			displayAndStoreError(_("Integration fails\n"));
		}
		break;
		case 65:
		{
			displayAndStoreError(_("%d : logical unit already used\n"),Err);
		}
		break;
		case 66:
		{
			displayAndStoreError(_("No more logical units available!\n"));
		}
		break;
		case 67:
		{
			displayAndStoreError(_("Unknown file format.\n"));
		}
		break;
		case 68:
		{
			displayAndStoreError(_("fatal error!!! your variables are saved in file : %s\n"),get_sci_data_strings(SAVE_ID));
			displayAndStoreError(_("bad call to a scilab function ?\n"));
			displayAndStoreError(_("check otherwise send a bug report to :\n"));
			displayAndStoreError(_("http://www.scilab.org/cgi-bin/bugzilla_bug_II/index.cgi\n"));
		}
		break;
		case 69:
		{
			displayAndStoreError(_("Floating point exception\n"));
		}
		break;
		case 70:
		{
			displayAndStoreError(_("Too many arguments in fort (max 30)\n"));
		}
		break;
		case 71:
		{
			displayAndStoreError(_("This variable is not valid in fort\n"));
		}
		break;
		case 72:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("%s is not valid in this context.\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 73:
		{
			displayAndStoreError(_("Error while linking\n"));
		}
		break;
		case 74:
		{
			displayAndStoreError(_("Leading coefficient is zero\n"));
		}
		break;
		case 75:
		{
			displayAndStoreError(_("Too high degree (max 100)\n"));
		}
		break;
		case 76:
		{
			displayAndStoreError(_("for x=val with type(val)=%d  is not implemented in Scilab.\n"),Err);
		}
		break;
		case 77:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("%s  : wrong number of rhs arguments.\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 78:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("%s  :wrong number of lhs arguments.\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 79:
		{
			/* no message  */
		}
		break;
		case 80:
		{
			displayAndStoreError(_("incorrect function (argument n: %d) \n"),Err);
		}
		break;
		case 81:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a real or complex matrix.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 82:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a real matrix.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 83:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a real vector.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 84:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a scalar.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 85:
		{
			displayAndStoreError(_("Host does not answer...\n"));
		}
		break;
		case 86:
		{
			displayAndStoreError(_("Uncontrollable system.\n"));
		}
		break;
		case 87:
		{
			displayAndStoreError(_("Unobservable system\n"));
		}
		break;
		case 88:
		{
			displayAndStoreError(_("sfact : singular or assymetric problem.\n"));
		}
		break;
		case 89:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument has incorrect dimensions.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument has incorrect dimensions.\n"));
			}
		}
		break;
		case 90:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument must be a transfer matrix.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument must be a transfer matrix.\n"));
			}
		}
		break;
		case 91:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument must be in state space form.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument must be in state space form.\n"));
			}
		}
		break;
		case 92:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument must be a rational matrix.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument must be a rational matrix.\n"));
			}
		}
		break;
		case 93:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument must be in continuous time.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("waiting for continuous time...\n"));
			}
		}
		break;
		case 94:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument must be in discrete time.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument must be in discrete time.\n"));
			}
		}
		break;
		case 95:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument must be SISO.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument must be SISO.\n"));
			}
		}
		break;
		case 96:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("time domain of %dth argument is not defined.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("time domain of argument is not defined.\n"));
			}
		}
		break;
		case 97:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument must be a system in state space or transfer matrix form.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("input argument must be a system in state space or transfer matrix form.\n"));
			}

		}
		break;
		case 98:
		{
			displayAndStoreError(_("variable returned by scilab argument function is incorrect.\n"));
		}
		break;
		case 99:
		{
			if (Err != 1)
			{
				displayAndStoreError(_(" elements of %dth th must be in increasing order!.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("elements of first argument are not (strictly) increasing.\n"));
			}
		}
		break;
		case 100:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("the elements of %dth argument are not in (strictly) decreasing order.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("elements of first argument are not in (strictly) decreasing order.\n"));
			}
		}
		break;
		case 101:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("last element of %dth argument <>  first.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("last element of first argument does not matches the first one.\n"));
			}
		}
		break;
		case 102:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("variable or function %s is not in file.\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 103:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_3);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("variable %s is not a valid rational function.\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 104:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_3);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("variable %s is not a valid state space representation.\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 105:
		{
			displayAndStoreError(_("Undefined fonction.\n"));
		}
		break;
		case 106:
		{
			displayAndStoreError(_("Function name already used.\n"));
		}
		break;
		case 107:
		{
			displayAndStoreError(_("too many functions are defined (maximum #:%d).\n"),Err);
		}
		break;
		case 108:
		{
			displayAndStoreError(_("too complex for scilab, may be a too long control instruction.\n"));
		}
		break;
		case 109:
		{
			displayAndStoreError(_("Too large, cant be displayed.\n"));
		}
		break;
		case 110:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("%s was a function when compiled but is now a primitive!\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 111:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_2);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("trying to re-define function %s .\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 112:
		{
			displayAndStoreError(_("Cannot allocate more memory.\n"));
		}
		break;
		case 113:
		{
			displayAndStoreError(_("Too large string.\n"));
		}
		break;
		case 114:
		{
			displayAndStoreError(_("Too many linked routines.\n"));
		}
		break;
		case 115:
		{
			displayAndStoreError(_("Stack problem detected within a loop.\n"));
			displayAndStoreError(_("A primitive function has been called with wrong number of lhs arguments.\n"));
			displayAndStoreError(_("No lhs test made for this function.\n"));
			displayAndStoreError(_("Please report this bug :\n"));
			displayAndStoreError(_("http://www.scilab.org/cgi-bin/bugzilla_bug_II/index.cgi\n"));

			C2F(showstack)(); /* display of calling tree */
		}
		break;
		case 116:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument has incorrect value.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("first argument has incorrect value.\n"));
			}
		}
		break;
		case 117:
		{
			displayAndStoreError(_("list element number %d is Undefined.\n"),Err);
		}
		break;
		case 118:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument must be a named variable not an expression.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument must be a named variable not an expression.\n"));
			}
		}
		break;
		case 119:
		{
			/* no message  */
		}
		break;
		case 120:
		{
			displayAndStoreError(_("Indices for non-zero elements must be given by a 2 column matrix.\n"));
		}
		break;
		case 121:
		{
			displayAndStoreError(_("Incompatible indices for non-zero elements.\n"));
		}
		break;
		case 122:
		{
			displayAndStoreError(_("logical unit number should be larger than %d.\n"),Err);
		}
		break;
		case 123:
		{
			displayAndStoreError(_("Function not bounded from below.\n"));
		}
		break;
		case 124:
		{
			/* no message  */
		}
		break;
		case 125:
		{
			displayAndStoreError(_("Problem may be unbounded : too high distance between two consecutive iterations.\n"));
		}
		break;
		case 126:
		{
			displayAndStoreError(_("Inconsistent constraints.\n"));
		}
		break;
		case 127:
		{
			displayAndStoreError(_("No feasible solution.\n"));
		}
		break;
		case 128:
		{
			displayAndStoreError(_("Degenerate starting point.\n"));
		}
		break;
		case 129:
		{
			displayAndStoreError(_("No feasible point has been found.\n"));
		}
		break;
		case 130:
		{
			displayAndStoreError(_("Optimization fails: back to initial point.\n"));
		}
		break;
		case 131:
		{
			displayAndStoreError(_("Optim: stop requested by simulator (ind=0)\n"));
		}
		break;
		case 132:
		{
			displayAndStoreError(_("Optim: incorrect input parameters.\n"));
		}
		break;
		case 133:
		{
			displayAndStoreError(_("Too small memory.\n"));
		}
		break;
		case 134:
		{
			displayAndStoreError(_("Optim: problem with initial constants in simul.\n"));
		}
		break;
		case 135:
		{
			displayAndStoreError(_("Optim : bounds and initial guess are incompatible.\n"));
		}
		break;
		case 136:
		{
			displayAndStoreError(_("Optim : this method is NOT implemented.\n"));
		}
		break;
		case 137:
		{
			displayAndStoreError(_("NO hot restart available in this method.\n"));
		}
		break;
		case 138:
		{
			displayAndStoreError(_("Optim : incorrect stopping parameters.\n"));
		}
		break;
		case 139:
		{
			displayAndStoreError(_("Optim : incorrect bounds.\n"));
		}
		break;
		case 140:
		{
			/* no message  */
		}
		break;
		case 141:
		{
			displayAndStoreError(_("incorrect function (argument n:%d).\n"),Err);
		}
		break;
		case 142:
		{
			displayAndStoreError(_("hot restart : dimension of working table (argument n:%d).\n"),Err);
		}
		break;
		case 143:
		{
			displayAndStoreError(_("Optim : df0 must be positive !\n"));
		}
		break;
		case 144:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Undefined operation for the given operands check or define function %s for overloading.\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;

		/* error 145 to 199  not used */

		/*
		messages from 201 to 203 and 205 to 214 are no more used by error
		(see Scierror  in stack1.c)
		*/
		case 200:
		{
			/* no message  */
		}
		break;
		case 201:
		{
			/* not used  */
			/* only for compatibility */
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a real or complex matrix.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 202:
		{
			/* not used  */
			/* only for compatibility */
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a real matrix.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 203:
		{
			/* not used  */
			/* only for compatibility */
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a real vector.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 204:
		{
			/* not used  */
			/* only for compatibility */
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a scalar.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 205:
		{
			int minvalue = C2F(recu).pstk[C2F(recu).pt];
			int maxvalue = C2F(recu).pstk[C2F(recu).pt + 1];

			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong matrix size %d,%d) expected.\n"),Err,NameVarOnStack,minvalue,maxvalue);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 206:
		{
			int vectorsize = C2F(recu).pstk[C2F(recu).pt];

			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong vector size (%d) expected.\n"),Err,NameVarOnStack,vectorsize);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 207:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a matrix of strings.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 208: 
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a boolean matrix.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 209:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a matrix.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 210:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a list.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 211:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a function or string (external function).\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 212:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a polynomial.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 213:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a working integer matrix.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 214:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_4);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("Argument %d of %s: wrong type argument, expecting a  vector.\n"),Err,NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 215:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument type must be boolean.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument type must be boolean.\n"));
			}
		}
		break;
		case 216:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument type must be boolean or scalar.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument type must be boolean or scalar.\n"));
			}
		}
		break;
		case 217:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument  must be a sparse matrix of scalars.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument type must be a sparse matrix of scalars.\n"));
			}
		}
		break;
		case 218:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument  must be a  handle to sparse lu factors.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument type must be a  handle to sparse lu factors.\n"));
			}
		}
		break;
		case 219:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument  must be a sparse or full scalar matrix.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument type must be a sparse or full scalar matrix.\n"));
			}
		}
		break;
		case 220:
		{
			displayAndStoreError(_("Null variable cannot be used here.\n"));
		}
		break;
		case 221:
		{
			displayAndStoreError(_("A sparse matrix entry is defined with two differents values.\n"));
		}
		break;
		case 222:
		{
			displayAndStoreError(_("lusolve not yet implemented for full input parameter (RHS)\n"));
		}
		break;
		case 223:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("It is not possible to redefine the %s primitive this way (see clearfun).\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 224:
		{
			displayAndStoreError(_("Type data base is full.\n"));
		}
		break;
		case 225:
		{
			displayAndStoreError(_("This data type is already defined.\n"));
		}
		break;
		case 226:
		{
			displayAndStoreError(_("Inequality comparison with empty matrix.\n"));
		}
		break;
		case 227:
		{
			displayAndStoreError(_("Missing index.\n"));
		}
		break;
		case 228:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("reference to the cleared global variable %s.\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 229:
		{
			/* no message  */
		}
		break;
		case 230:
		{
			displayAndStoreError(_("semi def fails.\n"));
		}
		break;
		case 231:
		{
			displayAndStoreError(_("First argument must be a single string.\n"));
		}
		break;
		case 232:
		{
			displayAndStoreError(_("Entry name not found.\n"));
		}
		break;
		case 233:
		{
			displayAndStoreError(_("Maximum number of dynamic interfaces reached.\n"));
		}
		break;
		case 234:
		{
			displayAndStoreError(_("Link: expecting more than one argument.\n"));
		}
		break;
		case 235:
		{
			displayAndStoreError(_("Link: problem with one of the entry point.\n"));
		}
		break;
		case 236:
		{
			displayAndStoreError(_("Link: the shared archive was not loaded.\n"));
		}
		break;
		case 237:
		{
			displayAndStoreError(_("link: Only one entry point allowed on this operating system.\n"));
		}
		break;
		case 238:
		{
			displayAndStoreError(_("Link: First argument cannot be a number.\n"));
		}
		break;
		case 239:
		{
			displayAndStoreError(_("You cannot link more functions, maxentry reached.\n"));
		}
		break;
		case 240:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_3);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("File %s already exists or directory write access denied.\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 241:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_3);
			if (NameVarOnStack)
			{
				displayAndStoreError(_("File %s does not exist or read access denied.\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 242:
		{
			displayAndStoreError(_("binary direct acces files must be opened by 'file'.\n"));
		}
		break;
		case 243:
		{
			displayAndStoreError(_("C file logical unit not allowed here.\n"));
		}
		break;
		case 244:
		{
			displayAndStoreError(_("Fortran file logical unit not allowed here.\n"));
		}
		break;
		case 245:
		{
			displayAndStoreError(_("No input file associated to logical unit %d.\n"),Err);
		}
		break;
		case 246:
		{
			char *NameVarOnStack = getConvertedNameFromStack(CVNAME_READING_TYPE_1);
			displayAndStoreError(_("function not defined for given argument type(s),\n"));
			if (NameVarOnStack)
			{
				displayAndStoreError(_("  check arguments or define function %s for overloading.\n"),NameVarOnStack);
				FREE(NameVarOnStack);
				NameVarOnStack = NULL;
			}
		}
		break;
		case 247:
		{
			/* no message  */
		}
		break;
		case 248:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument is not a valid variable name.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument is not a valid variable name.\n"));
			}
		}
		break;
		case 249:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument must not be an empty string.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument must not be an empty string.\n"));
			}
		}
		break;
		case 250:
		{
			displayAndStoreError(_("Recursive extraction is not valid in this context.\n"));
		}
		break;
		case 251:
		{
			displayAndStoreError(_("bvode: ipar dimensioned at least 11.\n"));
		}
		break;
		case 252:
		{
			displayAndStoreError(_("bvode: ltol must be of size ipar(4).\n"));
		}
		break;
		case 253:
		{
			displayAndStoreError(_("bvode: fixpnt must be of size ipar(11).\n"));
		}
		break;
		case 254:
		{
			displayAndStoreError(_("bvode: ncomp < 20 requested.\n"));
		}
		break;
		case 255:
		{
			displayAndStoreError(_("bvode: m must be of size ncomp.\n"));
		}
		break;
		case 256: case 257:
		{
			displayAndStoreError(_("bvode: sum(m) must be less than 40.\n"));
		}
		break;
		case 258:
		{
			displayAndStoreError(_("bvode: input data error.\n"));
		}
		break;
		case 259:
		{
			displayAndStoreError(_("bvode: no. of subintervals exceeds storage.\n"));
		}
		break;
		case 260:
		{
			displayAndStoreError(_("bvode: Th colocation matrix is singular.\n"));
		}
		break;
		case 261:
		{
			displayAndStoreError(_("Interface property table is full.\n"));
		}
		break;
		case 262:
		{
			int maxglobalvariables = isizt - C2F(vstk).isiz - 1;
			displayAndStoreError(_("Too many global variables! , max number is %d.\n"),maxglobalvariables);
		}
		break;
		case 263:
		{
			displayAndStoreError(_("Error while writing in file,(disk full or deleted file.\n"));
		}
		break;
		case 264:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument must not contain NaN or Inf.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument must not  contain NaN or Inf.\n"));
			}
		}
		break;
		case 265:
		{
			displayAndStoreError(_("A and B must have equal number of rows.\n"));
		}
		break;
		case 266:
		{
			displayAndStoreError(_("A and B must have equal number of columns.\n"));
		}
		break;
		case 267:
		{
			displayAndStoreError(_("A and B must have equal dimensions.\n"));
		}
		break;
		case 268:
		{
			displayAndStoreError(_("invalid return value for function passed in arg %d.\n"),Err);
		}
		break;
		case 269:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument eigenvalues must have negative real parts.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument eigenvalues must have negative real parts.\n"));
			}
		}
		break;
		case 270:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("%dth argument eigenvalues modulus must be less than one.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("argument eigenvalues modulus must be less than one.\n"));
			}
		}
		break;
		case 271:
		{
			if (Err != 1)
			{
				displayAndStoreError(_("Size varying argument a*eye(), (arg %d) not allowed here.\n"),Err);
			}
			else
			{
				displayAndStoreError(_("Size varying argument a*eye() not allowed here.\n"));
			}
		}
		break;
		case 272:
		{
			displayAndStoreError(_("endfunction is missing.\n"));
		}
		break;
		case 273:
		{
			displayAndStoreError(_("Instruction left hand side: waiting for a dot or a left parenthesis.\n"));
		}
		break;
		case 274:
		{
			displayAndStoreError(_("Instruction left hand side: waiting for a name.\n"));
		}
		break;
		case 275:
		{
			displayAndStoreError(_("varargout keyword cannot be used here.\n"));
		}
		break;
		case 276:
		{
			displayAndStoreError(_("Missing operator, comma, or semicolon.\n"));
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 277:
		{
			displayAndStoreError(_("Too many commands defined.\n"));
		}
		break;
		case 278: case 279: case 280:
		{
			/* no message  */
		}
		break;

		default:
		{
			integer bufl;
			char *buffer = NULL;
			/* message d'erreur soft */
			/* Bug 1422 corrected - Francois VOGEL June 2006 */
			bufl = 1;
			while(*(unsigned char *)&C2F(cha1).buf[bufl - 1] != '\0' && bufl < 80)
			{
				++bufl;
			}
			--bufl;
			/* remove blank */
			buffer = (char*)MALLOC((strlen(C2F(cha1).buf)+1)*sizeof(char));
			if (buffer)
			{
				strcpy(buffer,C2F(cha1).buf);
				strip_blank(buffer);
				displayAndStoreError(buffer);
				FREE(buffer);
			}

		}
		break;
    }
		return 0;
}
/*-----------------------------------------------------------------------------------*/
static void strip_blank(char *source)
{
	char *p;
	p = source;
	/* look for end of string */
	while(*p != '\0') p++;
	while(p != source)
	{
		p--;
		if(*p != ' ') break;
		*p = '\0';
	}
}
/*-----------------------------------------------------------------------------------*/
static char *getConvertedNameFromStack(int cvnametype)
{
	int one = 1;
	int lenString = 0;

	char *returnedString = NULL;
	char local_variable_buffer[bsiz];
	
	switch (cvnametype)
	{
	case CVNAME_READING_TYPE_1 : 
		{
			C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6], C2F(cha1).buf, &one, bsiz);
			strncpy(local_variable_buffer, C2F(cha1).buf,bsiz);
			local_variable_buffer[bsiz-1] = '\0';
		}
		break;

	case CVNAME_READING_TYPE_2 :
		{
			C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6], local_variable_buffer, &one, nlgh);
			local_variable_buffer[nlgh] = '\0';
		}
		break;

	case CVNAME_READING_TYPE_3 :
		{
			C2F(cvname)(&C2F(recu).ids[C2F(recu).pt * 6 - 6], C2F(cha1).buf, &one, bsiz);
			strncpy(local_variable_buffer, C2F(cha1).buf,bsiz);
			local_variable_buffer[bsiz-1] = '\0';
		}
		break;

	case CVNAME_READING_TYPE_4 :
		{
			cvname_(&C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6], C2F(cha1).buf + 3, &one, nlgh+1);
			strncpy(local_variable_buffer, C2F(cha1).buf+3,nlgh);
			local_variable_buffer[nlgh] = '\0';
		}
		break;

	default:
		strcpy(local_variable_buffer,_("\"unknown data\""));
		break;
	}

	strip_blank(local_variable_buffer);

	lenString = (int)strlen(local_variable_buffer);
	if (lenString > 0)
	{
		returnedString = (char*)MALLOC(sizeof(char)*(lenString+1));
		strcpy(returnedString,local_variable_buffer);
	}

	return returnedString ;
}
/*-----------------------------------------------------------------------------------*/
static void displayAndStoreError(const char *msg,...)
{
	int len = 0;

	va_list ap;
	static char s_buf[bsiz];

	va_start(ap,msg);

#if defined(linux) || defined(_MSC_VER)
	{
		int count=0;
		count= vsnprintf(s_buf,bsiz, msg, ap );
		if (count == -1)
		{
			s_buf[bsiz-1]='\0';
		}
	}
#else
	(void )vsprintf(s_buf, msg, ap );
#endif
	va_end(ap);


	/* store error (lasterror) */
	len = (int)strlen(s_buf);
	C2F(msgstore)(s_buf,&len);

	/* display error */
	sciprint(s_buf);
}
/*-----------------------------------------------------------------------------------*/
static void resetLastError(void)
{
	#define SPACE_CHAR " "
	int zero = 0;
	int lenspace = (int)strlen(SPACE_CHAR);

	/* reset lasterror */
	C2F(linestore)(&zero);
	C2F(funnamestore)(SPACE_CHAR, &zero, lenspace);
	C2F(freemsgtable)();
}
/*-----------------------------------------------------------------------------------*/

