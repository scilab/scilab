/*------------------------------------------------------------------------*/
/* file: sci_gsort.c                                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-baptiste Silvy, Allan CORNET ,Cong Wu (2007)            */
/*------------------------------------------------------------------------*/
#include "gw_elementaries_functions.h"
#include "stack-c.h"
#include "gsort.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "sortTemplate.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_gsort) _PARAMS((char *fname, unsigned long fname_len))
{
	char iord[2] ;
	char typex[10];
	SciIntMat Im;
	int Type = 0;
	char **S = NULL;
	int m1 = 0,n1 = 0,l1 =0 ;
	int m2 = 0,n2 = 0,l2 =0 ;
	int m3 = 0,n3 = 0,l3 =0 ;
	int ind_m1 = 0, ind_n1 = 0, ind_l1 =0;
	int *indices = NULL;
	int iflag = 0;

	iord[0] = 'd'; iord[1]='\0';
	typex[0] = 'g'; typex[1] = '\0';

	Rhs = Max(0, Rhs);
	CheckRhs(1,3);
	CheckLhs(1,2);

	if (Rhs >= 1)
	{
		Type=VarType(1);
		switch (Type) 
		{
		case sci_strings : 
			GetRhsVar(1,"S",&m1,&n1,&S);
			break;
		case sci_matrix :
			GetRhsVar(1,"d",&m1,&n1,&l1);
			break;
		case sci_ints:
			GetRhsVar(1,"I",&m1,&n1,&Im);
			break;
		default :
			Scierror(999,"%s: first argument has a wrong type, expecting scalar or string matrix\r\n",fname);
			return 0;
		}
	}

	if (Rhs >= 2)
	{
		char c;
		GetRhsVar(2,"c",&m2,&n2,&l2);
		if ( m2 == 0 ) 
		{
			Scierror(999,"%s: second argument is an empty string\r\n",fname);
			return 0;
		}
		c = *cstk(l2);
		if (c != 'r' && c != 'c' && c != 'g' && c != 'l') 
		{
			Scierror(999,"%s: second argument has a wrong value %s should be in r,c,g,lr,lc\r\n",fname,cstk(l2));
			return 0;
		}
		strcpy(typex,cstk(l2));
	}

	if (Rhs == 3)
	{
		GetRhsVar(3,"c",&m3,&n3,&l3);
		CheckLength(3,m3,1);
		if ( *cstk(l3) != 'i' && *cstk(l3) != 'd') 
		{
			Scierror(999,"%s: third argument must be \"i\" or \"d\"\r\n",fname);
			return 0;
		}
		iord[0] = *cstk(l3);
	}

	if ( typex[0] == 'l') 
	{
		if (typex[1] == 'r') 
		{
			ind_m1 = m1;
			ind_n1 = 1;
			ind_l1 = 0;
			if (ind_m1 != 0) indices = (int*)MALLOC(sizeof(int)*(ind_m1));   /* Only return in row*/
		}
		else 
		{
			ind_m1 = 1;
			ind_n1 = n1;
			ind_l1 = 0;
			if (ind_n1 != 0) indices = (int*)MALLOC(sizeof(int)*(ind_n1));  /*Only return in col */
           
		}
	}
	else 
	{
		ind_m1 = m1;
		ind_n1 = n1;
		ind_l1 = 0;
		if ( ind_m1*ind_n1 != 0 )indices = (int*)MALLOC(sizeof(int)*(ind_m1*ind_n1));  /* return a matrix*/
	}

	if (Lhs == 2) iflag = 1; 
	else iflag = 0;

	switch (Type)
	{
	case sci_matrix:
		{
			int i = 0;
			double *matrix = stk(l1);
			double *tmp_matrix = NULL;
			if ( m1*n1 != 0 ) tmp_matrix = (double*)MALLOC(sizeof(double)*(m1*n1));
			for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];
			if (typex[0]=='r' || typex[0]=='c') rowcolsortd(tmp_matrix,indices,m1,n1,typex,iord);/* When it is row sort or colume sort*/
			if (typex[0]=='g' ) wholesortd(tmp_matrix,indices,m1,n1,typex,iord); /* When it is 'g', to sort them all*/
			if (typex[0]=='l') lgsortd(tmp_matrix,indices,m1,n1,typex,iord);  /* When it is going to be lr or lc*/
			CreateVarFromPtr(Rhs+1,"d",&m1,&n1,&tmp_matrix);
			LhsVar(1)= Rhs+1 ;                    /*Output */
			if (Lhs == 2)
			{
				CreateVarFromPtr(Rhs+2,"i",&ind_m1,&ind_n1,&indices)
				LhsVar(2)= Rhs+2 ;
			}
			C2F(putlhsvar)();
			if (indices) {FREE(indices); indices = NULL;}
			if (tmp_matrix)	{ FREE(tmp_matrix); tmp_matrix = NULL;}
		}
		break;
	
	case sci_strings:
		{
			int i;
			char *tmp_matrix = NULL;
			tmp_matrix = (char*)MALLOC(sizeof(double)*(m1*n1));
            for (i = 0;i< m1*n1; i++) tmp_matrix[i] = S[i][0];
			//if (typex[0]=='l') lgsorts1(tmp_matrix,indices,m1,n1,typex,iord); /* When it is going to be lr or lc*/
            if (typex[0]=='g' ) wholesorts(S,indices,m1,n1,typex,iord); /* When it is 'g', to sort them all*/
			//if (typex[0]=='r' || typex[0]=='c') rowcolsorts(tmp_matrix,indices,m1,n1,typex,iord);/* When it is row sort or colume sort*/
			//for (i = 0;i< m1*n1; i++) S[i][0]=tmp_matrix[i] ;
			CreateVarFromPtr(Rhs+1,"S", &m1, &n1, S);    /*Output */
			LhsVar(1)=Rhs+1;
			if (Lhs == 2)
			{
				CreateVarFromPtr(Rhs+2,"i",&ind_m1,&ind_n1,&indices)
				LhsVar(2)= Rhs+2 ;
			}
			C2F(putlhsvar)();
			if (indices) {FREE(indices); indices = NULL;}
			if (tmp_matrix)	{ FREE(tmp_matrix); tmp_matrix = NULL;}
		}
		break;
	
	case sci_ints:               /* Can not find a example , so can not just remove it */
		{
			switch(Im.it)
			{
			case 1 :
				C2F(gsortchar)(Im.D,indices,&iflag,&m1,&n1,typex,iord);
				break;
			case 4 :
				C2F(gsortint)(Im.D,indices,&iflag,&m1,&n1,typex,iord);
				break;
			case 11 :
				C2F(gsortuchar)(Im.D,indices,&iflag,&m1,&n1,typex,iord);
				break;
			case 12 :
				C2F(gsortushort)(Im.D,indices,&iflag,&m1,&n1,typex,iord);
				break;
			case 14 :
				C2F(gsortuint)(Im.D,indices,&iflag,&m1,&n1,typex,iord);
				break;
			default:
				Scierror(999,"invalid type.\r\n");
				return 0;
			}

			LhsVar(1)= 1 ;

			if (Lhs == 2)
			{
				CreateVarFromPtr(Rhs+2,"i",&ind_m1,&ind_n1,&indices)
				LhsVar(2)= Rhs+2 ;
			}
			C2F(putlhsvar)();
			if (indices) {FREE(indices); indices = NULL;}
		}
		break;
	default:
		Scierror(999,"invalid type.\r\n");
		return 0;
		break;
	}

	return 0;
} 
/*-----------------------------------------------------------------------------------*/
