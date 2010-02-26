/*  Scicos
*
*  Copyright (C) INRIA - Allan CORNET
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
#include "gw_scicos.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
/* intbuildouttb build an initialized outtb list
*
* [outtb]=buildouttb(lnksz,lnktyp)
*
* rhs 1 : lnksz, give the size of scilab object in outtb
*         can be all int type or double matrix
*         can have n,2 or 2,n size
*
* rhs 2 : lnktyp, gives the type of scilab objetc in outtb
*         1 : double
*         2 : complex
*         3 : int32
*         4 : int16
*         5 : int8
*         6 : uint32
*         7 : uint16
*         8 : uint8
*         else : double
*         can be all int type or double matrix
*         can have n,1 or 1,n size
*
* lhs 1 : a list of size n
*
* 02/07/06, Alan : Initial version.
* 21/11/06, Alan : Allow void rhs input param.
*
*/
/*--------------------------------------------------------------------------*/
int sci_buildouttb(char *fname,unsigned long fname_len)
{
	static int l1 = 0, m1 = 0,   n1 = 0;
	static int l2 = 0, m2 = 0,   n2 = 0;
	static int l3 = 0, n3 = 1;
	SciIntMat M1,M2,M3;

	int n_lnksz = 0, n_lnktyp = 0;
	int *lnksz = NULL,*lnktyp = NULL;

	double *ptr_d = NULL;
	double *ptr_dc = NULL;
	int *ptr_i = NULL;
	short *ptr_s = NULL;
	char *ptr_c = NULL;
	int *ptr_ui = NULL;
	short *ptr_us = NULL;
	char *ptr_uc = NULL;

	int nm = 0,i = 0,j = 0,ierr = 0;

	static int minlhs=1, maxlhs=1;
	static int minrhs=2, maxrhs=2;

	/*check number of lhs/rhs*/
	CheckLhs(minlhs, maxlhs);
	CheckRhs(minrhs, maxrhs);

	/*check type of Rhs 1*/
	if (VarType(1) == 1)
	{
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
	}
	else if (VarType(1) == sci_ints)
	{
		GetRhsVar(1, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m1, &n1, &M1);
	}
	else
	{
		Scierror(888, _("%s : lnksz argument must be double or integer.\n"),fname);
		return 0;
	}

	/*check type of Rhs 2*/
	if (VarType(2) == 1)
	{
		GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
	}
	else if (VarType(2) == sci_ints)
	{
		GetRhsVar(2, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m2, &n2, &M2);
	}
	else
	{
		Scierror(888,_("%s : lnktyp argument must be double or integer.\n"),fname);
		if (lnksz!=NULL) FREE(lnksz);
		return 0;
	}

	/*check size of Rhs 1*/
	if (m1 == 2) n_lnksz = n1;
	else if (n1==2) n_lnksz = m1;
	/*void double input give void list output*/
	else if ((n1==0)||(m1==0))
	{
		if ((n2==0)||(m2==0))
		{
			/*manually code a void list on rhs(1)*/
			ptr_i = (int*)GetData(1);
			ptr_i[0] = sci_list;
			ptr_i[1] = 0;
			ptr_i[2] = 1;
			LhsVar(1) = 1;
			C2F(putlhsvar)();
			return 0;
		}
		else
		{
			Scierror(888, _("%s : inconsistent dimension between lnksz and lnktyp.\n"), fname);
			return 0;
		}
	}
	else
	{
		Scierror(888, _("%s : bad dimension for lnksz argument.\n"), fname);
		return 0;
	}

	/*check size of Rhs 2*/
	if (m2 == 1) n_lnktyp = n2;
	else if (n2 == 1) n_lnktyp = m2;
	else if ((n2 == 0)||(m2 == 0))
	{
		if ( (n1 != 0) && (m1 != 0) )
		{
			Scierror(888, _("%s : inconsistent dimension between lnksz and lnktyp.\n"),fname);
			return 0;
		}
	}
	else
	{
		Scierror(888, _("%s : bad dimension for lnktyp argument.\n"),fname);
		return 0;
	}

	/*cross size checking*/
	if (n_lnksz!=n_lnktyp)
	{
		Scierror(888, _("%s : lnksz and lnktyp argument must have the same length.\n"),fname);
		return 0;
	}

	/*allocate lnksz*/
	if ((lnksz=MALLOC(2*n_lnksz*sizeof(int)))==NULL)
	{
		Scierror(999, _("%s : No more free memory.\n"),fname);
		return 0;
	}

	/* Allocate lnktyp*/
	if ((lnktyp=MALLOC(n_lnktyp*sizeof(int)))==NULL)
	{
		Scierror(999, _("%s : No more free memory.\n"),fname);
		if (lnksz!=NULL) FREE(lnksz);
		return 0;
	}

	/*store rhs 1 in lnksz */
	if ((m1 == n1)&&(m2 == m1)) m1 = -1; /* this is done for matrix 2,2 */
	if (VarType(1) == 8)
	{
		switch (M1.it)
		{
		case I_CHAR   : if (m1==2)
						{
							for(j=0;j<n_lnksz;j++)
							{
								lnksz[j]=(int) IC_CHAR(M1.D)[j*2];
								lnksz[n_lnksz+j]=(int) IC_CHAR(M1.D)[2*j+1];
							}
						}
						else
						{
							for(j=0;j<2*n_lnksz;j++)
								lnksz[j]=(int) IC_CHAR(M1.D)[j];
						}
						break;

		case I_INT16  : if (m1==2)
						{
							for(j=0;j<n_lnksz;j++)
							{
								lnksz[j]=(int) IC_INT16(M1.D)[j*2];
								lnksz[n_lnksz+j]=(int) IC_INT16(M1.D)[2*j+1];
							}
						}
						else
						{
							for(j=0;j<2*n_lnksz;j++)
								lnksz[j]=(int) IC_INT16(M1.D)[j];
						}
						break;

		case I_INT32  : if (m1==2)
						{
							for(j=0;j<n_lnksz;j++)
							{
								lnksz[j]=(int) IC_INT32(M1.D)[j*2];
								lnksz[n_lnksz+j]=(int) IC_INT32(M1.D)[2*j+1];
							}
						}
						else
						{
							for(j=0;j<2*n_lnksz;j++)
								lnksz[j]=(int) IC_INT32(M1.D)[j];
						}
						break;

		case I_UCHAR  : if (m1==2)
						{
							for(j=0;j<n_lnksz;j++)
							{
								lnksz[j]=(int) IC_UCHAR(M1.D)[j*2];
								lnksz[n_lnksz+j]=(int) IC_UCHAR(M1.D)[2*j+1];
							}
						}
						else
						{
							for(j=0;j<2*n_lnksz;j++)
								lnksz[j]=(int) IC_UCHAR(M1.D)[j];
						}
						break;

		case I_UINT16 : if (m1==2)
						{
							for(j=0;j<n_lnksz;j++)
							{
								lnksz[j]=(int) IC_UINT16(M1.D)[j*2];
								lnksz[n_lnksz+j]=(int) IC_UINT16(M1.D)[2*j+1];
							}
						}
						else
						{
							for(j=0;j<2*n_lnksz;j++)
								lnksz[j]=(int) IC_UINT16(M1.D)[j];
						}
						break;

		case I_UINT32 : if (m1==2)
						{
							for(j=0;j<n_lnksz;j++)
							{
								lnksz[j]=(int) IC_UINT32(M1.D)[j*2];
								lnksz[n_lnksz+j]=(int) IC_UINT32(M1.D)[2*j+1];
							}
						}
						else
						{
							for(j=0;j<2*n_lnksz;j++)
								lnksz[j]=(int) IC_UINT32(M1.D)[j];
						}
						break;
		}
	}
	else
	{
		if (m1==2)
		{
			for(j=0;j<n_lnksz;j++)
			{
				lnksz[j]=(int) ((double *) stk(l1))[j*2];
				lnksz[n_lnksz+j]=(int) ((double *) stk(l1))[2*j+1];
			}
		}
		else
		{
			for(j=0;j<2*n_lnksz;j++)
				lnksz[j]=(int) ((double *) stk(l1))[j];
		}
	}

	/*store rhs 2 in lnktyp */
	if (VarType(2) == sci_ints)
	{
		switch (M2.it)
		{
		case I_CHAR   : for(j=0;j<n_lnktyp;j++)
							lnktyp[j]=(int) IC_CHAR(M2.D)[j];
			break;

		case I_INT16  : for(j=0;j<n_lnktyp;j++)
							lnktyp[j]=(int) IC_INT16(M2.D)[j];
			break;

		case I_INT32  : for(j=0;j<n_lnktyp;j++)
							lnktyp[j]=(int) IC_INT32(M2.D)[j];
			break;

		case I_UCHAR  : for(j=0;j<n_lnktyp;j++)
							lnktyp[j]=(int) IC_UCHAR(M2.D)[j];
			break;

		case I_UINT16 : for(j=0;j<n_lnktyp;j++)
							lnktyp[j]=(int) IC_UINT16(M2.D)[j];
			break;

		case I_UINT32 : for(j=0;j<n_lnktyp;j++)
							lnktyp[j]=(int) IC_UINT32(M2.D)[j];
			break;
		}
	}
	else
	{
		for(j=0;j<n_lnktyp;j++)
			lnktyp[j]=(int) ((double *) stk(l2))[j];
	}

	/* build output list */
	CreateVar(3,LIST_DATATYPE,&n_lnktyp,&n3,&l3);

	for(i=0;i<n_lnktyp;i++)
	{
		nm=lnksz[i]*lnksz[i+n_lnktyp];
		switch (lnktyp[i])
		{
		case 1  : if ((ptr_d=MALLOC(nm*sizeof(double)))==NULL)
				  {
					  ierr=-1;
					  break;
				  }
				  for (j=0;j<nm;j++) ptr_d[j]=0;
				  CreateListVarFromPtr(3,i+1,MATRIX_OF_DOUBLE_DATATYPE,&lnksz[i],&lnksz[i+n_lnktyp], &ptr_d);
				  FREE(ptr_d);
				  break;

		case 2  : if ((ptr_d=MALLOC(2*nm*sizeof(double)))==NULL)
				  {
					  ierr=-1;
					  break;
				  }
				  for (j=0;j<2*nm;j++) ptr_d[j]=0;
				  ptr_dc = &ptr_d[nm];
				  CreateListCVarFromPtr(3,i+1,MATRIX_OF_DOUBLE_DATATYPE,(j=1,&j),&lnksz[i],&lnksz[i+n_lnktyp],&ptr_d,&ptr_dc);
				  FREE(ptr_d);
				  break;

		case 3  : if ((ptr_i=MALLOC(nm*sizeof(int)))==NULL)
				  {
					  ierr=-1;
					  break;
				  }
				  for (j=0;j<nm;j++) ptr_i[j]=0;
				  M3.m = lnksz[i];
				  M3.n = lnksz[i+n_lnktyp];
				  M3.it = 4;
				  M3.l = -1;
				  M3.D = ptr_i;
				  CreateListVarFromPtr(3,i+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&M3.m,&M3.n,&M3);
				  FREE(ptr_i);
				  break;

		case 4  : if ((ptr_s=MALLOC(nm*sizeof(short)))==NULL)
				  {
					  ierr=-1;
					  break;
				  }
				  for (j=0;j<nm;j++) ptr_s[j]=0;
				  M3.m = lnksz[i];
				  M3.n = lnksz[i+n_lnktyp];
				  M3.it = 2;
				  M3.l = -1;
				  M3.D = ptr_s;
				  CreateListVarFromPtr(3,i+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&M3.m,&M3.n,&M3);
				  FREE(ptr_s);
				  break;

		case 5  : if ((ptr_c=MALLOC(nm*sizeof(char)))==NULL)
				  {
					  ierr=-1;
					  break;
				  }
				  for (j=0;j<nm;j++) ptr_c[j]=0;
				  M3.m = lnksz[i];
				  M3.n = lnksz[i+n_lnktyp];
				  M3.it = 1;
				  M3.l = -1;
				  M3.D = ptr_c;
				  CreateListVarFromPtr(3,i+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&M3.m,&M3.n,&M3);
				  FREE(ptr_c);
				  break;

		case 6  : if ((ptr_ui=MALLOC(nm*sizeof(unsigned int)))==NULL)
				  {
					  ierr=-1;
					  break;
				  }
				  for (j=0;j<nm;j++) ptr_ui[j]=0;
				  M3.m = lnksz[i];
				  M3.n = lnksz[i+n_lnktyp];
				  M3.it = 14;
				  M3.l = -1;
				  M3.D = ptr_ui;
				  CreateListVarFromPtr(3,i+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&M3.m,&M3.n,&M3);
				  FREE(ptr_ui);
				  break;

		case 7  : if ((ptr_us=MALLOC(nm*sizeof(unsigned short)))==NULL)
				  {
					  ierr=-1;
					  break;
				  }
				  for (j=0;j<nm;j++) ptr_us[j]=0;
				  M3.m = lnksz[i];
				  M3.n = lnksz[i+n_lnktyp];
				  M3.it = 12;
				  M3.l = -1;
				  M3.D = ptr_us;
				  CreateListVarFromPtr(3,i+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&M3.m,&M3.n,&M3);
				  FREE(ptr_us);
				  break;

		case 8  : if ((ptr_uc=MALLOC(nm*sizeof(unsigned char)))==NULL)
				  {
					  ierr=-1;
					  break;
				  }
				  for (j=0;j<nm;j++) ptr_uc[j]=0;
				  M3.m = lnksz[i];
				  M3.n = lnksz[i+n_lnktyp];
				  M3.it = 11;
				  M3.l = -1;
				  M3.D = ptr_uc;
				  CreateListVarFromPtr(3,i+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&M3.m,&M3.n,&M3);
				  FREE(ptr_uc);
				  break;

		default : if ((ptr_d=MALLOC(nm*sizeof(double)))==NULL)
				  {
					  ierr=-1;
					  break;
				  }
				  for (j=0;j<nm;j++) ptr_d[j]=0;
				  CreateListVarFromPtr(3,i+1,MATRIX_OF_DOUBLE_DATATYPE,&lnksz[i],&lnksz[i+n_lnktyp], &ptr_d);
				  FREE(ptr_d);
				  break;
		}

		if (ierr==-1)
		{
			Scierror(999,_("%s : No more free memory.\n"),fname);
			FREE(lnksz);
			FREE(lnktyp);
			return 0;
		}
	}

	LhsVar(1) = 3;
	C2F(putlhsvar)();

	FREE(lnksz);
	FREE(lnktyp);
	return 0;
}
/*--------------------------------------------------------------------------*/
