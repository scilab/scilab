/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "warningmode.h"
#include "sciprint.h"
#include "stack-def.h" /* C2F(cha1) */
#include "stack-c.h" /* Fin */
#include "MALLOC.h"
#include "msgs.h"
#include "localization.h"
#include "cvstr.h"
#include "do_error_number.h"
#include <stdio.h>
/*--------------------------------------------------------------------------*/
#ifdef BUF
#undef BUF
#endif
#define BUF C2F(cha1).buf
/*--------------------------------------------------------------------------*/
extern int C2F(showstack)(void);
/*--------------------------------------------------------------------------*/
static int msg_1(int *n, int *ierr);
static int msg_2(int *n, int *ierr);
static int msg_3(int *n, int *ierr);
static int msg_4(int *n, int *ierr);
static int msg_5(int *n, int *ierr);
static int msg_6(int *n, int *ierr);
static int msg_7(int *n, int *ierr);
static int msg_8(int *n, int *ierr);
static int msg_9(int *n, int *ierr);
static int msg_10(int *n, int *ierr);
static int msg_11(int *n, int *ierr);
static int msg_12(int *n, int *ierr);
static int msg_13(int *n, int *ierr);
static int msg_14(int *n, int *ierr);
static int msg_15(int *n, int *ierr);
static int msg_16(int *n, int *ierr);
static int msg_17(int *n, int *ierr);
static int msg_18(int *n, int *ierr);
static int msg_19(int *n, int *ierr);
static int msg_20(int *n, int *ierr);
static int msg_21(int *n, int *ierr);
static int msg_22(int *n, int *ierr);
static int msg_23(int *n, int *ierr);
static int msg_24(int *n, int *ierr);
static int msg_25(int *n, int *ierr);
static int msg_26(int *n, int *ierr);
static int msg_27(int *n, int *ierr);
static int msg_28(int *n, int *ierr);
static int msg_29(int *n, int *ierr);
static int msg_30(int *n, int *ierr);
static int msg_31(int *n, int *ierr);
static int msg_32(int *n, int *ierr);
static int msg_33(int *n, int *ierr);
static int msg_34(int *n, int *ierr);
static int msg_35(int *n, int *ierr);
static int msg_36(int *n, int *ierr);
static int msg_37(int *n, int *ierr);
static int msg_38(int *n, int *ierr);
static int msg_39(int *n, int *ierr);
static int msg_40(int *n, int *ierr);
static int msg_41(int *n, int *ierr);
static int msg_42(int *n, int *ierr);
static int msg_43(int *n, int *ierr);
static int msg_44(int *n, int *ierr);
static int msg_45(int *n, int *ierr);
static int msg_46(int *n, int *ierr);
static int msg_47(int *n, int *ierr);
static int msg_48(int *n, int *ierr);
static int msg_49(int *n, int *ierr);
static int msg_50(int *n, int *ierr);
static int msg_51(int *n, int *ierr);
static int msg_52(int *n, int *ierr);
static int msg_53(int *n, int *ierr);
static int msg_54(int *n, int *ierr);
static int msg_55(int *n, int *ierr);
static int msg_56(int *n, int *ierr);
static int msg_57(int *n, int *ierr);
static int msg_58(int *n, int *ierr);
static int msg_59(int *n, int *ierr);
static int msg_60(int *n, int *ierr);
static int msg_61(int *n, int *ierr);
static int msg_62(int *n, int *ierr);
static int msg_63(int *n, int *ierr);
static int msg_64(int *n, int *ierr);
static int msg_65(int *n, int *ierr);
static int msg_66(int *n, int *ierr);
static int msg_67(int *n, int *ierr);
static int msg_68(int *n, int *ierr);
static int msg_69(int *n, int *ierr);
static int msg_70(int *n, int *ierr);
static int msg_71(int *n, int *ierr);
static int msg_72(int *n, int *ierr);
static int msg_73(int *n, int *ierr);
static int msg_74(int *n, int *ierr);
static int msg_75(int *n, int *ierr);
static int msg_76(int *n, int *ierr);
static int msg_77(int *n, int *ierr);
static int msg_78(int *n, int *ierr);
static int msg_79(int *n, int *ierr);
static int msg_80(int *n, int *ierr);
static int msg_81(int *n, int *ierr);
static int msg_82(int *n, int *ierr);
static int msg_83(int *n, int *ierr);
static int msg_84(int *n, int *ierr);
static int msg_85(int *n, int *ierr);
static int msg_86(int *n, int *ierr);
static int msg_87(int *n, int *ierr);
static int msg_88(int *n, int *ierr);
static int msg_89(int *n, int *ierr);
static int msg_90(int *n, int *ierr);
static int msg_91(int *n, int *ierr);
static int msg_92(int *n, int *ierr);
static int msg_93(int *n, int *ierr);
static int msg_94(int *n, int *ierr);
static int msg_95(int *n, int *ierr);
static int msg_96(int *n, int *ierr);
static int msg_97(int *n, int *ierr);
static int msg_98(int *n, int *ierr);
static int msg_99(int *n, int *ierr);
static int msg_100(int *n, int *ierr);
static int msg_101(int *n, int *ierr);
static int msg_102(int *n, int *ierr);
static int msg_103(int *n, int *ierr);
static int msg_104(int *n, int *ierr);
static int msg_105(int *n, int *ierr);
static int msg_106(int *n, int *ierr);
static int msg_107(int *n, int *ierr);
static int msg_108(int *n, int *ierr);
static int msg_109(int *n, int *ierr);
static int msg_110(int *n, int *ierr);
static int msg_111(int *n, int *ierr);
static int msg_112(int *n, int *ierr);
static int msg_113(int *n, int *ierr);
static int msg_114(int *n, int *ierr);
static int msg_115(int *n, int *ierr);

static int msg_default(int *n, int *ierr);
/*--------------------------------------------------------------------------*/
int C2F(msgs)(int *n, int *ierr)
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
		case 111:
			msg_111(n,ierr);
			break;
		case 112:
			msg_112(n,ierr);
			break;
		case 113:
			msg_113(n,ierr);
			break;
		case 114:
			msg_114(n,ierr);
			break;
		case 115:
			msg_115(n,ierr);
			break;
		default:
			msg_default(n,ierr);
			break;
		}
	sciprint("\n");
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_1(int *n, int *ierr)
{
	sciprint(_("Warning :\n"));
	sciprint(_("Non convergence in the QZ algorithm.\n"));
	if (*ierr > 0)
	{
		sciprint(_("The top %d  x %d blocks may not be in generalized Schur form.\n"),*ierr);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_2(int *n, int *ierr)
{
	sciprint(_("Warning :\n"));
	sciprint(_("Non convergence in QR steps.\n"));

	if (*ierr > 0)
	{
		sciprint(_("The top %d x %d block may not be in Schur form.\n"),*ierr);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_3(int *n, int *ierr)
{
	sciprint(_("Warning :\n"));
	sciprint(_("Non convergence in QR steps.\n"));
	if (*ierr > 0)
	{
		sciprint(_("The first %d singular values may be incorrect.\n"),*ierr);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_4(int *n, int *ierr)
{
	sciprint(_("Warning:  Result may be inaccurate.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_5(int *n, int *ierr)
{
	char localbuf[14];
	strncpy(localbuf,BUF,13); // 0>12
	localbuf[13]='\0';
	sciprint(_("Warning :\n"));
	sciprint(_("matrix is close to singular or badly scaled. rcond = %s\n"),localbuf);

	if (*ierr > 0)
	{
		sciprint(_("computing least squares solution. (see lsq).\n"));
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_6(int *n, int *ierr)
{
	char localbuf[14];
	strncpy(localbuf,BUF,13); // 0>12
	localbuf[13]='\0';
	sciprint(_("Warning :\n"));
	sciprint(_("eigenvectors are badly conditioned.\n"));
	sciprint(_("results may be inaccurate. rcond = %s\n"),localbuf);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_7(int *n, int *ierr)
{
	sciprint(_("Warning: obsolete use of '=' instead of '=='.\n"));
	C2F(showstack)();
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_8(int *n, int *ierr)
{
	sciprint(_("Warning: obsolete use of eye rand or ones.\n"));
	C2F(showstack)();
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_9(int *n, int *ierr)
{
	sciprint(_("Rank defficient. rank = %d\n"),*ierr);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_10(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_11(int *n, int *ierr)
{
	sciprint(_("Quapro encounters cycles on degenerate point.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_12(int *n, int *ierr)
{
        // Copy BUF (src, with size 4096) into localbuf (dest, with size 16)
        // man strncpy :
        // "Thus, if there is no null byte among the first n bytes of src, the result will not be null-terminated."
        // Obviously, the src string does not contain \0, since it comes from Fortran.
        char localbuf[16];// 0>15
        strncpy(localbuf,BUF,15);// 0>14
        localbuf[15]='\0';
	sciprint(_("Norm of projected gradient lower than %s.\n"),localbuf);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_13(int *n, int *ierr)
{
        // Same comment as for msg_12
	char localbuf[16];
	strncpy(localbuf,BUF,15);
        localbuf[15]='\0';
	sciprint(_("at last iteration f decreases by less than %s.\n"),localbuf);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_14(int *n, int *ierr)
{
	sciprint(_("Optimization stops because too small variations for x.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_15(int *n, int *ierr)
{
	sciprint(_("Optim stops: maximum number of calls to f is reached.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_16(int *n, int *ierr)
{
	sciprint(_("Optim stops: maximum number of iterations is reached.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_17(int *n, int *ierr)
{
	sciprint(_("Optim stops: too small variations in gradient direction.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_18(int *n, int *ierr)
{
	sciprint(_("Stop during calculation of descent direction.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_19(int *n, int *ierr)
{
	sciprint(_("Stop during calculation of estimated hessian.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_20(int *n, int *ierr)
{
	sciprint(_("End of optimization.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_21(int *n, int *ierr)
{
	sciprint(_("End of optimization (linear search fails).\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_22(int *n, int *ierr)
{
	char localbuf[5];
	strncpy(localbuf,BUF,4);
	sciprint(_("sfact : uncomplete convergence relative precision reached : 10**(%s).\n"),localbuf);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_23(int *n, int *ierr)
{
	sciprint(_("Help file inconsistent...\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_24(int *n, int *ierr)
{
	char *localbuf=NULL;
	if (*ierr >0)
	{
		localbuf=(char*)MALLOC(sizeof(char)*(*ierr +1));
		strncpy(localbuf,BUF,*ierr);
		sciprint(_("Functions files location : %s.\n"),localbuf);
		if (localbuf) {FREE(localbuf);localbuf=NULL;}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_25(int *n, int *ierr)
{
	char *localbuf=NULL;
	if (*ierr >0)
	{
		localbuf=(char*)MALLOC(sizeof(char)*(*ierr +1));
		strncpy(localbuf,BUF,*ierr);
		sciprint("    : %s.\n",localbuf);
		if (localbuf) {FREE(localbuf);localbuf=NULL;}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_26(int *n, int *ierr)
{
	sciprint(_("Pause mode: enter empty lines to continue.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_27(int *n, int *ierr)
{
	char localbuf[nlgh+1];
	strncpy(localbuf,BUF,nlgh-1);
	localbuf[nlgh-1]='\0';
	sciprint(_("Breakpoints of function : %s\n"),localbuf);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_28(int *n, int *ierr)
{
	char localbuf[4];
	localbuf[0]=BUF[10];
	localbuf[1]=BUF[11];
	localbuf[2]=BUF[12];
	localbuf[3]='\0';
	sciprint(_("The top %d  x %d blocks may not be in generalized Schur form.\n"),localbuf);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_29(int *n, int *ierr)
{
	char localbuf[nlgh+1];
	strncpy(localbuf,BUF,nlgh);
	sciprint(_("The top %d  x %d blocks may not be in generalized Schur form.\n"),localbuf);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_30(int *n, int *ierr)
{
	sciprint(_("Warning: recursion problem..., cleared.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_31(int *n, int *ierr)
{
    /*int cerr=-1;*/
	sciprint(_("Warning: stack problem..., cleared.\n"));
	/*Error(cerr);*/
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_32(int *n, int *ierr)
{
	char localbuf1[nsiz];
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
	sciprint(_("Stop after row %s in function %s.\n"),localbuf1,localbuf2);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_33(int *n, int *ierr)
{
	char *localbuf=NULL;
	int nreal=0;
	int one = 1;
	nreal = C2F(iop).lpt[3] - C2F(iop).lpt[2];
	C2F(cvstr)(&nreal, &C2F(iop).lin[C2F(iop).lpt[2] - 2], BUF, &one, (long int)bsiz);

	localbuf=(char*)MALLOC(sizeof(char)*(nreal +1));
	strncpy(localbuf,BUF,nreal);
	localbuf[nreal]='\0';
	sciprint(_("Warning :\n"));
	sciprint(_("The identifier : %s\n"),localbuf);
	localbuf[nlgh]='\0';
	sciprint(_(" has been truncated to: %s.\n"),localbuf);
	if (localbuf) {FREE(localbuf);localbuf=NULL;}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_34(int *n, int *ierr)
{
	sciprint(_("Real part\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_35(int *n, int *ierr)
{
	sciprint(_("Imaginary part\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_36(int *n, int *ierr)
{
	sciprint(_("Maximum size of buffer : %d characters.\n"),*ierr);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_37(int *n, int *ierr)
{
	char localbuf1[4];
	char localbuf2[14];

	strncpy(localbuf1,BUF,3);
	strncpy(localbuf2,&BUF[4],13);
	sciprint(_("Rank deficient : rank = %s  - tol = %s .\n"),localbuf1,localbuf2);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_38(int *n, int *ierr)
{
	sciprint(_("Your variables are:\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_39(int *n, int *ierr)
{
	char localbuf1[10];
	char localbuf2[10];

	strncpy(localbuf1,BUF,9);
	strncpy(localbuf2,&BUF[10],9);

	sciprint(_("Using %s elements out of %s.\n"),localbuf1,localbuf2);

	strncpy(localbuf1,&BUF[20],9);
	strncpy(localbuf2,&BUF[30],9);
	sciprint(_(" and %s variables out of %s.\n"),localbuf1,localbuf2);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_40(int *n, int *ierr)
{
	sciprint(_("System functions:\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_41(int *n, int *ierr)
{
	sciprint(_("Commands:\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_42(int *n, int *ierr)
{
	int p=0;
	int lk=0;
	int k=0;
	int km=0;
	int one=1;
	char line[140];

	C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], line, &one, (long int)nlgh);
	line[nlgh]='\0';

	sciprint(_("Warning : redefining function: %s. Use funcprot(0) to avoid this message"),line);
	sciprint("\n");

	p = C2F(recu).pt + 1;

L42_1:
	--p;
	if (p == 0) goto end_msg_42;

	/* @TODO what is 502 */
	if (C2F(recu).rstk[p - 1] != 502) goto L42_1;
	/* @TODO what is 19 */
	k = C2F(iop).lpt[0] - 19;

	/* recherche du nom de la macro correspondant a ce niveau */
	/* @TODO what is 5 or 2 + 1 */
	lk = C2F(iop).lin[k + 5] / 2 + 1;
	if (lk <= C2F(vstk).lstk[Top]) goto end_msg_42;
	/* @TODO what is 4 */
	km = C2F(iop).lin[k + 4] - 1;
L42_2:
	++km;
	if (km > C2F(vstk).isiz) goto end_msg_42;
	if (C2F(vstk).lstk[km - 1] != lk) goto L42_2;
	/* @TODO what is this ugly line ??? */
	C2F(cvname)(&C2F(vstk).idstk[km * nsiz - nsiz], line, &one, (long int)nlgh);

	sciprint(_("       inside function: %s.\n"),line);
end_msg_42:
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_43(int *n, int *ierr)
{
	sciprint(_("Not enough memory to perform simplification.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_44(int *n, int *ierr)
{
	sciprint(_("Your global variables are...\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_45(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_46(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_47(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_48(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_49(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_50(int *n, int *ierr)
{
	sciprint(_("%s: Loop on two orders detected.\n"),"arl2");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_51(int *n, int *ierr)
{
	sciprint(_("%s: Impossible to reach required order.\n   previous order computed solution returned.\n"),"arl2");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_52(int *n, int *ierr)
{
	sciprint(_("%s: Failure when looking for the intersection with domains boundaries.\n   previous order computed solution returned.\n"),"arl2");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_53(int *n, int *ierr)
{
	sciprint(_("%s: Too many solutions found.\n   previous order computed solution returned.\n"),"arl2");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_54(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_55(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_56(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_57(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_58(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_59(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_60(int *n, int *ierr)
{
	sciprint(_("Warning: loaded file has been created with a previous version of Scilab\nor you are trying to load a file saved on a different architecture.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_61(int *n, int *ierr)
{
	char line[bsiz];
	int one = 1;
	C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], line, &one,nlgh);
	sciprint(_("Warning:\n"));
	sciprint(_("Impossible to load variable %s.\n"),line);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_62(int *n, int *ierr)
{
	sciprint(_("Warning: Use of standard list to define typed structures\n          is obsolete. Use tlist.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_63(int *n, int *ierr)
{
	sciprint(_("Warning : division by zero...\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_64(int *n, int *ierr)
{
	sciprint(_("Warning : singularity of 'log' or 'tan' function.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_65(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_66(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_67(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_68(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_69(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_70(int *n, int *ierr)
{
	char localbuf[10];
	strncpy(localbuf,BUF,10);
	sciprint(_("At time: %s ,Too many iteration to achieve required precision.\n"),localbuf);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_71(int *n, int *ierr)
{
	sciprint(_("stepsize not significant in rkqc.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_72(int *n, int *ierr)
{
	sciprint(_("Warning: variable %%ODEOPTIONS not found.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_73(int *n, int *ierr)
{
	sciprint(_("Warning: integration up to tcrit.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_74(int *n, int *ierr)
{
	sciprint(_("Warning: integration not completed! check tolerance parameters or step size.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_75(int *n, int *ierr)
{
	sciprint(_("Warning: Jacobian external is given, but\n not used!,  see %ODEOPTIONS(6).\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_76(int *n, int *ierr)
{
	sciprint(_("Warning: No Jacobian external given but\n one is required by %ODEOPTIONS(6) value !\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_77(int *n, int *ierr)
{
	sciprint(_("itask=2,3 or 5: At most one value of t\n is allowed, the last element of t is used.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_78(int *n, int *ierr)
{
	sciprint(_("Warning: odedc forces itask=4 and handles\n tcrit.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_79(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_80(int *n, int *ierr)
{
	sciprint(_("Warning: function is already compiled.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_81(int *n, int *ierr)
{
	sciprint(_("%s: termination for lack of space to divide triangle.\n"),"int2d");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_82(int *n, int *ierr)
{
	sciprint(_("%s: termination because of roundoff noise.\n"),"int2d");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_83(int *n, int *ierr)
{
	sciprint(_("%s: termination for relative error < (5.0*%eps).\n"),"int2d");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_84(int *n, int *ierr)
{
	sciprint(_("%s: termination: function evaluations > MEVALS.\n"),"int2d");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_85(int *n, int *ierr)
{
	sciprint(_("%s: maxpts was too small to obtain the required accuracy.\n"),"int3d");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_86(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_87(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_88(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_89(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_90(int *n, int *ierr)
{
	sciprint(_("Too many input/output ports for hilited block.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_91(int *n, int *ierr)
{
	sciprint(_("Too many input/output entries for hilited block.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_92(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_93(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_94(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_95(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_96(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_97(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_98(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_99(int *n, int *ierr)
{
	sciprint("\n");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_100(int *n, int *ierr)
{
	sciprint(_("Undefined display for this data type.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_101(int *n, int *ierr)
{
	int one = 1;
	int i=0;
	char line[bsiz];

	C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], line, &one, (long int)nlgh);

	for (i=0;i<(int)strlen(line);i++)
	{
		if (line[i]==' ') line[i]='\0';
		break;
	}

	sciprint(_("Warning: primitive function %s has moved.\n"),line);
	sciprint(_("Rebuild your function libraries.\n"));
	C2F(showstack)();
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_102(int *n, int *ierr)
{
	int one = 1;
	int i=0;
	char line[bsiz];

	C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], line, &one, (long int)nlgh);

	for (i=0;i<(int)strlen(line);i++)
	{
		if (line[i]==' ') line[i]='\0';
		break;
	}

	sciprint(_("Warning: primitive function %s is now a Scilab function.\n"),line);
	sciprint(_("Rebuild your function libraries.\n"));

	C2F(showstack)();
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_103(int *n, int *ierr)
{
	sciprint(_("Roundoff errors make leading eigenvalues\n in the Schur form no longer satisfy criterion.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_104(int *n, int *ierr)
{
	sciprint(_("Formal variable name has been truncated.\n"));
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_105(int *n, int *ierr)
{
	char *localbuf=NULL;
	localbuf=(char*)MALLOC(sizeof(char)*(*ierr +1));
	strncpy(localbuf,BUF,*ierr);
	sciprint(_("Unknown key <%s> ignored.\n"),localbuf);
	if (localbuf) {FREE(localbuf);localbuf=NULL;}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_106(int *n, int *ierr)
{
	sciprint(_("%s: window dimensions have been set less than 2^16.\n"),"xset");
	return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_107(int *n, int *ierr)
{
  sciprint(_("Some data have not been computed they are replaced by NaN.\n"));
  return 0;
}
/*--------------------------------------------------------------------------*/
/* Messages for lsqrsolve */
/*--------------------------------------------------------------------------*/
static int msg_108(int *n, int *ierr)
{
  sciprint(_("%s: both actual and predicted relative reductions in the criterion at most %s.\n"),"lsqrsolve","ftol");
  return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_109(int *n, int *ierr)
{
  sciprint(_("%s: relative error between two consecutive iterates is at most %s.\n"),"lsqrsolve","xtol");
  return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_110(int *n, int *ierr)
{
  sciprint(_("%s: the cosine of the angle between %s and any column of the jacobian is at most %s in absolute value.\n"),"lsqrsolve","fvec","gtol");
  return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_111(int *n, int *ierr)
{
  sciprint(_("%s: Number of calls to %s has reached or exceeded %s.\n"),"lsqrsolve","fct","maxfev");
  return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_112(int *n, int *ierr)
{
  sciprint(_("%s: %s is too small. No further reduction in the criterion is possible.\n"),"lsqrsolve","ftol");
  return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_113(int *n, int *ierr)
{
  sciprint(_("%s: %s is too small. No further reduction in the criterion is possible.\n"),"lsqrsolve","xtol");
  return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_114(int *n, int *ierr)
{
  sciprint(_("%s: %s is too small. %s is orthogonal to the columns of the jacobian to machine precision.\n"),"lsqrsolve","gtol","fvec");
  return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_115(int *n, int *ierr)
{
	sciprint(_("poly: variable name must be lesser than 5 characters long.\n"));
  return 0;
}
/*--------------------------------------------------------------------------*/
static int msg_default(int *n, int *ierr)
{
	char *localbuf=NULL;
	int len=(int)strlen(BUF);
	int i=0;
	localbuf=(char*)MALLOC(sizeof(char)*(len+1));
	strncpy(localbuf,BUF,len);
	sciprint(_("Warning:\n"));
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
/*--------------------------------------------------------------------------*/
