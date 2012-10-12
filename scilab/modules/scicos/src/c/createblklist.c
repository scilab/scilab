/*  Scicos
*
*  Copyright (C) DIGITEO - 2009 - Allan CORNET
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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
#include "createblklist.h"
#include "machine.h"
#include "stack-c.h"
#include "import.h"
#include "var2sci.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
extern void str2sci(char** x,int n,int m); /* core/src/c/str2sci.h */
extern void C2F(itosci)();
extern void C2F(dtosci)();
extern int C2F(scierr)();
extern void C2F(vvtosci)();
extern int C2F(mktlist)(); 
/*--------------------------------------------------------------------------*/
int createblklist(scicos_block *Blocks, int *ierr, int flag_imp, int funtyp)
{
	/*local variable declaration*/
	int j,k;
	int nu,mu,ny,my;
	int u_typ,y_typ;
	int oz_typ,opar_typ;

	/*variable used when imp_flag>=0*/
	int nv,mv;          /* length of data                                        */
	int nblk,ng;        /* to store number of blocks and number of zero cross.   */
	void *ptr;          /* ptr for data comming from import structure            */
	int *ptr_int;       /* ptr to store ptr on integer                           */
	double *ptr_double; /* ptr to store ptr on double                            */
	int *xptr=NULL;     /* to retrieve xptr by import and zcptr of scicos_blocks */
	int *zcptr=NULL;
	double *x=NULL;     /* ptr for x, xd and g for scicos_blocks              */
	double *xd=NULL;
	double *g=NULL;

	/* set length of block list -please update me-                           */
	static int nblklst=40;
	/* set string of first element of scilab Blocks tlist -please update me- */
	static char *str_blklst[]={ "scicos_block", "nevprt"  , "funpt" , "type"  ,
		"scsptr"      , "nz"      , "z"     , "noz"   ,
		"ozsz"        , "oztyp"   , "oz"    , "nx"    ,
		"x"           , "xd"      , "res"   , "nin"   ,
		"insz"        , "inptr"   , "nout"  , "outsz" ,
		"outptr"      , "nevout"  , "evout" , "nrpar" ,
		"rpar"        , "nipar"   , "ipar"  , "nopar" ,
		"oparsz"      , "opartyp" , "opar"  , "ng"    ,
		"g"           , "ztyp"    , "jroot" , "label" ,
		"work"        , "nmode"   , "mode"  , "xprop"};

	/* char ptr for str2sci - see below - */
	char **str1;

	/* set nblk, x, xd ptr coming from import strucuture,
	* if flag_imp >=0
	*/
	if (flag_imp>=0)
	{
		/*retrieve nblk by import structure*/
		strcpy(C2F(cha1).buf,"nblk");
		*ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
		if (*ierr==0) return 0;
		ptr_int = (int *) ptr;
		nblk = *ptr_int;

		/* retrieve ng by import structure */
		strcpy(C2F(cha1).buf,"ng");
		*ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
		ptr_int = (int *) ptr; /* cast void* ptr to int* ptr */
		ng = *ptr_int;

		/*retrieve xptr by import structure*/
		strcpy(C2F(cha1).buf,"xptr");
		*ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
		if (*ierr==0) return 0;
		ptr_int = (int *) ptr;
		xptr = ptr_int;

		/*retrieve zcptr by import structure*/
		strcpy(C2F(cha1).buf,"zcptr");
		*ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
		ptr_int = (int *) ptr;
		zcptr = ptr_int;

		/*retrieve x and xd by import structure*/
		strcpy(C2F(cha1).buf,"x");
		*ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
		if (*ierr==0) return 0;
		ptr_double = (double *) ptr;
		x = ptr_double;
		xd = &x[xptr[nblk]-1];

		/*retrieve g by import structure*/
		strcpy(C2F(cha1).buf,"g");
		*ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
		ptr_double = (double *) ptr;
		g = ptr_double;
	}

	/****************************
	* create scilab tlist Blocks
	****************************/
	/* 1 - scicos_block */
	str2sci(str_blklst,1,nblklst);

	/* 2 - nevprt */
	C2F(itosci)(&Blocks[0].nevprt,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 3 - funpt */
	if (sizeof(voidg) >= sizeof(double))
	{
		// store N double values as the function pointer value
		j = sizeof(voidg) / sizeof(double);
	}
	else
	{
		// push at least one double
		j = 1;
	}
	C2F(dtosci)(&Blocks[0].funpt, &j, (k = 1, &k));

	/* 4 - type */
	C2F(itosci)(&Blocks[0].type,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 5 - scsptr */
	C2F(itosci)(&Blocks[0].scsptr,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 6 - nz */
	C2F(itosci)(&Blocks[0].nz,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 7 - z */
	if(Blocks[0].scsptr>0)
	{
		C2F(vvtosci)(Blocks[0].z,&Blocks[0].nz);
		if (C2F(scierr)()!=0) return 0;
	}
	else
	{
		*ierr=var2sci(Blocks[0].z,Blocks[0].nz,1,SCSREAL_N);
		if (*ierr!=0) return 0; 
	}

	/* 8 - noz */
	C2F(itosci)(&Blocks[0].noz,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 9 - ozsz */
	C2F(itosci)(Blocks[0].ozsz,(j=Blocks[0].noz,&j),(k=2,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 10 - oztyp */
	C2F(itosci)(Blocks[0].oztyp,(j=Blocks[0].noz,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 11 - oz */
	for (k=0;k<Blocks[0].noz;k++)
	{
		nu=Blocks[0].ozsz[k]; /* retrieve number of rows */
		mu=Blocks[0].ozsz[Blocks[0].noz+k]; /* retrieve number of cols */
		oz_typ=Blocks[0].oztyp[k]; /* retrieve type */
		*ierr=var2sci(Blocks[0].ozptr[k],nu,mu,oz_typ);
		if (*ierr!=0) return 0;
	}
	/* if C blocks or null size then concatenate in a single list */
	if ((funtyp==4) || (Blocks[0].noz==0))
	{
		C2F(mklist)(&Blocks[0].noz); /*create oz list*/
	}

	/* 12 - nx */
	C2F(itosci)(&Blocks[0].nx,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 13 - x */
	if (flag_imp>=0) *ierr=var2sci(&x[xptr[flag_imp]-1],Blocks[0].nx,1,SCSREAL_N);
	else *ierr=var2sci(Blocks[0].x,Blocks[0].nx,1,SCSREAL_N);
	if (*ierr!=0) return 0; 

	/* 14 - xd */
	if (flag_imp>=0) *ierr=var2sci(&xd[xptr[flag_imp]-1],Blocks[0].nx,1,SCSREAL_N);
	else *ierr=var2sci(Blocks[0].xd,Blocks[0].nx,1,SCSREAL_N);
	if (*ierr!=0) return 0; 

	/* 15 - res */
	*ierr=var2sci(Blocks[0].res,Blocks[0].nx,1,SCSREAL_N);
	if (*ierr!=0) return 0;

	/* 16 - nin */
	C2F(itosci)(&Blocks[0].nin,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 17 - insz */
	C2F(itosci)(Blocks[0].insz,(j=3*Blocks[0].nin,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 18 - inptr */
	for (k=0;k<Blocks[0].nin;k++) 
	{
		nu=Blocks[0].insz[k]; /* retrieve number of rows */
		mu=Blocks[0].insz[Blocks[0].nin+k]; /* retrieve number of cols */
		u_typ=Blocks[0].insz[2*Blocks[0].nin+k]; /* retrieve type */
		*ierr=var2sci(Blocks[0].inptr[k],nu,mu,u_typ);
		if (*ierr!=0) return 0;
	}
	C2F(mklist)(&Blocks[0].nin); /*create inptr list*/

	/* 19 - nout */
	C2F(itosci)(&Blocks[0].nout,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 20 - outsz */
	C2F(itosci)(Blocks[0].outsz,(j=3*Blocks[0].nout,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 21 - outptr */
	for (k=0;k<Blocks[0].nout;k++) 
	{
		ny=Blocks[0].outsz[k]; /* retrieve number of rows */
		my=Blocks[0].outsz[Blocks[0].nout+k]; /* retrieve number of cols */
		y_typ=Blocks[0].outsz[2*Blocks[0].nout+k]; /* retrieve type */
		*ierr=var2sci(Blocks[0].outptr[k],ny,my,y_typ);
		if (*ierr!=0) return 0;
	}
	C2F(mklist)(&Blocks[0].nout); /*create outptr list*/

	/* 22 - nevout */
	C2F(itosci)(&Blocks[0].nevout,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 23 - evout */
	*ierr=var2sci(Blocks[0].evout,Blocks[0].nevout,1,SCSREAL_N);
	if (*ierr!=0) return 0;

	/* 24 - nrpar */
	C2F(itosci)(&Blocks[0].nrpar,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 25 - rpar */
	if(Blocks[0].scsptr>0)
	{
		C2F(vvtosci)(Blocks[0].rpar,&Blocks[0].nrpar);
		if (C2F(scierr)()!=0) return 0;
	}
	else
	{
		*ierr=var2sci(Blocks[0].rpar,Blocks[0].nrpar,1,SCSREAL_N);
		if (*ierr!=0) return 0;
	}

	/* 26 - nipar */
	C2F(itosci)(&Blocks[0].nipar,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 27 - ipar */
	C2F(itosci)(Blocks[0].ipar,(j=Blocks[0].nipar,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 28 - nopar */
	C2F(itosci)(&Blocks[0].nopar,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 29 - oparsz */
	C2F(itosci)(Blocks[0].oparsz,(j=Blocks[0].nopar,&j),(k=2,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 30 - opartyp */
	C2F(itosci)(Blocks[0].opartyp,(j=Blocks[0].nopar,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 31 - opar */
	for (k=0;k<Blocks[0].nopar;k++)
	{
		nu=Blocks[0].oparsz[k]; /* retrieve number of rows */
		mu=Blocks[0].oparsz[Blocks[0].nopar+k]; /* retrieve number of cols */
		opar_typ=Blocks[0].opartyp[k]; /* retrieve type */
		*ierr=var2sci(Blocks[0].oparptr[k],nu,mu,opar_typ);
		if (*ierr!=0) return 0;
	}
	/* if C blocks or null size then concatenate in a single list */
	if ((funtyp==4) || (Blocks[0].nopar==0))
	{
		C2F(mklist)(&Blocks[0].nopar); /*create opar list*/
	}

	/* 32 - ng */
	C2F(itosci)(&Blocks[0].ng,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 33 - g */
	if (flag_imp>=0) *ierr=var2sci(&g[zcptr[flag_imp]-1],Blocks[0].ng,1,SCSREAL_N);
	else *ierr=var2sci(Blocks[0].g,Blocks[0].ng,1,SCSREAL_N);
	if (*ierr!=0) return 0;

	/* 34 - ztyp */
	C2F(itosci)(&Blocks[0].ztyp,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 35 - jroot */
	C2F(itosci)(Blocks[0].jroot,(j=Blocks[0].ng,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 36 - label */
	if ((str1=MALLOC(sizeof(char*))) ==NULL )  return 0;
	if ((str1[0]=MALLOC(sizeof(char)*(strlen(Blocks[0].label)+1))) ==NULL )  return 0;
	(str1[0])[strlen(Blocks[0].label)]='\0';
	strncpy(str1[0],Blocks[0].label,strlen(Blocks[0].label));
	str2sci(str1,1,1);
	FREE(str1[0]);
	FREE(str1);
	if (C2F(scierr)()!=0) return 0; 

	/* 37 - work*/
	C2F(itosci)(&Blocks[0].work,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 38 - nmode*/
	C2F(itosci)(&Blocks[0].nmode,(j=1,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 39 - mode */
	C2F(itosci)(Blocks[0].mode,(j=Blocks[0].nmode,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	/* 40 - xprop */
	C2F(itosci)(Blocks[0].xprop,(j=Blocks[0].nx,&j),(k=1,&k));
	if (C2F(scierr)()!=0) return 0;

	C2F(mktlist)(&nblklst); /*create Blocks list*/
	if (C2F(scierr)()!=0) return 0;

	/*return 1 if succeeded */
	return 1;
}
/*--------------------------------------------------------------------------*/ 
