/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 - 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
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
static void msgout_string(const char *msgformat);
static void displayAndStoreError(const char *msg);
static void resetLastError(void);
/*-----------------------------------------------------------------------------------*/
int C2F(errmsg)(integer *n,integer *errtyp)
{
	resetLastError();
	
	/* store error code (lasterror) */
	C2F(errstore)(n);

	*errtyp = 0; /* by default errors are recoverable */

    switch ((int)*n)
    {
		case 1:
		{
			sciprint(_("Incorrect assignment\n"));

		}
		break;
		case 2:
		{
			sciprint(("Invalid factor\n"));
			*errtyp = 1;
		}
		break;
		case 3:
		{
			sciprint(_("Waiting for right parenthesis\n"));
			*errtyp = 1;
		}
		break;
		case 4:
		{
			msgout_string(_("Undefined variable : %s"));
		}
		break;
		case 5:
		{
			sciprint(_("Inconsistent column/row dimensions\n"));
		}
		break;
		case 6:
		{
			sciprint(_("Inconsistent row/column dimensions\n"));
		}
		break;
		case 7:
		{
			sciprint(_("Dot cannot be used as modifier for this operator\n"));
		}
		break;
		case 8:
		{
			sciprint(_("Inconsistent addition\n"));
		}
		break;
		case 9:
		{
			sciprint(_("Inconsistent subtraction\n"));
		}
		break;
		case 10:
		{
			sciprint(_("Inconsistent multiplication\n"));
		}
		break;
		case 11:
		{
			sciprint(_("Inconsistent right division \n"));
		}
		break;
		case 12:
		{
			sciprint(_("Inconsistent left division\n"));
		}
		break;
		case 13:
		{
			sciprint(_("Redefining permanent variable\n"));
		}
		break;
		case 14:
		{
			sciprint(_("Eye variable undefined in this context\n"));
		}
		break;
		case 15:
		{
			sciprint(_("Submatrix incorrectly defined\n"));
		}
		break;
		case 16:
		{
			sciprint(_("Incorrect command!\n"));
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

			sciprint(_("stack size exceeded!\n"));
			sciprint(_("Use stacksize function to increase it)\n"));
			sciprint(_("Memory used for variables : %d\n"),Memory_used_for_variables);
			sciprint(_("Intermediate memory needed: %d\n"),Intermediate_Memory);
			sciprint(_("Total  memory available   : %d\n"),Total_Memory_available);
		}
		break;
		case 18:
		{
			sciprint(_("Too many variables!\n"));
		}
		break;
		case 19:
		{
			sciprint(_("Problem is singular\n"));
		}
		break;
		case 20:
		{
			if (Err == 1)
			{
				sciprint(_("first argument must be square matrix.\n"));
			}
			else
			{
				sciprint(_("%dth argument must be square matrix.\n"),Err);
			}
		}
		break;
		case 21:
		{
			sciprint(_("Invalid index\n"));
		}
		break;
		case 22:
		{
			sciprint(_("Recursion problems. Sorry....\n"));
		}
		break;
		case 23:
		{
			sciprint(_("Matrix norms available are 1, 2, inf, and fro\n"));
		}
		break;
		case 24:
		{
			sciprint(_("Convergence problem...\n"));
		}
		break;
		case 25:
		{
			msgout_string(_("bad call to primitive : %s"));
		}
		break;
		case 26:
		{
			sciprint(_("too complex recursion! (recursion tables are full))"));
			/* break recursion */
			C2F(recu).pt = Min(C2F(recu).pt,4096);
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 27:
		{
			sciprint(_("Division by zero...\n"));
		}
		break;
		case 28:
		{
			sciprint(_("Empty function...\n"));
		}
		break;
		case 29:
		{
			sciprint(_("Matrix is not positive definite\n"));
		}
		break;
		case 30:
		{
			sciprint(_("Invalid exponent\n"));
		}
		break;
		case 31:
		{
			sciprint(_( "incorrect string\n"));
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 32:
		{
			sciprint(_("Singularity of log or tan function\n"));
		}
		break;
		case 33:
		{
			sciprint(_("Too many :\n"));
		}
		break;
		case 34:
		{
			sciprint(_("incorrect control intruction syntax.\n"));
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 35:
		{
			switch (C2F(recu).ids[C2F(recu).pt * 6 - 6]) /* find instruction type */
			{
				case 1: /* if */
					sciprint(_("Syntax error in an if instruction.\n"));
				break;

				case 2: /* while */
					sciprint(_("Syntax error in a while instruction.\n"));
				break;

				default: /* select/case */
					sciprint(_("Syntax error in a select/case instruction.\n"));
				break;
			}
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 36:
		{
			if (Err == 1)
			{
				sciprint(_("first argument is incorrect.\n"));
			}
			else
			{
				sciprint(_("%dth argument is incorrect here.\n"),Err);
			}
		}
		break;
		case 37:
		{
			sciprint(_("incorrect function at line %d.\n"),Err);
		}
		break;
		case 38:
		{
			sciprint(_("File name incorrect\n"));
		}
		break;
		case 39:
		{
			sciprint(_("Incorrect number of arguments\n"));
		}
		break;
		case 40:
		{
			sciprint(_("waiting for end of command.\n"));
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 41:
		{
			sciprint(_("Incompatible output parameter (LHS)\n"));
		}
		break;
		case 42:
		{
			sciprint(_("Incompatible input parameter (RHS)\n"));
		}
		break;
		case 43:
		{
			sciprint(_("Not implemented in scilab....\n"));
		}
		break;
		case 44:
		{
			if (Err == 1)
			{
				sciprint(_("first argument is incorrect.\n"));
			}
			else
			{
				sciprint(_("%dth argument is incorrect.\n"),Err);
			}

		}
		break;
		case 45:
		{
			sciprint(_("null matrix (argument # %d).\n"),Err);
		}
		break;
		case 46:
		{
			sciprint(_("incorrect syntax.\n"));
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 47:
		{
			sciprint(_(" end or else is missing...\n"));
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 48:
		{
			sciprint(_(" input line longer than buffer size: %d\n"),bsiz);
		}
		break;
		case 49:
		{
			sciprint(_("Incorrect file or format\n"));
		}
		break;
		case 50:
		{
			msgout_string(_("subroutine not found : %s"));
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
				sciprint(_("%dth argument must be a real matrix.\n"),Err);
			}
			else
			{
				sciprint(_("argument must be a real matrix.\n"));
			}
		}
		break;
		case 53:
		{
			if (Err != 1)
			{
				sciprint(_("%dth input is invalid (waiting for real or complex matrix.\n"),Err);
			}
			else
			{
				sciprint(_("invalid input (waiting for real or complex matrix.\n"));
			}
		}
		break;
		case 54:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument type must be polynomial.\n"),Err);
			}
			else
			{
				sciprint(_("argument type must be polynomial.\n"));
			}
		}
		break;
		case 55:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument type must be character string.\n"),Err);
			}
			else
			{
				sciprint(_("argument type must be  character string.\n"));
			}
		}
		break;
		case 56:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument must be a list.\n"),Err);
			}
			else
			{
				sciprint(_("argument must be a list.\n"));
			}
		}
		break;
		case 57:
		{
			sciprint(_("Problem with comparison symbol...\n"));
		}
		break;
		case 58:
		{
			if (Rhs == 0)
			{
				sciprint(_("function has no input argument...\n"));
			}
			else
			{
				sciprint(_("incorrect number of arguments in function call...\n"));
				sciprint(_("arguments are :\n"));
				/* print variables name on stack :( */
				C2F(prntid)(&((integer *)&C2F(stack))[C2F(recu).pstk[C2F(recu).pt - 1] - 1], &Rhs, &C2F(iop).wte);
			}

		}
		break;
		case 59:
		{
			if (Lhs == 0)
			{
				sciprint(_("function has no output\n"));
			}
			else
			{
				sciprint(_("incorrect # of outputs in the function\n"));
				sciprint(_("arguments are :\n"));
				/* print variables name on stack :( */
				C2F(prntid)(&((integer *)&C2F(stack))[C2F(recu).pstk[C2F(recu).pt - 1] - 1], &Lhs, &C2F(iop).wte);
			}
		}
		break;
		case 60:
		{
			sciprint(_("Argument with incompatible dimensions\n"));
		}
		break;
		case 61:
		{
			sciprint(_("Direct acces : give format\n"));
		}
		break;
		case 62:
		{
			sciprint(_("end of file at line %d\n"),Err);
		}
		break;
		case 63:
		{
			sciprint(_("%d graphic terminal?\n"),Err);
		}
		break;
		case 64:
		{
			sciprint(_("Integration fails\n"));
		}
		break;
		case 65:
		{
			sciprint(_("%d : logical unit already used\n"),Err);
		}
		break;
		case 66:
		{
			sciprint(_("No more logical units available!\n"));
		}
		break;
		case 67:
		{
			sciprint(_("Unknown file format \n"));
		}
		break;
		case 68:
		{
			sciprint(_("fatal error!!! your variables are saved in file : %s\n"),get_sci_data_strings(SAVE_ID));
			sciprint(_("bad call to a scilab function ?\n"));
			sciprint(_("check otherwise send a bug report to :\n"));
			sciprint(_("http://www.scilab.org/cgi-bin/bugzilla_bug_II/index.cgi\n"));
		}
		break;
		case 69:
		{
			sciprint(_("Floating point exception\n"));
		}
		break;
		case 70:
		{
			sciprint(_("Too many arguments in fort (max 30)\n"));
		}
		break;
		case 71:
		{
			sciprint(_("This variable is not valid in fort\n"));
		}
		break;
		case 72:
		{
			msgout_string(_("%s is not valid in this context.\n"));
		}
		break;
		case 73:
		{
			sciprint(_("Error while linking\n"));
		}
		break;
		case 74:
		{
			sciprint(_("Leading coefficient is zero\n"));
		}
		break;
		case 75:
		{
			sciprint(_("Too high degree (max 100)\n"));
		}
		break;
		case 76:
		{
			sciprint(_("for x=val with type(val)=%d  is not implemented in Scilab.\n"),Err);
		}
		break;
		case 77:
		{
			msgout_string(_("%s  : wrong number of rhs arguments.\n"));
		}
		break;
		case 78:
		{
			msgout_string(_("%s  :wrong number of lhs arguments.\n"));
		}
		break;
		case 79:
		{
			/* no message  */
		}
		break;
		case 80:
		{
			sciprint(_("incorrect function (argument n: %d) \n"),Err);
		}
		break;
		case 81:
		{
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a real or complex matrix\n"));
		}
		break;
		case 82:
		{
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a real matrix\n"));
		}
		break;
		case 83:
		{
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a real vector\n"));
		}
		break;
		case 84:
		{
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a scalar\n"));
		}
		break;
		case 85:
		{
			sciprint(_("Host does not answer...\n"));
		}
		break;
		case 86:
		{
			sciprint(_("Uncontrollable system\n"));
		}
		break;
		case 87:
		{
			sciprint(_("Unobservable system\n"));
		}
		break;
		case 88:
		{
			sciprint(_("sfact : singular or assymetric problem\n"));
		}
		break;
		case 89:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument has incorrect dimensions.\n"),Err);
			}
			else
			{
				sciprint(_("argument has incorrect dimensions.\n"));
			}
		}
		break;
		case 90:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument must be a transfer matrix.\n"),Err);
			}
			else
			{
				sciprint(_("argument must be a transfer matrix.\n"));
			}
		}
		break;
		case 91:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument must be in state space form.\n"),Err);
			}
			else
			{
				sciprint(_("argument must be in state space form.\n"));
			}
		}
		break;
		case 92:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument must be a rational matrix.\n"),Err);
			}
			else
			{
				sciprint(_("argument must be a rational matrix.\n"));
			}
		}
		break;
		case 93:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument must be in continuous time.\n"),Err);
			}
			else
			{
				sciprint(_("waiting for continuous time...\n"));
			}
		}
		break;
		case 94:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument must be in discrete time.\n"),Err);
			}
			else
			{
				sciprint(_("argument must be in discrete time.\n"));
			}
		}
		break;
		case 95:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument must be SISO.\n"),Err);
			}
			else
			{
				sciprint(_("argument must be SISO.\n"));
			}

		}
		break;
		case 96:
		{
			if (Err != 1)
			{
				sciprint(_("time domain of %dth argument is not defined.\n"),Err);
			}
			else
			{
				sciprint(_("time domain of argument is not defined.\n"));
			}
		}
		break;
		case 97:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument must be a system in state space or transfer matrix form.\n"),Err);
			}
			else
			{
				sciprint(_("input argument must be a system in state space or transfer matrix form.\n"));
			}

		}
		break;
		case 98:
		{
			sciprint(_("variable returned by scilab argument function is incorrect.\n"));
		}
		break;
		case 99:
		{
			if (Err != 1)
			{
				sciprint(_(" elements of %dth th must be in increasing order!.\n"),Err);
			}
			else
			{
				sciprint(_("elements of first argument are not (strictly) increasing.\n"));
			}
		}
		break;
		case 100:
		{
			if (Err != 1)
			{
				sciprint(_("the elements of %dth argument are not in (strictly) decreasing order.\n"),Err);
			}
			else
			{
				sciprint(_("elements of first argument are not in (strictly) decreasing order.\n"));
			}
		}
		break;
		case 101:
		{
			if (Err != 1)
			{
				sciprint(_("last element of %dth argument <>  first.\n"),Err);
			}
			else
			{
				sciprint(_("last element of first argument does not matches the first one.\n"));
			}
		}
		break;
		case 102:
		{
			msgout_string(_("variable or function %s is not in file.\n"));
		}
		break;
		case 103:
		{
			msgout_string(_("variable %s is not a valid rational function.\n"));
		}
		break;
		case 104:
		{
			msgout_string(_("variable %s is not a valid state space representation.\n"));
		}
		break;
		case 105:
		{
			sciprint(_("Undefined fonction.\n"));
		}
		break;
		case 106:
		{
			sciprint(_("Function name already used.\n"));
		}
		break;
		case 107:
		{
			sciprint(_("too many functions are defined (maximum #:%d).\n"),Err);
		}
		break;
		case 108:
		{
			sciprint(_("too complex for scilab, may be a too long control instruction.\n"));
		}
		break;
		case 109:
		{
			sciprint(_("Too large, cant be displayed\n"));
		}
		break;
		case 110:
		{
			msgout_string(_("%s was a function when compiled but is now a primitive!\n"));
		}
		break;
		case 111:
		{
			msgout_string(_("trying to re-define function %s .\n"));
		}
		break;
		case 112:
		{
			sciprint(_("Cannot allocate more memory.\n"));
		}
		break;
		case 113:
		{
			sciprint(_("Too large string.\n"));
		}
		break;
		case 114:
		{
			sciprint(_("Too many linked routines.\n"));
		}
		break;
		case 115:
		{
			sciprint(_("Stack problem detected within a loop.\n"));
			sciprint(_("A primitive function has been called with wrong number of lhs arguments.\n"));
			sciprint(_("No lhs test made for this function.\n"));
			sciprint(_("Please report this bug :\n"));
			sciprint(_("http://www.scilab.org/cgi-bin/bugzilla_bug_II/index.cgi\n"));

			C2F(showstack)(); /* display of calling tree */
		}
		break;
		case 116:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument has incorrect value.\n"),Err);
			}
			else
			{
				sciprint(_("first argument has incorrect value.\n"));
			}
		}
		break;
		case 117:
		{
			sciprint(_("list element number %d is Undefined.\n"),Err);
		}
		break;
		case 118:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument must be a named variable not an expression.\n"),Err);
			}
			else
			{
				sciprint(_("argument must be a named variable not an expression.\n"));
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
			sciprint(_("Indices for non-zero elements must be given by a 2 column matrix.\n"));
		}
		break;
		case 121:
		{
			sciprint(_("Incompatible indices for non-zero elements.\n"));
		}
		break;
		case 122:
		{
			sciprint(_("logical unit number should be larger than %d.\n"),Err);
		}
		break;
		case 123:
		{
			sciprint(_("Function not bounded from below.\n"));
		}
		break;
		case 124:
		{
			/* no message  */
		}
		break;
		case 125:
		{
			sciprint(_("Problem may be unbounded : too high distance between two consecutive iterations.\n"));
		}
		break;
		case 126:
		{
			sciprint(_("Inconsistent constraints.\n"));
		}
		break;
		case 127:
		{
			sciprint(_("No feasible solution.\n"));
		}
		break;
		case 128:
		{
			sciprint(_("Degenerate starting point.\n"));
		}
		break;
		case 129:
		{
			sciprint(_("No feasible point has been found.\n"));
		}
		break;
		case 130:
		{
			sciprint(_("Optimization fails: back to initial point.\n"));
		}
		break;
		case 131:
		{
			sciprint(_("Optim: stop requested by simulator (ind=0)\n"));
		}
		break;
		case 132:
		{
			sciprint(_("Optim: incorrect input parameters.\n"));
		}
		break;
		case 133:
		{
			sciprint(_("Too small memory.\n"));
		}
		break;
		case 134:
		{
			sciprint(_("Optim: problem with initial constants in simul \n"));
		}
		break;
		case 135:
		{
			sciprint(_("Optim : bounds and initial guess are incompatible.\n"));
		}
		break;
		case 136:
		{
			sciprint(_("Optim : this method is NOT implemented.\n"));
		}
		break;
		case 137:
		{
			sciprint(_("NO hot restart available in this method.\n"));
		}
		break;
		case 138:
		{
			sciprint(_("Optim : incorrect stopping parameters.\n"));
		}
		break;
		case 139:
		{
			sciprint(_("Optim : incorrect bounds.\n"));
		}
		break;
		case 140:
		{
			/* no message  */
		}
		break;
		case 141:
		{
			sciprint(_("incorrect function (argument n:%d).\n"),Err);
		}
		break;
		case 142:
		{
			sciprint(_("hot restart : dimension of working table (argument n:%d).\n"),Err);
		}
		break;
		case 143:
		{
			sciprint(_("Optim : df0 must be positive !\n"));
		}
		break;
		case 144:
		{
			msgout_string(_("Undefined operation for the given operands check or define function %s for overloading\n"));
		}
		break;
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
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a real or complex matrix.\n"));
		}
		break;
		case 202:
		{
			/* not used  */
			/* only for compatibility */
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a real matrix.\n"));
		}
		break;
		case 203:
		{
			/* not used  */
			/* only for compatibility */
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a real vector.\n"));
		}
		break;
		case 204:
		{
			/* not used  */
			/* only for compatibility */
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a scalar.\n"));
		}
		break;
		case 205:
		{
			int minvalue = C2F(recu).pstk[C2F(recu).pt];
			int maxvalue = C2F(recu).pstk[C2F(recu).pt + 1];
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong matrix size "));
			sciprint(_("(%d,%d) expected.\n"),minvalue,maxvalue);
		}
		break;
		case 206:
		{
			int vectorsize = C2F(recu).pstk[C2F(recu).pt];
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong vector size "));
			sciprint(_("(%d) expected.\n"),vectorsize);
		}
		break;
		case 207:
		{
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a matrix of strings.\n"));
		}
		break;
		case 208: 
		{
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a boolean matrix.\n"));
		}
		break;
		case 209:
		{
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a matrix.\n"));
		}
		break;
		case 210:
		{
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a list.\n"));
		}
		break;
		case 211:
		{
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a function or string (external function).\n"));
		}
		break;
		case 212:
		{
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a polynomial.\n"));
		}
		break;
		case 213:
		{
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a working integer matrix.\n"));
		}
		break;
		case 214:
		{
			sciprint(_("Argument %d"),Err);
			msgout_string(_(" of %s: wrong type argument, expecting a  vector.\n"));
		}
		break;
		case 215:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument type must be boolean.\n"),Err);
			}
			else
			{
				sciprint(_("argument type must be boolean.\n"));
			}
		}
		break;
		case 216:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument type must be boolean or scalar.\n"),Err);
			}
			else
			{
				sciprint(_("argument type must be boolean or scalar.\n"));
			}
		}
		break;
		case 217:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument  must be a sparse matrix of scalars.\n"),Err);
			}
			else
			{
				sciprint(_("argument type must be a sparse matrix of scalars.\n"));
			}
		}
		break;
		case 218:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument  must be a  handle to sparse lu factors.\n"),Err);
			}
			else
			{
				sciprint(_("argument type must be a  handle to sparse lu factors.\n"));
			}
		}
		break;
		case 219:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument  must be a sparse or full scalar matrix.\n"),Err);
			}
			else
			{
				sciprint(_("argument type must be a sparse or full scalar matrix.\n"));
			}
		}
		break;
		case 220:
		{
			sciprint(_("Null variable cannot be used here.\n"));
		}
		break;
		case 221:
		{
			sciprint(_("A sparse matrix entry is defined with two differents values.\n"));
		}
		break;
		case 222:
		{
			sciprint(_("lusolve not yet implemented for full input parameter (RHS)\n"));
		}
		break;
		case 223:
		{
			msgout_string(_("It is not possible to redefine the %s primitive this way (see clearfun)."));
		}
		break;
		case 224:
		{
			sciprint(_("Type data base is full.\n"));
		}
		break;
		case 225:
		{
			sciprint(_("This data type is already defined.\n"));
		}
		break;
		case 226:
		{
			sciprint(_("Inequality comparison with empty matrix.\n"));
		}
		break;
		case 227:
		{
			sciprint(_("Missing index.\n"));
		}
		break;
		case 228:
		{
			msgout_string(_("reference to the cleared global variable %s.\n"));
		}
		break;
		case 229:
		{
			/* no message  */
		}
		break;
		case 230:
		{
			sciprint(_("semi def fails.\n"));
		}
		break;
		case 231:
		{
			sciprint(_("First argument must be a single string.\n"));
		}
		break;
		case 232:
		{
			sciprint(_("Entry name not found.\n"));
		}
		break;
		case 233:
		{
			sciprint(_("Maximum number of dynamic interfaces reached.\n"));
		}
		break;
		case 234:
		{
			sciprint(_("Link: expecting more than one argument.\n"));
		}
		break;
		case 235:
		{
			sciprint(_("Link: problem with one of the entry point.\n"));
		}
		break;
		case 236:
		{
			sciprint(_("Link: the shared archive was not loaded.\n"));
		}
		break;
		case 237:
		{
			sciprint(_("link: Only one entry point allowed on this operating system.\n"));
		}
		break;
		case 238:
		{
			sciprint(_("Link: First argument cannot be a number.\n"));
		}
		break;
		case 239:
		{
			sciprint(_("You cannot link more functions, maxentry reached.\n"));
		}
		break;
		case 240:
		{
			msgout_string(_("File %s already exists or directory write access denied.\n"));
		}
		break;
		case 241:
		{
			msgout_string(_("File %s does not exist or read access denied.\n"));
		}
		break;
		case 242:
		{
			sciprint(_("binary direct acces files must be opened by 'file'.\n"));
		}
		break;
		case 243:
		{
			sciprint(_("C file logical unit not allowed here.\n"));
		}
		break;
		case 244:
		{
			sciprint(_("Fortran file logical unit not allowed here.\n"));
		}
		break;
		case 245:
		{
			sciprint(_("No input file associated to logical unit %d.\n"),Err);
		}
		break;
		case 246:
		{
			sciprint(_("function not defined for given argument type(s),\n"));
			msgout_string(_("  check arguments or define function %s for overloading.\n"));
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
				sciprint(_("%dth argument is not a valid variable name.\n"),Err);
			}
			else
			{
				sciprint(_("argument is not a valid variable name.\n"));
			}
		}
		break;
		case 249:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument must not be an empty string.\n"),Err);
			}
			else
			{
				sciprint(_("argument must not be an empty string.\n"));
			}
		}
		break;
		case 250:
		{
			sciprint(_("Recursive extraction is not valid in this context.\n"));
		}
		break;
		case 251:
		{
			sciprint(_("bvode: ipar dimensioned at least 11.\n"));
		}
		break;
		case 252:
		{
			sciprint(_("bvode: ltol must be of size ipar(4).\n"));
		}
		break;
		case 253:
		{
			sciprint(_("bvode: fixpnt must be of size ipar(11).\n"));
		}
		break;
		case 254:
		{
			sciprint(_("bvode: ncomp < 20 requested.\n"));
		}
		break;
		case 255:
		{
			sciprint(_("bvode: m must be of size ncomp.\n"));
		}
		break;
		case 256: case 257:
		{
			sciprint(_("bvode: sum(m) must be less than 40.\n"));
		}
		break;
		case 258:
		{
			sciprint(_("bvode: input data error.\n"));
		}
		break;
		case 259:
		{
			sciprint(_("bvode: no. of subintervals exceeds storage.\n"));
		}
		break;
		case 260:
		{
			sciprint(_("bvode: Th colocation matrix is singular.\n"));
		}
		break;
		case 261:
		{
			sciprint(_("Interface property table is full.\n"));
		}
		break;
		case 262:
		{
			int maxglobalvariables = isizt - C2F(vstk).isiz - 1;
			sciprint(_("Too many global variables! , max number is %d.\n"),maxglobalvariables);
		}
		break;
		case 263:
		{
			sciprint(_("Error while writing in file,(disk full or deleted file.\n"));
		}
		break;
		case 264:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument must not contain NaN or Inf.\n"),Err);
			}
			else
			{
				sciprint(_("argument must not  contain NaN or Inf.\n"));
			}
		}
		break;
		case 265:
		{
			sciprint(_("A and B must have equal number of rows.\n"));
		}
		break;
		case 266:
		{
			sciprint(_("A and B must have equal number of columns.\n"));
		}
		break;
		case 267:
		{
			sciprint(_("A and B must have equal dimensions.\n"));
		}
		break;
		case 268:
		{
			sciprint(_("invalid return value for function passed in arg %d.\n"),Err);
		}
		break;
		case 269:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument eigenvalues must have negative real parts.\n"),Err);
			}
			else
			{
				sciprint(_("argument eigenvalues must have negative real parts.\n"));
			}
		}
		break;
		case 270:
		{
			if (Err != 1)
			{
				sciprint(_("%dth argument eigenvalues modulus must be less than one.\n"),Err);
			}
			else
			{
				sciprint(_("argument eigenvalues modulus must be less than one.\n"));
			}
		}
		break;
		case 271:
		{
			if (Err != 1)
			{
				sciprint(_("Size varying argument a*eye(), (arg %d) not allowed here.\n"),Err);
			}
			else
			{
				sciprint(_("Size varying argument a*eye() not allowed here.\n"));
			}
		}
		break;
		case 272:
		{
			sciprint(_("endfunction is missing.\n"));
		}
		break;
		case 273:
		{
			sciprint(_("Instruction left hand side: waiting for a dot or a left parenthesis"));
		}
		break;
		case 274:
		{
			sciprint(_("Instruction left hand side: waiting for a name.\n"));
		}
		break;
		case 275:
		{
			sciprint(_("varargout keyword cannot be used here.\n"));
		}
		break;
		case 276:
		{
			sciprint(_("Missing operator, comma, or semicolon.\n"));
			*errtyp = 1; /* error not recoverable */
		}
		break;
		case 277:
		{
			sciprint(_("Too many commands defined.\n"));
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
				sciprint(buffer);
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
static void msgout_string(const char *msgformat)
{
	int i = 0;
	
	char *variable_buffer = NULL;
	C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6], C2F(cha1).buf, &i, nlgh);

	variable_buffer = (char*)MALLOC( sizeof(char)*(1+strlen(C2F(cha1).buf) ) );
	if (variable_buffer)
	{
		char *msg_buffer = NULL;
		strcpy(variable_buffer,C2F(cha1).buf);
		strip_blank(variable_buffer);

		msg_buffer = (char*)MALLOC( sizeof(char)*( strlen(msgformat)+1+strlen(variable_buffer) ) );
		if (msg_buffer)
		{
			sprintf(msg_buffer,msgformat,variable_buffer);
			strip_blank(msg_buffer);

			displayAndStoreError(msg_buffer);

			FREE(msg_buffer);
			msg_buffer = NULL;
		}
		FREE(variable_buffer);
		variable_buffer = NULL;

	}
}
/*-----------------------------------------------------------------------------------*/
static void displayAndStoreError(const char *msg)
{
	int len = 0;

	/* store error (lasterror) */
	len = (int)strlen(msg);
	C2F(msgstore)((char*)msg,&len);

	/* display error */
	sciprint((char*)msg);
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
