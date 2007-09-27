/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "msg_errors.h"
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
    C2F(funnamestore)(" ", &i,strlen(" "));
    C2F(freemsgtable)();
    C2F(errstore)(n);
    
    switch ((int)*n) 
    {
			case 1:
			{
				MSGOUT("incorrect assignment");
			}
			break;
			case 2:
			{
				MSGOUT("invalid factor");
    			*errtyp = 1;
			}
			break;
			case 3:
			{
				MSGOUT("waiting for right parenthesis");
    			*errtyp = 1;
			}
			break;
			case 4:
			{
				char *Str=NULL;
				C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6], C2F(cha1).buf, &i, 4096L);
				Str=(char*)MALLOC( sizeof(char)*( strlen("undefined variable : %s")+1+strlen(C2F(cha1).buf) ) );
				sprintf(Str,SCI_ERRMSG4,C2F(cha1).buf);
				MSGOUT(Str);
				FREE(Str);
			}
			break;
			case 5:
			{
				MSGOUT(SCI_ERRMSG5);
			}
			break;
			case 6:
			{
				MSGOUT(SCI_ERRMSG6);
			}
			break;
			case 7:
			{
				MSGOUT("dot cannot be used as modifier for this operator");
			}
			break;
			case 8:
			{
				MSGOUT("inconsistent addition");
			}
			break;
			case 9:
			{
				MSGOUT("inconsistent subtraction");
			}
			break;
			case 10:
			{
				MSGOUT("inconsistent multiplication");
			}
			break;
			case 11:
			{
				MSGOUT("inconsistent right division ");
			}
			break;
			case 12:
			{
				MSGOUT("inconsistent left division");
			}
			break;
			case 13:
			{
				MSGOUT("redefining permanent variable");
			}
			break;
			case 14:
			{
				MSGOUT("eye variable undefined in this context");
			}
			break;
			case 15:
			{
				MSGOUT("submatrix incorrectly defined");
			}
			break;
			case 16:
			{
				MSGOUT("incorrect command!");
				*errtyp = 1;
			}
			break;
			case 17:
			{
			}
			break;
			case 18:
			{
				MSGOUT("too many variables!");
			}
			break;
			case 19:
			{
				MSGOUT("Problem is singular");
			}
			break;
			case 20:
			{
			}
			break;
			case 21:
			{
				MSGOUT("invalid index");
			}
			break;
			case 22:
			{
				MSGOUT(" recursion problems. Sorry....");
			}
			break;
			case 23:
			{
				MSGOUT(" Matrix norms available are 1, 2, inf, and fro");
			}
			break;
			case 24:
			{
				MSGOUT("convergence problem...");
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
				MSGOUT("division by zero...");
			}
			break;
			case 28:
			{
				MSGOUT("empty function...");
			}
			break;
			case 29:
			{
				MSGOUT("matrix is not positive definite");
			}
			break;
			case 30:
			{
				MSGOUT("invalid exponent");
			}
			break;
			case 31:
			{
			}
			break;
			case 32:
			{
				MSGOUT("singularity of log or tan function");
			}
			break;
			case 33:
			{
				MSGOUT("too many :");
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
				MSGOUT("file name incorrect");
			}
			break;
			case 39:
			{
				MSGOUT("incorrect number of arguments");
			}
			break;
			case 40:
			{
			}
			break;
			case 41:
			{
				MSGOUT("incompatible LHS");
			}
			break;
			case 42:
			{
				MSGOUT("incompatible RHS");
			}
			break;
			case 43:
			{
				MSGOUT("not implemented in scilab....");
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
				MSGOUT("incorrect file or format");
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
				MSGOUT("problem with comparison symbol...");
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
				MSGOUT("argument with incompatible dimensions");
			}
			break;
			case 61:
			{
				MSGOUT("direct acces : give format");
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
				MSGOUT("integration fails");
			}
			break;
			case 65:
			{
			}
			break;
			case 66:
			{
				MSGOUT("no more logical units available!");
			}
			break;
			case 67:
			{
				MSGOUT("unknown file format ");
			}
			break;
			case 68:
			{
			}
			break;
			case 69:
			{
				MSGOUT("floating point exception");
			}
			break;
			case 70:
			{
				MSGOUT("too many arguments in fort (max 30)");
			}
			break;
			case 71:
			{
				MSGOUT("this variable is not valid in fort");
			}
			break;
			case 72:
			{
			}
			break;
			case 73:
			{
				MSGOUT("error while linking");
			}
			break;
			case 74:
			{
				MSGOUT("Leading coefficient is zero");
			}
			break;
			case 75:
			{
				MSGOUT("Too high degree (max 100)");
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
				MSGOUT("host does not answer...");
			}
			break;
			case 86:
			{
				MSGOUT("uncontrollable system");
			}
			break;
			case 87:
			{
				MSGOUT("unobservable system");
			}
			break;
			case 88:
			{
				MSGOUT("sfact : singular or assymetric problem");
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
				MSGOUT("undefined fonction");
			}
			break;
			case 106:
			{
				MSGOUT(" fonction name already used");
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
				MSGOUT("too large, cant be displayed");
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
				MSGOUT("Cannot allocate more memory");
			}
			break;
			case 113:
			{
				MSGOUT("too large string");
			}
			break;
			case 114:
			{
				MSGOUT("too many linked routines");
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
				MSGOUT(" function not bounded from below");
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
				MSGOUT("Inconsistent constraints");
			}
			break;
			case 127:
			{
				MSGOUT("no feasible solution");
			}
			break;
			case 128:
			{
				MSGOUT("degenerate starting point");
			}
			break;
			case 129:
			{
				MSGOUT("no feasible point has been found");
			}
			break;
			case 130:
			{
				MSGOUT(" optimization fails: back to initial point");
			}
			break;
			case 131:
			{
				MSGOUT(" optim: stop requested by simulator (ind=0)");
			}
			break;
			case 132:
			{
				MSGOUT(" optim: incorrect input parameters");
			}
			break;
			case 133:
			{
				MSGOUT(" too small memory");
			}
			break;
			case 134:
			{
				MSGOUT("optim: problem with initial constants in simul ");
			}
			break;
			case 135:
			{
				MSGOUT("optim : bounds and initial guess are incompatible");
			}
			break;
			case 136:
			{
				MSGOUT("optim : this method is NOT implemented ");
			}
			break;
			case 137:
			{
				MSGOUT("NO hot restart available in this method");
			}
			break;
			case 138:
			{
				MSGOUT("optim : incorrect stopping parameters");
			}
			break;
			case 139:
			{
				MSGOUT("optim : incorrect bounds");
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
				MSGOUT("optim : df0 must be positive !");
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
				MSGOUT("null variable cannot be used here");
			}
			break;
			case 221:
			{
			}
			break;
			case 222:
			{
				MSGOUT("lusolve not yet implemented for full RHS");
			}
			break;
			case 223:
			{
			}
			break;
			case 224:
			{
				MSGOUT("Type data base is full");
			}
			break;
			case 225:
			{
				MSGOUT("This data type is already defined");
			}
			break;
			case 226:
			{
				MSGOUT("Inequality comparison with empty matrix");
			}
			break;
			case 227:
			{
				MSGOUT("Missing index");
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
				MSGOUT("semidef fails");
			}
			break;
			case 231:
			{
				MSGOUT("First argument must be a single string");
			}
			break;
			case 232:
			{
				MSGOUT("Entry name not found");
			}
			break;
			case 233:
			{
			}
			break;
			case 234:
			{
				MSGOUT("link: expecting more than one argument");
			}
			break;
			case 235:
			{
				MSGOUT("link: problem with one of the entry point");
			}
			break;
			case 236:
			{
				MSGOUT("link: the shared archive was not loaded");
			}
			break;
			case 237:
			{
			}
			break;
			case 238:
			{
				MSGOUT("link: First argument cannot be a number");
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
				MSGOUT("C file logical unit not allowed here");
			}
			break;
			case 244:
			{
				MSGOUT("Fortran file logical unit not allowed here");
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
				MSGOUT("bvode: ipar dimensioned at least 11");
			}
			break;
			case 252:
			{
				MSGOUT("bvode: ltol must be of size ipar(4)");
			}
			break;
			case 253:
			{
				MSGOUT("bvode: fixpnt must be of size ipar(11)");
			}
			break;
			case 254:
			{
				MSGOUT("bvode: ncomp < 20 requested ");
			}
			break;
			case 255:
			{
				MSGOUT("bvode: m must be of size ncomp");
			}
			break;
			case 256:
			{
				MSGOUT("bvode: sum(m) must be less than 40");
			}
			break;
			case 257:
			{
				MSGOUT("bvode: sum(m) must be less than 40");
			}
			break;
			case 258:
			{
				MSGOUT("bvode: input data error");
			}
			break;
			case 259:
			{
				MSGOUT("bvode: no. of subintervals exceeds storage");
			}
			break;
			case 260:
			{
				MSGOUT("bvode: Th colocation matrix is singular");
			}
			break;
			case 261:
			{
				MSGOUT("Interface property table is full");
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
				MSGOUT("A and B must have equal number of rows");
			}
			break;
			case 266:
			{
				MSGOUT("A and B must have equal number of columns");
			}
			break;
			case 267:
			{
				MSGOUT("A and B must have equal dimensions");
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
				MSGOUT("endfunction is missing");
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
				MSGOUT("varargout keyword cannot be used here");
			}
			break;
			case 276:
			{
				MSGOUT("Missing operator, comma, or semicolon");
			}
			break;
			case 277:
			{
				MSGOUT("Too many commands defined");
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
