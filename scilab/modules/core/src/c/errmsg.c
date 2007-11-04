/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "machine.h"
#include "localization.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "msgstore.h"
#include "inffic.h"
#include "msgout.h"
/*-----------------------------------------------------------------------------------*/
#define MSGOUT(msg) C2F(msgout)(&io, &lunit, msg, strlen(msg));
/*-----------------------------------------------------------------------------------*/
extern int C2F(showstack)();
extern int C2F(cvname)();
extern int C2F(prntid)();
/*-----------------------------------------------------------------------------------*/
int C2F(errmsg)(integer *n,integer *errtyp)
{
	int i=0;
    static integer lunit=0;
	static integer ll=0;
    static integer io=0;
    
    ll = C2F(iop).lct[4];
    lunit = C2F(iop).wte;
    *errtyp = 0;

    C2F(linestore)(&i);
    C2F(funnamestore)(" ", &i);//strlen(" "));
    C2F(freemsgtable)();
    C2F(errstore)(n);
    
    switch ((int)*n) 
    {
			case 1:
			{
				MSGOUT(_("Incorrect assignment"));
			}
			break;
			case 2:
			{
				MSGOUT(("Invalid factor"));
    			*errtyp = 1;
			}
			break;
			case 3:
			{
				MSGOUT(_("Waiting for right parenthesis"));
    			*errtyp = 1;
			}
			break;
			case 4:
			{
				char *Str=NULL;
				C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6], C2F(cha1).buf, &i, 4096L);
				#define ERROR_MSG _("Undefined variable : %s")
				Str=(char*)MALLOC( sizeof(char)*( strlen(ERROR_MSG)+1+strlen(C2F(cha1).buf) ) );
				sprintf(Str,ERROR_MSG,C2F(cha1).buf);
				MSGOUT(Str);
				FREE(Str);
				#undef ERROR_MSG
			}
			break;
			case 5:
			{
				MSGOUT(_("Inconsistent column/row dimensions"));
			}
			break;
			case 6:
			{
				MSGOUT(_("Inconsistent row/column dimensions"));
			}
			break;
			case 7:
			{
				MSGOUT(_("Dot cannot be used as modifier for this operator"));
			}
			break;
			case 8:
			{
				MSGOUT(_("Inconsistent addition"));
			}
			break;
			case 9:
			{
				MSGOUT(_("Inconsistent subtraction"));
			}
			break;
			case 10:
			{
				MSGOUT(_("Inconsistent multiplication"));
			}
			break;
			case 11:
			{
				MSGOUT(_("Inconsistent right division "));
			}
			break;
			case 12:
			{
				MSGOUT(_("Inconsistent left division"));
			}
			break;
			case 13:
			{
				MSGOUT(_("Redefining permanent variable"));
			}
			break;
			case 14:
			{
				MSGOUT(_("Eye variable undefined in this context"));
			}
			break;
			case 15:
			{
				MSGOUT(_("Submatrix incorrectly defined"));
			}
			break;
			case 16:
			{
				MSGOUT(_("Incorrect command!"));
				*errtyp = 1;
			}
			break;
			case 17:
			{
			}
			break;
			case 18:
			{
				MSGOUT(_("Too many variables!"));
			}
			break;
			case 19:
			{
				MSGOUT(_("Problem is singular"));
			}
			break;
			case 20:
			{
			}
			break;
			case 21:
			{
				MSGOUT(_("Invalid index"));
			}
			break;
			case 22:
			{
				MSGOUT(_("Recursion problems. Sorry...."));
			}
			break;
			case 23:
			{
				MSGOUT(_("Matrix norms available are 1, 2, inf, and fro"));
			}
			break;
			case 24:
			{
				MSGOUT(_("Convergence problem..."));
			}
			break;
			case 25:
			{
			}
			break;
			case 26:
			{
			}
			break;
			case 27:
			{
				MSGOUT(_("Division by zero..."));
			}
			break;
			case 28:
			{
				MSGOUT(_("Empty function..."));
			}
			break;
			case 29:
			{
				MSGOUT(_("Matrix is not positive definite"));
			}
			break;
			case 30:
			{
				MSGOUT(_("Invalid exponent"));
			}
			break;
			case 31:
			{
			}
			break;
			case 32:
			{
				MSGOUT(_("Singularity of log or tan function"));
			}
			break;
			case 33:
			{
				MSGOUT(_("Too many :"));
			}
			break;
			case 34:
			{

			}
			break;
			case 35:
			{
			}
			break;
			case 36:
			{
			}
			break;
			case 37:
			{
			}
			break;
			case 38:
			{
				MSGOUT(_("File name incorrect"));
			}
			break;
			case 39:
			{
				MSGOUT(_("Incorrect number of arguments"));
			}
			break;
			case 40:
			{
			}
			break;
			case 41:
			{
				MSGOUT(_("Incompatible output parameter (LHS)"));
			}
			break;
			case 42:
			{
				MSGOUT(_("Incompatible input parameter (RHS)"));
			}
			break;
			case 43:
			{
				MSGOUT(_("Not implemented in scilab...."));
			}
			break;
			case 44:
			{
			}
			break;
			case 45:
			{
			}
			break;
			case 46:
			{
			}
			break;
			case 47:
			{
			}
			break;
			case 48:
			{
			}
			break;
			case 49:
			{
				MSGOUT(_("Incorrect file or format"));
			}
			break;
			case 50:
			{
			}
			break;
			case 51:
			{
				/* no message  */
			}
			break;
			case 52:
			{
			}
			break;
			case 53:
			{
			}
			break;
			case 54:
			{
			}
			break;
			case 55:
			{
			}
			break;
			case 56:
			{
			}
			break;
			case 57:
			{
				MSGOUT(_("Problem with comparison symbol..."));
			}
			break;
			case 58:
			{
			}
			break;
			case 59:
			{
			}
			break;
			case 60:
			{
				MSGOUT(_("Argument with incompatible dimensions"));
			}
			break;
			case 61:
			{
				MSGOUT(_("Direct acces : give format"));
			}
			break;
			case 62:
			{
			}
			break;
			case 63:
			{
			}
			break;
			case 64:
			{
				MSGOUT(_("Integration fails"));
			}
			break;
			case 65:
			{
			}
			break;
			case 66:
			{
				MSGOUT(_("No more logical units available!"));
			}
			break;
			case 67:
			{
				MSGOUT(_("Unknown file format "));
			}
			break;
			case 68:
			{
			}
			break;
			case 69:
			{
				MSGOUT(_("Floating point exception"));
			}
			break;
			case 70:
			{
				MSGOUT(_("Too many arguments in fort (max 30)"));
			}
			break;
			case 71:
			{
				MSGOUT(_("This variable is not valid in fort"));
			}
			break;
			case 72:
			{
			}
			break;
			case 73:
			{
				MSGOUT(_("Error while linking"));
			}
			break;
			case 74:
			{
				MSGOUT(_("Leading coefficient is zero"));
			}
			break;
			case 75:
			{
				MSGOUT(_("Too high degree (max 100)"));
			}
			break;
			case 76:
			{
			}
			break;
			case 77:
			{
			}
			break;
			case 78:
			{
			}
			break;
			case 79:
			{
				/* no message  */
			}
			break;
			case 80:
			{
			}
			break;
			case 81:
			{
			}
			break;
			case 82:
			{
			}
			break;
			case 83:
			{
			}
			break;
			case 84:
			{
			}
			break;
			case 85:
			{
				MSGOUT(_("Host does not answer..."));
			}
			break;
			case 86:
			{
				MSGOUT(_("Uncontrollable system"));
			}
			break;
			case 87:
			{
				MSGOUT(_("Unobservable system"));
			}
			break;
			case 88:
			{
				MSGOUT(_("sfact : singular or assymetric problem"));
			}
			break;
			case 89:
			{
			}
			break;
			case 90:
			{
			}
			break;
			case 91:
			{
			}
			break;
			case 92:
			{
			}
			break;
			case 93:
			{
			}
			break;
			case 94:
			{
			}
			break;
			case 95:
			{
			}
			break;
			case 96:
			{
			}
			break;
			case 97:
			{
			}
			break;
			case 98:
			{
			}
			break;
			case 99:
			{
			}
			break;
			case 100:
			{
			}
			break;
			case 101:
			{
			}
			break;
			case 102:
			{
			}
			break;
			case 103:
			{
			}
			break;
			case 104:
			{
			}
			break;
			case 105:
			{
				MSGOUT(_("Undefined fonction"));
			}
			break;
			case 106:
			{
				MSGOUT(_("Function name already used"));
			}
			break;
			case 107:
			{
			}
			break;
			case 108:
			{
			}
			break;
			case 109:
			{
				MSGOUT(_("Too large, cant be displayed"));
			}
			break;
			case 110:
			{
			}
			break;
			case 111:
			{
			}
			break;
			case 112:
			{
				MSGOUT(_("Cannot allocate more memory"));
			}
			break;
			case 113:
			{
				MSGOUT(_("Too large string"));
			}
			break;
			case 114:
			{
				MSGOUT(_("Too many linked routines"));
			}
			break;
			case 115:
			{
			}
			break;
			case 116:
			{
			}
			break;
			case 117:
			{
			}
			break;
			case 118:
			{
			}
			break;
			case 119:
			{
				/* no message  */
			}
			break;
			case 120:
			{
			}
			break;
			case 121:
			{
			}
			break;
			case 122:
			{
			}
			break;
			case 123:
			{
				MSGOUT(_("Function not bounded from below"));
			}
			break;
			case 124:
			{
				/* no message  */
			}
			break;
			case 125:
			{
			}
			break;
			case 126:
			{
				MSGOUT(_("Inconsistent constraints"));
			}
			break;
			case 127:
			{
				MSGOUT(_("No feasible solution"));
			}
			break;
			case 128:
			{
				MSGOUT(_("Degenerate starting point"));
			}
			break;
			case 129:
			{
				MSGOUT(_("No feasible point has been found"));
			}
			break;
			case 130:
			{
				MSGOUT(_("Optimization fails: back to initial point"));
			}
			break;
			case 131:
			{
				MSGOUT(_("Optim: stop requested by simulator (ind=0)"));
			}
			break;
			case 132:
			{
				MSGOUT(_("Optim: incorrect input parameters"));
			}
			break;
			case 133:
			{
				MSGOUT(_("Too small memory"));
			}
			break;
			case 134:
			{
				MSGOUT(_("Optim: problem with initial constants in simul "));
			}
			break;
			case 135:
			{
				MSGOUT(_("Optim : bounds and initial guess are incompatible"));
			}
			break;
			case 136:
			{
				MSGOUT(_("Optim : this method is NOT implemented "));
			}
			break;
			case 137:
			{
				MSGOUT(_("NO hot restart available in this method"));
			}
			break;
			case 138:
			{
				MSGOUT(_("Optim : incorrect stopping parameters"));
			}
			break;
			case 139:
			{
				MSGOUT(_("Optim : incorrect bounds"));
			}
			break;
			case 140:
			{/* no message  */
			}
			break;
			case 141:
			{
			}
			break;
			case 142:
			{
			}
			break;
			case 143:
			{
				MSGOUT(_("Optim : df0 must be positive !"));
			}
			break;
			case 144:
			{
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
			}
			break;
			case 202:
			{
			}
			break;
			case 203:
			{
			}
			break;
			case 204:
			{
			}
			break;
			case 205:
			{
			}
			break;
			case 206:
			{
			}
			break;
			case 207:
			{
			}
			break;
			case 208:
			{
			}
			break;
			case 209:
			{
			}
			break;
			case 210:
			{
			}
			break;
			case 211:
			{
			}
			break;
			case 212:
			{
			}
			break;
			case 213:
			{
			}
			break;
			case 214:
			{
			}
			break;
			case 215:
			{
			}
			break;
			case 216:
			{
			}
			break;
			case 217:
			{
			}
			break;
			case 218:
			{
			}
			break;
			case 219:
			{
			}
			break;
			case 220:
			{
				MSGOUT(_("Null variable cannot be used here"));
			}
			break;
			case 221:
			{
			}
			break;
			case 222:
			{
				MSGOUT(_("lusolve not yet implemented for full input parameter (RHS)"));
			}
			break;
			case 223:
			{
			}
			break;
			case 224:
			{
				MSGOUT(_("Type data base is full"));
			}
			break;
			case 225:
			{
				MSGOUT(_("This data type is already defined"));
			}
			break;
			case 226:
			{
				MSGOUT(_("Inequality comparison with empty matrix"));
			}
			break;
			case 227:
			{
				MSGOUT(_("Missing index"));
			}
			break;
			case 228:
			{
			}
			break;
			case 229:
			{
				/* no message  */
			}
			break;
			case 230:
			{
				MSGOUT(_("semidef fails"));
			}
			break;
			case 231:
			{
				MSGOUT(_("First argument must be a single string"));
			}
			break;
			case 232:
			{
				MSGOUT(_("Entry name not found"));
			}
			break;
			case 233:
			{
			}
			break;
			case 234:
			{
				MSGOUT(_("Link: expecting more than one argument"));
			}
			break;
			case 235:
			{
				MSGOUT(_("Link: problem with one of the entry point"));
			}
			break;
			case 236:
			{
				MSGOUT(_("Link: the shared archive was not loaded"));
			}
			break;
			case 237:
			{
			}
			break;
			case 238:
			{
				MSGOUT(_("Link: First argument cannot be a number"));
			}
			break;
			case 239:
			{
			}
			break;
			case 240:
			{
			}
			break;
			case 241:
			{
			}
			break;
			case 242:
			{
			}
			break;
			case 243:
			{
				MSGOUT(_("C file logical unit not allowed here"));
			}
			break;
			case 244:
			{
				MSGOUT(_("Fortran file logical unit not allowed here"));
			}
			break;
			case 245:
			{
			}
			break;
			case 246:
			{
			}
			break;
			case 247:
			{
				/* no message  */
			}
			break;
			case 248:
			{
			}
			break;
			case 249:
			{
			}
			break;
			case 250:
			{
			}
			break;
			case 251:
			{
				MSGOUT(_("bvode: ipar dimensioned at least 11"));
			}
			break;
			case 252:
			{
				MSGOUT(_("bvode: ltol must be of size ipar(4)"));
			}
			break;
			case 253:
			{
				MSGOUT(_("bvode: fixpnt must be of size ipar(11)"));
			}
			break;
			case 254:
			{
				MSGOUT(_("bvode: ncomp < 20 requested "));
			}
			break;
			case 255:
			{
				MSGOUT(_("bvode: m must be of size ncomp"));
			}
			break;
			case 256:
			{
				MSGOUT(_("bvode: sum(m) must be less than 40"));
			}
			break;
			case 257:
			{
				MSGOUT(_("bvode: sum(m) must be less than 40"));
			}
			break;
			case 258:
			{
				MSGOUT(_("bvode: input data error"));
			}
			break;
			case 259:
			{
				MSGOUT(_("bvode: no. of subintervals exceeds storage"));
			}
			break;
			case 260:
			{
				MSGOUT(_("bvode: Th colocation matrix is singular"));
			}
			break;
			case 261:
			{
				MSGOUT(_("Interface property table is full"));
			}
			break;
			case 262:
			{
			}
			break;
			case 263:
			{
			}
			break;
			case 264:
			{
			}
			break;
			case 265:
			{
				MSGOUT(_("A and B must have equal number of rows"));
			}
			break;
			case 266:
			{
				MSGOUT(_("A and B must have equal number of columns"));
			}
			break;
			case 267:
			{
				MSGOUT(_("A and B must have equal dimensions"));
			}
			break;
			case 268:
			{
			}
			break;
			case 269:
			{
			}
			break;
			case 270:
			{
			}
			break;
			case 271:
			{
			}
			break;
			case 272:
			{
				MSGOUT(_("endfunction is missing"));
			}
			break;
			case 273:
			{
			}
			break;
			case 274:
			{
			}
			break;
			case 275:
			{
				MSGOUT(_("varargout keyword cannot be used here"));
			}
			break;
			case 276:
			{
				MSGOUT(_("Missing operator, comma, or semicolon"));
			}
			break;
			case 277:
			{
				MSGOUT(_("Too many commands defined"));
			}
			break;
			case 278:
			{
				/* no message  */
			}
			break;
			case 279:
			{
				/* no message  */
			}
			break;
			case 280:
			{
				/* no message  */
			}
			break;
			
			default:
			{
				integer bufl;
				/* message d'erreur soft */
				/* Bug 1422 corrected - Francois VOGEL June 2006 */
				bufl = 1;
				while(*(unsigned char *)&C2F(cha1).buf[bufl - 1] != '\0' && bufl < 80) 
				{
					++bufl;
				}
				--bufl;
				MSGOUT(C2F(cha1).buf);
			}
			break;
    }
		return 0; 
}
/*-----------------------------------------------------------------------------------*/    
