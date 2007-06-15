/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "warningmode.h"
#include "message_scilab.h"
#include "sciprint.h"
#include "stack-def.h" /* C2F(cha1) */
#include "stack-c.h" /* Fin */
#include "MALLOC.h"
#include "msgs.h"
#include "../../localization/includes/QueryStringMessage.h"
#include "cvstr.h"
/*-----------------------------------------------------------------------------------*/
#ifdef BUF
#undef BUF
#endif
#define BUF C2F(cha1).buf
/*-----------------------------------------------------------------------------------*/
extern int C2F(showstack)();
extern int C2F(error)(int *);
/*-----------------------------------------------------------------------------------*/
static int msg_1(integer *n, integer *ierr);
static int msg_2(integer *n, integer *ierr);
static int msg_3(integer *n, integer *ierr);
static int msg_4(integer *n, integer *ierr);
static int msg_5(integer *n, integer *ierr);
static int msg_6(integer *n, integer *ierr);
static int msg_7(integer *n, integer *ierr);
static int msg_8(integer *n, integer *ierr);
static int msg_9(integer *n, integer *ierr);
static int msg_10(integer *n, integer *ierr);
static int msg_11(integer *n, integer *ierr);
static int msg_12(integer *n, integer *ierr);
static int msg_13(integer *n, integer *ierr);
static int msg_14(integer *n, integer *ierr);
static int msg_15(integer *n, integer *ierr);
static int msg_16(integer *n, integer *ierr);
static int msg_17(integer *n, integer *ierr);
static int msg_18(integer *n, integer *ierr);
static int msg_19(integer *n, integer *ierr);
static int msg_20(integer *n, integer *ierr);
static int msg_21(integer *n, integer *ierr);
static int msg_22(integer *n, integer *ierr);
static int msg_23(integer *n, integer *ierr);
static int msg_24(integer *n, integer *ierr);
static int msg_25(integer *n, integer *ierr);
static int msg_26(integer *n, integer *ierr);
static int msg_27(integer *n, integer *ierr);
static int msg_28(integer *n, integer *ierr);
static int msg_29(integer *n, integer *ierr);
static int msg_30(integer *n, integer *ierr);
static int msg_31(integer *n, integer *ierr);
static int msg_32(integer *n, integer *ierr);
static int msg_33(integer *n, integer *ierr);
static int msg_34(integer *n, integer *ierr);
static int msg_35(integer *n, integer *ierr);
static int msg_36(integer *n, integer *ierr);
static int msg_37(integer *n, integer *ierr);
static int msg_38(integer *n, integer *ierr);
static int msg_39(integer *n, integer *ierr);
static int msg_40(integer *n, integer *ierr);
static int msg_41(integer *n, integer *ierr);
static int msg_42(integer *n, integer *ierr);
static int msg_43(integer *n, integer *ierr);
static int msg_44(integer *n, integer *ierr);
static int msg_45(integer *n, integer *ierr);
static int msg_46(integer *n, integer *ierr);
static int msg_47(integer *n, integer *ierr);
static int msg_48(integer *n, integer *ierr);
static int msg_49(integer *n, integer *ierr);
static int msg_50(integer *n, integer *ierr);
static int msg_51(integer *n, integer *ierr);
static int msg_52(integer *n, integer *ierr);
static int msg_53(integer *n, integer *ierr);
static int msg_54(integer *n, integer *ierr);
static int msg_55(integer *n, integer *ierr);
static int msg_56(integer *n, integer *ierr);
static int msg_57(integer *n, integer *ierr);
static int msg_58(integer *n, integer *ierr);
static int msg_59(integer *n, integer *ierr);
static int msg_60(integer *n, integer *ierr);
static int msg_61(integer *n, integer *ierr);
static int msg_62(integer *n, integer *ierr);
static int msg_63(integer *n, integer *ierr);
static int msg_64(integer *n, integer *ierr);
static int msg_65(integer *n, integer *ierr);
static int msg_66(integer *n, integer *ierr);
static int msg_67(integer *n, integer *ierr);
static int msg_68(integer *n, integer *ierr);
static int msg_69(integer *n, integer *ierr);
static int msg_70(integer *n, integer *ierr);
static int msg_71(integer *n, integer *ierr);
static int msg_72(integer *n, integer *ierr);
static int msg_73(integer *n, integer *ierr);
static int msg_74(integer *n, integer *ierr);
static int msg_75(integer *n, integer *ierr);
static int msg_76(integer *n, integer *ierr);
static int msg_77(integer *n, integer *ierr);
static int msg_78(integer *n, integer *ierr);
static int msg_79(integer *n, integer *ierr);
static int msg_80(integer *n, integer *ierr);
static int msg_81(integer *n, integer *ierr);
static int msg_82(integer *n, integer *ierr);
static int msg_83(integer *n, integer *ierr);
static int msg_84(integer *n, integer *ierr);
static int msg_85(integer *n, integer *ierr);
static int msg_86(integer *n, integer *ierr);
static int msg_87(integer *n, integer *ierr);
static int msg_88(integer *n, integer *ierr);
static int msg_89(integer *n, integer *ierr);
static int msg_90(integer *n, integer *ierr);
static int msg_91(integer *n, integer *ierr);
static int msg_92(integer *n, integer *ierr);
static int msg_93(integer *n, integer *ierr);
static int msg_94(integer *n, integer *ierr);
static int msg_95(integer *n, integer *ierr);
static int msg_96(integer *n, integer *ierr);
static int msg_97(integer *n, integer *ierr);
static int msg_98(integer *n, integer *ierr);
static int msg_99(integer *n, integer *ierr);
static int msg_100(integer *n, integer *ierr);
static int msg_101(integer *n, integer *ierr);
static int msg_102(integer *n, integer *ierr);
static int msg_103(integer *n, integer *ierr);
static int msg_104(integer *n, integer *ierr);
static int msg_105(integer *n, integer *ierr);
static int msg_106(integer *n, integer *ierr);
static int msg_107(integer *n, integer *ierr);
static int msg_108(integer *n, integer *ierr);
static int msg_109(integer *n, integer *ierr);
static int msg_110(integer *n, integer *ierr);
static int msg_default(integer *n, integer *ierr);
/*-----------------------------------------------------------------------------------*/
int C2F(msgs)(integer *n, integer *ierr)
{
	if ( getWarningMode() )
	{
		switch (*n)
		{
		case 1:
			msg_1(n,ierr);
			break;
		case 2:
			msg_2(n,ierr);
			break;
		case 3:
			msg_3(n,ierr);
			break;
		case 4:
			msg_4(n,ierr);
			break;
		case 5:
			msg_5(n,ierr);
			break;
		case 6:
			msg_6(n,ierr);
			break;
		case 7:
			msg_7(n,ierr);
			break;
		case 8:
			msg_8(n,ierr);
			break;
		case 9:
			msg_9(n,ierr);
			break;
		case 10:
			msg_10(n,ierr);
			break;
		case 11:
			msg_11(n,ierr);
			break;
		case 12:
			msg_12(n,ierr);
			break;
		case 13:
			msg_13(n,ierr);
			break;
		case 14:
			msg_14(n,ierr);
			break;
		case 15:
			msg_15(n,ierr);
			break;
		case 16:
			msg_16(n,ierr);
			break;
		case 17:
			msg_17(n,ierr);
			break;
		case 18:
			msg_18(n,ierr);
			break;
		case 19:
			msg_19(n,ierr);
			break;
		case 20:
			msg_20(n,ierr);
			break;
		case 21:
			msg_21(n,ierr);
			break;
		case 22:
			msg_22(n,ierr);
			break;
		case 23:
			msg_23(n,ierr);
			break;
		case 24:
			msg_24(n,ierr);
			break;
		case 25:
			msg_25(n,ierr);
			break;
		case 26:
			msg_26(n,ierr);
			break;
		case 27:
			msg_27(n,ierr);
			break;
		case 28:
			msg_28(n,ierr);
			break;
		case 29:
			msg_29(n,ierr);
			break;
		case 30:
			msg_30(n,ierr);
			break;
		case 31:
			msg_31(n,ierr);
			break;
		case 32:
			msg_32(n,ierr);
			break;
		case 33:
			msg_33(n,ierr);
			break;
		case 34:
			msg_34(n,ierr);
			break;
		case 35:
			msg_35(n,ierr);
			break;
		case 36:
			msg_36(n,ierr);
			break;
		case 37:
			msg_37(n,ierr);
			break;
		case 38:
			msg_38(n,ierr);
			break;
		case 39:
			msg_39(n,ierr);
			break;
		case 40:
			msg_40(n,ierr);
			break;
		case 41:
			msg_41(n,ierr);
			break;
		case 42:
			msg_42(n,ierr);
			break;
		case 43:
			msg_43(n,ierr);
			break;
		case 44:
			msg_44(n,ierr);
			break;
		case 45:
			msg_45(n,ierr);
			break;
		case 46:
			msg_46(n,ierr);
			break;
		case 47:
			msg_47(n,ierr);
			break;
		case 48:
			msg_48(n,ierr);
			break;
		case 49:
			msg_49(n,ierr);
			break;
		case 50:
			msg_50(n,ierr);
			break;
		case 51:
			msg_51(n,ierr);
			break;
		case 52:
			msg_52(n,ierr);
			break;
		case 53:
			msg_53(n,ierr);
			break;
		case 54:
			msg_54(n,ierr);
			break;
		case 55:
			msg_55(n,ierr);
			break;
		case 56:
			msg_56(n,ierr);
			break;
		case 57:
			msg_57(n,ierr);
			break;
		case 58:
			msg_58(n,ierr);
			break;
		case 59:
			msg_59(n,ierr);
			break;
		case 60:
			msg_60(n,ierr);
			break;
		case 61:
			msg_61(n,ierr);
			break;
		case 62:
			msg_62(n,ierr);
			break;
		case 63:
			msg_63(n,ierr);
			break;
		case 64:
			msg_64(n,ierr);
			break;
		case 65:
			msg_65(n,ierr);
			break;
		case 66:
			msg_66(n,ierr);
			break;
		case 67:
			msg_67(n,ierr);
			break;
		case 68:
			msg_68(n,ierr);
			break;
		case 69:
			msg_69(n,ierr);
			break;
		case 70:
			msg_70(n,ierr);
			break;
		case 71:
			msg_71(n,ierr);
			break;
		case 72:
			msg_72(n,ierr);
			break;
		case 73:
			msg_73(n,ierr);
			break;
		case 74:
			msg_74(n,ierr);
			break;
		case 75:
			msg_75(n,ierr);
			break;
		case 76:
			msg_76(n,ierr);
			break;
		case 77:
			msg_77(n,ierr);
			break;
		case 78:
			msg_78(n,ierr);
			break;
		case 79:
			msg_79(n,ierr);
			break;
		case 80:
			msg_80(n,ierr);
			break;
		case 81:
			msg_81(n,ierr);
			break;
		case 82:
			msg_82(n,ierr);
			break;
		case 83:
			msg_83(n,ierr);
			break;
		case 84:
			msg_84(n,ierr);
			break;
		case 85:
			msg_85(n,ierr);
			break;
		case 86:
			msg_86(n,ierr);
			break;
		case 87:
			msg_87(n,ierr);
			break;
		case 88:
			msg_88(n,ierr);
			break;
		case 89:
			msg_89(n,ierr);
			break;
		case 90:
			msg_90(n,ierr);
			break;
		case 91:
			msg_91(n,ierr);
			break;
		case 92:
			msg_92(n,ierr);
			break;
		case 93:
			msg_93(n,ierr);
			break;
		case 94:
			msg_94(n,ierr);
			break;
		case 95:
			msg_95(n,ierr);
			break;
		case 96:
			msg_96(n,ierr);
			break;
		case 97:
			msg_97(n,ierr);
			break;
		case 98:
			msg_98(n,ierr);
			break;
		case 99:
			msg_99(n,ierr);
			break;
		case 100:
			msg_100(n,ierr);
			break;
		case 101:
			msg_101(n,ierr);
			break;
		case 102:
			msg_102(n,ierr);
			break;
		case 103:
			msg_103(n,ierr);
			break;
		case 104:
			msg_104(n,ierr);
			break;
		case 105:
			msg_105(n,ierr);
			break;
		case 106:
			msg_106(n,ierr);
			break;
		case 107:
			msg_107(n,ierr);
			break;
		case 108:
			msg_108(n,ierr);
			break;
		case 109:
			msg_109(n,ierr);
			break;
		case 110:
			msg_110(n,ierr);
			break;
		default:
			msg_default(n,ierr);
			break;
		}
	}
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_1(integer *n, integer *ierr)
{
	message_scilab("core_message_1");
	message_scilab("core_message_2");
	if (*ierr > 0)
	{
		message_scilab("core_message_3",*ierr);
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_2(integer *n, integer *ierr)
{
	message_scilab("core_message_1");
	message_scilab("core_message_4");

	if (*ierr > 0)
	{
		message_scilab("core_message_5",*ierr);
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_3(integer *n, integer *ierr)
{
	message_scilab("core_message_1");
	message_scilab("core_message_4");
	if (*ierr > 0)
	{
		message_scilab("core_message_6",*ierr);
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_4(integer *n, integer *ierr)
{
	message_scilab("core_message_7");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_5(integer *n, integer *ierr)
{
	char localbuf[14];
	strncpy(localbuf,BUF,13);
	localbuf[13]='\0';
	message_scilab("core_message_1");
	message_scilab("core_message_8",localbuf);

	if (*ierr > 0)
	{
		message_scilab("core_message_9");
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_6(integer *n, integer *ierr)
{
	char localbuf[14];
	strncpy(localbuf,BUF,13);
	localbuf[13]='\0';
	message_scilab("core_message_1");
	message_scilab("core_message_10");
	message_scilab("core_message_11",localbuf);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_7(integer *n, integer *ierr)
{
	message_scilab("core_message_12");
	C2F(showstack)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_8(integer *n, integer *ierr)
{
	message_scilab("core_message_13");
	C2F(showstack)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_9(integer *n, integer *ierr)
{
	message_scilab("core_message_14",ierr);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_10(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_11(integer *n, integer *ierr)
{
	message_scilab("core_message_15");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_12(integer *n, integer *ierr)
{
	char localbuf[16];
	strncpy(localbuf,BUF,15);
	message_scilab("core_message_16",localbuf);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_13(integer *n, integer *ierr)
{
	char localbuf[16];
	strncpy(localbuf,BUF,15);

	message_scilab("core_message_17",localbuf);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_14(integer *n, integer *ierr)
{
	message_scilab("core_message_18");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_15(integer *n, integer *ierr)
{
	message_scilab("core_message_19");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_16(integer *n, integer *ierr)
{
	message_scilab("core_message_20");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_17(integer *n, integer *ierr)
{
	message_scilab("core_message_21");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_18(integer *n, integer *ierr)
{
	message_scilab("core_message_22");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_19(integer *n, integer *ierr)
{
	message_scilab("core_message_23");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_20(integer *n, integer *ierr)
{
	message_scilab("core_message_24");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_21(integer *n, integer *ierr)
{
	message_scilab("core_message_25");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_22(integer *n, integer *ierr)
{
	char localbuf[5];
	strncpy(localbuf,BUF,4);
	message_scilab("core_message_26",localbuf);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_23(integer *n, integer *ierr)
{
	message_scilab("core_message_27");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_24(integer *n, integer *ierr)
{
	char *localbuf=NULL;
	if (*ierr >0)
	{
		localbuf=(char*)MALLOC(sizeof(char)*(*ierr +1));
		strncpy(localbuf,BUF,*ierr);
		message_scilab("core_message_28",localbuf);
		if (localbuf) {FREE(localbuf);localbuf=NULL;}
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_25(integer *n, integer *ierr)
{
	char *localbuf=NULL;
	if (*ierr >0)
	{
		localbuf=(char*)MALLOC(sizeof(char)*(*ierr +1));
		strncpy(localbuf,BUF,*ierr);
		message_scilab("core_message_29",localbuf);
		if (localbuf) {FREE(localbuf);localbuf=NULL;}
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_26(integer *n, integer *ierr)
{
	message_scilab("core_message_30");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_27(integer *n, integer *ierr)
{
	char localbuf[nlgh+1];
	strncpy(localbuf,BUF,nlgh-1);
	localbuf[nlgh-1]='\0';
	message_scilab("core_message_31",localbuf);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_28(integer *n, integer *ierr)
{
	char localbuf[4];
	localbuf[0]=BUF[10];
	localbuf[1]=BUF[11];
	localbuf[2]=BUF[12];
	localbuf[3]='\0';
	message_scilab("core_message_32",localbuf);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_29(integer *n, integer *ierr)
{
	char localbuf[nlgh+1];
	strncpy(localbuf,BUF,nlgh);
	message_scilab("core_message_33",localbuf);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_30(integer *n, integer *ierr)
{
	message_scilab("core_message_34");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_31(integer *n, integer *ierr)
{
	int cerr=-1;
	message_scilab("core_message_35");
	C2F(error)(&cerr);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_32(integer *n, integer *ierr)
{
	char localbuf1[6];
	char localbuf2[nlgh];
	int l=nlgh+1;

	strncpy(localbuf1,&BUF[nlgh+1],5);
	localbuf1[5]='\0';
	do 
	{
		--l;
	} while(BUF[l-1]==' ');
	
	strncpy(localbuf2,BUF,l);
	localbuf2[l]='\0';
	message_scilab("core_message_36",localbuf1,localbuf2);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_33(integer *n, integer *ierr)
{
	char *localbuf=NULL;
	integer nreal=0;
	integer c__1 = 1;
	nreal = C2F(iop).lpt[3] - C2F(iop).lpt[2];
	C2F(cvstr)(&nreal, &C2F(iop).lin[C2F(iop).lpt[2] - 2], BUF, &c__1, (long int)4096);

	localbuf=(char*)MALLOC(sizeof(char)*(nreal +1));
	strncpy(localbuf,BUF,nreal);
	message_scilab("core_message_1");
	message_scilab("core_message_37",localbuf);
	localbuf[nlgh]='\0';
	message_scilab("core_message_38",localbuf);
	if (localbuf) {FREE(localbuf);localbuf=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_34(integer *n, integer *ierr)
{
	message_scilab("core_message_39");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_35(integer *n, integer *ierr)
{
	message_scilab("core_message_40");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_36(integer *n, integer *ierr)
{
	message_scilab("core_message_41",*ierr);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_37(integer *n, integer *ierr)
{
	char localbuf1[4];
	char localbuf2[14];

	strncpy(localbuf1,BUF,3);
	strncpy(localbuf2,&BUF[4],13);
	message_scilab("core_message_42",localbuf1,localbuf2);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_38(integer *n, integer *ierr)
{
	message_scilab("core_message_43");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_39(integer *n, integer *ierr)
{
	char localbuf1[10];
	char localbuf2[10];

	strncpy(localbuf1,BUF,9);
	strncpy(localbuf2,&BUF[10],9);

	message_scilab("core_message_44",localbuf1,localbuf2);

	strncpy(localbuf1,&BUF[20],9);
	strncpy(localbuf2,&BUF[30],9);
	message_scilab("core_message_45",localbuf1,localbuf2);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_40(integer *n, integer *ierr)
{
	message_scilab("core_message_46");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_41(integer *n, integer *ierr)
{
	message_scilab("core_message_47");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_42(integer *n, integer *ierr)
{
	integer p=0;
	integer lk=0;
	integer k=0;
	integer km=0;
	integer c__1=1;
	char line[140];
	char *lMSG=QueryStringMessage("core_message_48");

	C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6], line, &c__1, (long int)24);
	line[24]='\0';
	
	if (lMSG)
	{
		sciprint(lMSG,line);
		sciprint("\n");
	}
	if (lMSG) {FREE(lMSG);lMSG=NULL;}

	p = C2F(recu).pt + 1;

L42_1:
	--p;
	if (p == 0) goto end_msg_42;
	
	if (C2F(recu).rstk[p - 1] != 502) goto L42_1;
	k = C2F(iop).lpt[0] - 19;

	/* recherche du nom de la macro correspondant a ce niveau */
	lk = C2F(iop).lin[k + 5] / 2 + 1;
	if (lk <= C2F(vstk).lstk[C2F(vstk).top]) goto end_msg_42;
	km = C2F(iop).lin[k + 4] - 1;
L42_2:
	++km;
	if (km > C2F(vstk).isiz) goto end_msg_42;
	if (C2F(vstk).lstk[km - 1] != lk) goto L42_2;

	C2F(cvname)(&C2F(vstk).idstk[km * 6 - 6], line, &c__1, (long int)24);

	message_scilab("core_message_49",line);
end_msg_42:
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_43(integer *n, integer *ierr)
{
	message_scilab("core_message_50");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_44(integer *n, integer *ierr)
{
	message_scilab("core_message_51");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_45(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_46(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_47(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_48(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_49(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_50(integer *n, integer *ierr)
{
	message_scilab("core_message_52");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_51(integer *n, integer *ierr)
{
	message_scilab("core_message_53");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_52(integer *n, integer *ierr)
{
	message_scilab("core_message_54");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_53(integer *n, integer *ierr)
{
	message_scilab("core_message_55");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_54(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_55(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_56(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_57(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_58(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_59(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_60(integer *n, integer *ierr)
{
	message_scilab("core_message_56");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_61(integer *n, integer *ierr)
{
	char line[140];
	integer c__1=1;
	C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6], line, &c__1,24);
	message_scilab("core_message_1");
	message_scilab("core_message_57",line);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_62(integer *n, integer *ierr)
{
	message_scilab("core_message_58");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_63(integer *n, integer *ierr)
{
	message_scilab("core_message_59");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_64(integer *n, integer *ierr)
{
	message_scilab("core_message_60");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_65(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_66(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_67(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_68(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_69(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_70(integer *n, integer *ierr)
{
	char localbuf[10];
	strncpy(localbuf,BUF,10);
	message_scilab("core_message_61",localbuf);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_71(integer *n, integer *ierr)
{
	message_scilab("core_message_62");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_72(integer *n, integer *ierr)
{
	message_scilab("core_message_63");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_73(integer *n, integer *ierr)
{
	message_scilab("core_message_64");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_74(integer *n, integer *ierr)
{
	message_scilab("core_message_65");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_75(integer *n, integer *ierr)
{
	message_scilab("core_message_66");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_76(integer *n, integer *ierr)
{
	message_scilab("core_message_67");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_77(integer *n, integer *ierr)
{
	message_scilab("core_message_68");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_78(integer *n, integer *ierr)
{
	message_scilab("core_message_69");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_79(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_80(integer *n, integer *ierr)
{
	message_scilab("core_message_70");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_81(integer *n, integer *ierr)
{
	message_scilab("core_message_71");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_82(integer *n, integer *ierr)
{
	message_scilab("core_message_72");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_83(integer *n, integer *ierr)
{
	message_scilab("core_message_73");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_84(integer *n, integer *ierr)
{
	message_scilab("core_message_74");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_85(integer *n, integer *ierr)
{
	message_scilab("core_message_75");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_86(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_87(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_88(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_89(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_90(integer *n, integer *ierr)
{
	message_scilab("core_message_76");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_91(integer *n, integer *ierr)
{
	message_scilab("core_message_77");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_92(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_93(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_94(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_95(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_96(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_97(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_98(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_99(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_100(integer *n, integer *ierr)
{
	message_scilab("core_message_78");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_101(integer *n, integer *ierr)
{
	integer c__1=1;
	int i=0;
	char line[140];

	C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6], line, &c__1, (long int)24);

	for (i=0;i<(int)strlen(line);i++)
	{
		if (line[i]==' ') line[i]='\0';
		break;
	}

	message_scilab("core_message_79",line);
	message_scilab("core_message_80");
	C2F(showstack)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_102(integer *n, integer *ierr)
{
	integer c__1=1;
	int i=0;
	char line[140];

	C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * 6 - 6], line, &c__1, (long int)24);

	for (i=0;i<(int)strlen(line);i++)
	{
		if (line[i]==' ') line[i]='\0';
		break;
	}

	message_scilab("core_message_81",line);
	message_scilab("core_message_82");

	C2F(showstack)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_103(integer *n, integer *ierr)
{
	message_scilab("core_message_83");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_104(integer *n, integer *ierr)
{
	message_scilab("core_message_84");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_105(integer *n, integer *ierr)
{
	char *localbuf=NULL;
	localbuf=(char*)MALLOC(sizeof(char)*(*ierr +1));
	strncpy(localbuf,BUF,*ierr);
	message_scilab("core_message_85",localbuf);
	if (localbuf) {FREE(localbuf);localbuf=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_106(integer *n, integer *ierr)
{
	message_scilab("core_message_86");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_107(integer *n, integer *ierr)
{
	message_scilab("core_message_87");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_108(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_109(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_110(integer *n, integer *ierr)
{
	sciprint(" \n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int msg_default(integer *n, integer *ierr)
{
	char *localbuf=NULL;
	int len=strlen(BUF);
	int i=0;
	localbuf=(char*)MALLOC(sizeof(char)*(len+1));
	strncpy(localbuf,BUF,len);
	sciprint(" Warning: \n");
	for (i=0;i<len;i++)
	{
		if (localbuf[i] == ' ')
		{
			localbuf[i]='\0';
			break;
		}
	}
	sciprint("%s\n",localbuf);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
