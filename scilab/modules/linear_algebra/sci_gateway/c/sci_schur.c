
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intdgees0)(char *fname, unsigned long fname_len);
extern int C2F(intzgees0)(char *fname, unsigned long fname_len);
extern int C2F(intoschur)(char *fname, unsigned long fname_len);
extern int C2F(intzschur)(char *fname, unsigned long fname_len);
extern int C2F(intdgees1)(char *fname, unsigned long fname_len);
extern int C2F(intzgees1)(char *fname, unsigned long fname_len);
extern int C2F(intfschur)(char *fname, unsigned long fname_len);
extern int C2F(intzfschur)(char *fname, unsigned long fname_len);

extern int C2F(intdgges)(char *fname, unsigned long fname_len);
extern int C2F(intzgges)(char *fname, unsigned long fname_len);
extern int C2F(intogschur)(char *fname, unsigned long fname_len);
extern int C2F(intozgschur)(char *fname, unsigned long fname_len);
extern int C2F(intzgschur)(char *fname, unsigned long fname_len);
extern int C2F(intgschur)(char *fname, unsigned long fname_len);

extern int schtst(int longueur, int *header);

/*--------------------------------------------------------------------------*/
extern int C2F(complexify)(int *num);
/*--------------------------------------------------------------------------*/
int C2F(intschur)(char *fname,unsigned long fname_len)
{
	int *header1;int *header2;int *header3;
	int Cmplx, CmplxA, CmplxB;
	int ret, something, X;
	int which = 1; int longueur;

	if (GetType(1)!=sci_matrix) 
	{
		OverLoad(1);
		return 0;
	}

	header1 = (int *) GetData(1);   CmplxA=header1[3];
	switch (Rhs) 
	{
		case 1:   /* schur(A)   */
			switch (CmplxA) 
			{
				case REAL :
					ret = C2F(intdgees0)("schur",5L);
				break;

				case COMPLEX :
					ret = C2F(intzgees0)("schur",5L);
				break;

				default :
					Scierror(999,_("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
					fname,1);
				break;
			}
		break;

		case 2 :   /* schur(A, something)   */
			header2 = (int *) GetData(2); something=header2[0];
			switch (something) 
			{
				case FUNCTION :
					switch (CmplxA) 
					{
						case REAL :
							ret = C2F(intoschur)("schur",5L);
						break;
						case COMPLEX :
							ret = C2F(intzschur)("schur",5L);
						break;
						default :
							Scierror(999,_("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
							fname,2);
						break;
					}
				break;

				case STRING  :
					longueur=header2[5]-header2[4];
					which = schtst(longueur, header2);
					switch (which) 
					{
						case STRINGREAL:
							switch (CmplxA) 
							{
								case REAL:
									ret = C2F(intdgees1)("schur",5L);
								break;

								case COMPLEX:
									Scierror(999,_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),
									fname,2);
								break;
							}
						return 0;

						case STRINGCOMPLEX:
							switch (CmplxA) 
							{
								case REAL:
									ret = C2F(intdgees1)("schur",5L);
								break;

								case COMPLEX:
									ret = C2F(intzgees1)("schur",5L);
								break;
							}
						break;

						default:
						/*   String is an external function  */
							switch (CmplxA) 
							{
								case REAL:
									ret = C2F(intfschur)("schur",5L);
								break;
								case COMPLEX:
									ret = C2F(intzfschur)("schur",5L);
								break;
							}
					}
				break;

				case SCI_DOUBLE: /*schur(A,B)*/
					if (GetType(2)!=sci_matrix) 
					{
						OverLoad(2);
						return 0;
					}
					CmplxB=header2[3];
					if ((CmplxA == 0) && (CmplxB ==0))  Cmplx =0;
					else if ((CmplxA == 1) && (CmplxB ==0)) 
					{
						C2F(complexify)((X=2,&X));
						Cmplx=1;
					}
					else if ((CmplxA == 0) && (CmplxB ==1)) 
					{
						C2F(complexify)((X=1,&X));
						Cmplx=1;
					}
					else  Cmplx=1;

					switch (Cmplx) 
					{
						case REAL :
							ret = C2F(intdgges)("schur",6L);
						break;

						case COMPLEX :
							ret = C2F(intzgges)("schur",6L);
						break;

						default :
							Scierror(999,_("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
							fname,1);
						break;
					}
				break;
			} /* end of switch (something) */
			break;
		case 3: /* schur(A,B,something) */
			if (GetType(2)!=sci_matrix) 
			{
				OverLoad(2);
				return 0;
			}
			header2 = (int *) GetData(2); something=header2[0];
			CmplxB=header2[3];
			if ((CmplxA == 0) && (CmplxB ==0))  Cmplx =0;
			else if ((CmplxA == 1) && (CmplxB ==0)) 
			{
				C2F(complexify)((X=2,&X));
				Cmplx=1;
			}
			else if ((CmplxA == 0) && (CmplxB ==1)) 
			{
				C2F(complexify)((X=1,&X));
				Cmplx=1;
			}
			else  Cmplx=1;

			header3 = (int *) GetData(3); something=header3[0];
			switch (something) 
			{
				case FUNCTION :
					switch (Cmplx) 
					{
						case REAL :
							ret = C2F(intogschur)("schur",6L);
						break;

						case COMPLEX :
							ret = C2F(intozgschur)("schur",6L);
						break;

						default :
							Scierror(999,_("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
							fname,3);
						break;
					}
				break;
				case STRING  :
					switch (Cmplx) 
					{
						case REAL:
							ret = C2F(intgschur)("schur",6L);
						break;
						case COMPLEX:
							ret = C2F(intzgschur)("schur",6L);
						break;
						default :
							Scierror(999,_("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
							fname,3);
						break;
					}
				break;
				default: /* switch (something) */
				break;
			}
		break; /* end of case 3 */

		default :
			Scierror(999,_("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
			fname,1);
		break;
	} /*end of switch(Rhs) */
	return 0;
}
/*--------------------------------------------------------------------------*/
int schtst (int longueur,int *header)
{
	if( (longueur==1) && ( Abs(header[6])==27 ) ) return STRINGREAL;  /* "r" */

	if( (longueur==4) && ( Abs(header[6])==27 ) && (Abs(header[7])==14 ) && (Abs(header[8])==10 ) && ( Abs(header[9])==21 ) ) return STRINGREAL;  /* "real"  */
	if( (longueur==4) && ( Abs(header[6])==12 ) && (Abs(header[7])==24 ) && (Abs(header[8])==22 ) && ( Abs(header[9])==25 ) ) return STRINGCOMPLEX;  /* "comp" */
	if( (longueur==7) && ( Abs(header[6])==12 ) && (Abs(header[7])==24 ) && (Abs(header[8])==22 ) && ( Abs(header[9])==25 ) && ( Abs(header[10])==21) && ( Abs(header[11])==14) && ( Abs(header[12])==33) ) return STRINGCOMPLEX;  /* "complex" */
	return 0;
}
/*--------------------------------------------------------------------------*/
