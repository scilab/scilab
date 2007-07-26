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
				MSGOUT(SCI_ERRMSG1);
			}
			break;
			case 2:
			{
				MSGOUT(SCI_ERRMSG2);
    			*errtyp = 1;
			}
			break;
			case 3:
			{
				MSGOUT(SCI_ERRMSG3);
    			*errtyp = 1;
			}
			break;
			case 4:
			{
				char *Str=NULL;
				C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6], C2F(cha1).buf, &i, 4096L);
				Str=(char*)MALLOC( sizeof(char)*( strlen(SCI_ERRMSG4)+1+strlen(C2F(cha1).buf) ) );
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
				MSGOUT(SCI_ERRMSG7);
			}
			break;
			case 8:
			{
				MSGOUT(SCI_ERRMSG8);
			}
			break;
			case 9:
			{
				MSGOUT(SCI_ERRMSG9);
			}
			break;
			case 10:
			{
				MSGOUT(SCI_ERRMSG10);
			}
			break;
			case 11:
			{
				MSGOUT(SCI_ERRMSG11);
			}
			break;
			case 12:
			{
				MSGOUT(SCI_ERRMSG12);
			}
			break;
			case 13:
			{
				MSGOUT(SCI_ERRMSG13);
			}
			break;
			case 14:
			{
				MSGOUT(SCI_ERRMSG14);
			}
			break;
			case 15:
			{
				MSGOUT(SCI_ERRMSG15);
			}
			break;
			case 16:
			{
				MSGOUT(SCI_ERRMSG16);
				*errtyp = 1;
			}
			break;
			case 17:
			{
			}
			break;
			case 18:
			{
				MSGOUT(SCI_ERRMSG18);
			}
			break;
			case 19:
			{
				MSGOUT(SCI_ERRMSG19);
			}
			break;
			case 20:
			{
			}
			break;
			case 21:
			{
				MSGOUT(SCI_ERRMSG21);
			}
			break;
			case 22:
			{
				MSGOUT(SCI_ERRMSG22);
			}
			break;
			case 23:
			{
				MSGOUT(SCI_ERRMSG23);
			}
			break;
			case 24:
			{
				MSGOUT(SCI_ERRMSG24);
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
				MSGOUT(SCI_ERRMSG27);
			}
			break;
			case 28:
			{
				MSGOUT(SCI_ERRMSG28);
			}
			break;
			case 29:
			{
				MSGOUT(SCI_ERRMSG29);
			}
			break;
			case 30:
			{
				MSGOUT(SCI_ERRMSG30);
			}
			break;
			case 31:
			{
			}
			break;
			case 32:
			{
				MSGOUT(SCI_ERRMSG32);
			}
			break;
			case 33:
			{
				MSGOUT(SCI_ERRMSG33);
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
				MSGOUT(SCI_ERRMSG38);
			}
			break;
			case 39:
			{
				MSGOUT(SCI_ERRMSG39);
			}
			break;
			case 40:
			{
			}
			break;
			case 41:
			{
				MSGOUT(SCI_ERRMSG41);
			}
			break;
			case 42:
			{
				MSGOUT(SCI_ERRMSG42);
			}
			break;
			case 43:
			{
				MSGOUT(SCI_ERRMSG43);
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
				MSGOUT(SCI_ERRMSG49);
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
				MSGOUT(SCI_ERRMSG57);
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
				MSGOUT(SCI_ERRMSG60);
			}
			break;
			case 61:
			{
				MSGOUT(SCI_ERRMSG61);
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
				MSGOUT(SCI_ERRMSG64);
			}
			break;
			case 65:
			{
			}
			break;
			case 66:
			{
				MSGOUT(SCI_ERRMSG66);
			}
			break;
			case 67:
			{
				MSGOUT(SCI_ERRMSG67);
			}
			break;
			case 68:
			{
			}
			break;
			case 69:
			{
				MSGOUT(SCI_ERRMSG69);
			}
			break;
			case 70:
			{
				MSGOUT(SCI_ERRMSG70);
			}
			break;
			case 71:
			{
				MSGOUT(SCI_ERRMSG71);
			}
			break;
			case 72:
			{
			}
			break;
			case 73:
			{
				MSGOUT(SCI_ERRMSG73);
			}
			break;
			case 74:
			{
				MSGOUT(SCI_ERRMSG74);
			}
			break;
			case 75:
			{
				MSGOUT(SCI_ERRMSG75);
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
				MSGOUT(SCI_ERRMSG85);
			}
			break;
			case 86:
			{
				MSGOUT(SCI_ERRMSG86);
			}
			break;
			case 87:
			{
				MSGOUT(SCI_ERRMSG87);
			}
			break;
			case 88:
			{
				MSGOUT(SCI_ERRMSG88);
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
				MSGOUT(SCI_ERRMSG105);
			}
			break;
			case 106:
			{
				MSGOUT(SCI_ERRMSG106);
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
				MSGOUT(SCI_ERRMSG109);
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
				MSGOUT(SCI_ERRMSG112);
			}
			break;
			case 113:
			{
				MSGOUT(SCI_ERRMSG113);
			}
			break;
			case 114:
			{
				MSGOUT(SCI_ERRMSG114);
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
				MSGOUT(SCI_ERRMSG123);
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
				MSGOUT(SCI_ERRMSG126);
			}
			break;
			case 127:
			{
				MSGOUT(SCI_ERRMSG127);
			}
			break;
			case 128:
			{
				MSGOUT(SCI_ERRMSG128);
			}
			break;
			case 129:
			{
				MSGOUT(SCI_ERRMSG129);
			}
			break;
			case 130:
			{
				MSGOUT(SCI_ERRMSG130);
			}
			break;
			case 131:
			{
				MSGOUT(SCI_ERRMSG131);
			}
			break;
			case 132:
			{
				MSGOUT(SCI_ERRMSG132);
			}
			break;
			case 133:
			{
				MSGOUT(SCI_ERRMSG133);
			}
			break;
			case 134:
			{
				MSGOUT(SCI_ERRMSG134);
			}
			break;
			case 135:
			{
				MSGOUT(SCI_ERRMSG135);
			}
			break;
			case 136:
			{
				MSGOUT(SCI_ERRMSG136);
			}
			break;
			case 137:
			{
				MSGOUT(SCI_ERRMSG137);
			}
			break;
			case 138:
			{
				MSGOUT(SCI_ERRMSG138);
			}
			break;
			case 139:
			{
				MSGOUT(SCI_ERRMSG139);
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
				MSGOUT(SCI_ERRMSG143);
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
				MSGOUT(SCI_ERRMSG220);
			}
			break;
			case 221:
			{
			}
			break;
			case 222:
			{
				MSGOUT(SCI_ERRMSG222);
			}
			break;
			case 223:
			{
			}
			break;
			case 224:
			{
				MSGOUT(SCI_ERRMSG224);
			}
			break;
			case 225:
			{
				MSGOUT(SCI_ERRMSG225);
			}
			break;
			case 226:
			{
				MSGOUT(SCI_ERRMSG226);
			}
			break;
			case 227:
			{
				MSGOUT(SCI_ERRMSG227);
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
				MSGOUT(SCI_ERRMSG230);
			}
			break;
			case 231:
			{
				MSGOUT(SCI_ERRMSG231);
			}
			break;
			case 232:
			{
				MSGOUT(SCI_ERRMSG232);
			}
			break;
			case 233:
			{
			}
			break;
			case 234:
			{
				MSGOUT(SCI_ERRMSG234);
			}
			break;
			case 235:
			{
				MSGOUT(SCI_ERRMSG235);
			}
			break;
			case 236:
			{
				MSGOUT(SCI_ERRMSG236);
			}
			break;
			case 237:
			{
			}
			break;
			case 238:
			{
				MSGOUT(SCI_ERRMSG238);
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
				MSGOUT(SCI_ERRMSG243);
			}
			break;
			case 244:
			{
				MSGOUT(SCI_ERRMSG244);
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
				MSGOUT(SCI_ERRMSG251);
			}
			break;
			case 252:
			{
				MSGOUT(SCI_ERRMSG252);
			}
			break;
			case 253:
			{
				MSGOUT(SCI_ERRMSG253);
			}
			break;
			case 254:
			{
				MSGOUT(SCI_ERRMSG254);
			}
			break;
			case 255:
			{
				MSGOUT(SCI_ERRMSG255);
			}
			break;
			case 256:
			{
				MSGOUT(SCI_ERRMSG256);
			}
			break;
			case 257:
			{
				MSGOUT(SCI_ERRMSG257);
			}
			break;
			case 258:
			{
				MSGOUT(SCI_ERRMSG258);
			}
			break;
			case 259:
			{
				MSGOUT(SCI_ERRMSG259);
			}
			break;
			case 260:
			{
				MSGOUT(SCI_ERRMSG260);
			}
			break;
			case 261:
			{
				MSGOUT(SCI_ERRMSG261);
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
				MSGOUT(SCI_ERRMSG265);
			}
			break;
			case 266:
			{
				MSGOUT(SCI_ERRMSG266);
			}
			break;
			case 267:
			{
				MSGOUT(SCI_ERRMSG267);
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
				MSGOUT(SCI_ERRMSG272);
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
				MSGOUT(SCI_ERRMSG275);
			}
			break;
			case 276:
			{
				MSGOUT(SCI_ERRMSG276);
			}
			break;
			case 277:
			{
				MSGOUT(SCI_ERRMSG277);
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
