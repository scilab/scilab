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
#include "sciprint.h"
#include "localization.h"
#include "Scierror.h"
#include "cvstr.h"
#include "import.h"
#include "il_state.h"
#include "il_sim.h"
#include "createblklist.h"
#include "copyvarfromlistentry.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
extern void str2sci(char** x,int n,int m); /* core/src/c/str2sci.h */
extern int C2F(mktlist)(); 
/*--------------------------------------------------------------------------*/
/* intgetscicosvarsc getscicosvars interface routine
* retrieve some information during simulation.
*
* [myvar]=getscicosvars(str)
*
* rhs 1  : str : a character string matrix with choice,
*               - 'x' to retrieve continuous state
*               - 'xptr' to retrieve ptr of continuous state
*               - 'z' to retrieve discrete state
*               - 'zptr' to retrieve ptr of discrete state
*               - 'rpar' to retrieve real parameters
*               - 'rpptr' to retrieve ptr of real parameters
*               - 'ipar' to retrieve int parameters
*               - 'ipptr' to retrieve  ptr of int parameters
*               - 'outtb' to retrieve output register (list of scilb object)
*               - 'inpptr' to retrieve number of input ports
*               - 'outptr' to retrieve number of output ports
*               - 'inplnk' to retrieve link number of input ports
*               - 'outlnk' to retrieve link number of output ports
*               ...... -see below-
*
* lhs 1  : myvar : matrix of int32 or double, or list or a Tlist
*
* 31/05/06, Alan : Rewritten from original fortran
* source code intgetscicosvars in coselm.f.
*
* 22/06/06, Alan : Allow multiple string in rhs(1).
*                  Create Tlist for Lhs(1).
*
* 23/06/06, Alan : Create blocks list for Lhs(1).
*
* 13/11/06, Alan : Remove il_sim_save global variable (all in sim
*                  come from import struct now)
*                  evtspt & pointi of state come from import struct
*
* 09/02/07, Alan : Update with oz/opar and restore il_sim_save only for opar
*
*/
int sci_getscicosvars(char *fname,unsigned long fname_len)
{
	/************************************
	* variables and constants d?inition
	************************************/
	/* auxilary variables for dimension and address */
	int m1 = 0, n1 = 0;     /* dimension of input character string               */
	int *il_str = NULL;   /* address of the description of the input parameter */
	int *l_str = NULL;    /* address of the data of the input parameter        */
	int l_tmp = 0;     /* temp variables to store address                   */
	int *il_tmp = NULL;

	/* definition of min/max input/output argument */
	static int minrhs = 0, maxrhs = 1;

	/* auxilary variables */
	int nv = 0, mv = 0;                /* length of data                                      */
	void *ptr = 0;                /* ptr for data comming from import structure          */
	int *ptr_int = NULL;             /* ptr to store ptr on int                         */
	double *ptr_dd = NULL;           /* ptr to store ptr on double                          */
	scicos_block *ptr_scsblk = NULL; /* ptr to store ptr of scicos_block structure          */
	outtb_el *ptr_elem = NULL;       /* ptr to store ptr of outtb_el structure              */
	int nblk = 0, ng = 0;              /* to store number of blocks and number of zero cross. */
	int *xptr = NULL, *zcptr = NULL;        /* to store xptr and zcptr of scicos_blocks            */
	double *x = NULL,*xd = NULL,*g = NULL;         /* ptr for x, xd and g for scicos_blocks               */
	int *funtyp = NULL;              /* ptr for funtyp */

	int ierr = 0;       /* error flag                                */
	int errc = 0;       /* error flag for ceateblklst                */
	int ptr_pos = 0;    /* to store position in scilab string object */
	int Topsave = 0;    /* to save the Top position                  */

	int sz_str = 0;     /* local variabe to store size of string */
	int i = 0,j = 0,k = 0;      /* local counter variable                */

	/* number of entries -please update me-                        */
	static int nentries = 69;
	/* define accepted entries of getscicosvars -please update me- */
	static char *entry[] = 
	   {"x"       , "nx"       , "xptr"   , "zcptr"      , "z"        ,
		"nz"      , "zptr"     , "noz"    , "oz"         , "ozsz"     ,
		"oztyp"   , "ozptr"    , "rpar"   , "rpptr"      , "ipar"     ,
		"ipptr"   , "opar"     , "oparsz" , "opartyp"    , "opptr"    ,
		"outtb"   , "inpptr"   , "outptr" , "inplnk"     , "outlnk"   ,
		"subs"    , "tevts"    , "evtspt" , "pointi"     , "iord"     ,
		"oord"    , "zord"     , "funtyp" , "ztyp"       , "cord"     ,
		"ordclk"  , "clkptr"   , "ordptr" , "critev"     , "mod"      ,
		"nmod"    , "iz"       , "nblk"   , "izptr"      , "outtbptr" ,
		"outtbsz" , "outtbtyp" , "nlnk"   , "nsubs"      , "nevts"    ,
		"niord"   , "noord"    , "nzord"  , "funptr"     , "ncord"    ,
		"nordptr" , "iwa"      , "blocks" , "ng"         , "g"        ,
		"t0"      , "tf"       , "Atol"   , "rtol"       , "ttol"     ,
		"deltat"  , "hmax"     , "nelem"  , "outtb_elem"};

	char **dyn_char = NULL; /* for allocation of first entry in tlist */

	/****************************************
	* Check number of inputs and outputs Rhs
	****************************************/
	CheckRhs(minrhs, maxrhs);

	/* Display usage of getscicosvars function if Rhs==0 */
	if (Rhs == 0)
	{
		sciprint(_("\ngetscicosvars : utility function to retrieve\n"));
		sciprint(_("                scicos arrays during simulation.\n\n"));
		sciprint(_("Usage : [myvar]=getscicosvars([\"str1\";\"str2\";...]);\n\n"));
		sciprint(_("- myvar : an int32 or double matrix or a Tlist.\n"));
		sciprint(_("- [\"str1\";\"str2\",...] is a string matrix\n"));
		sciprint(_("  that must be informed with the following values :\n"));

		/* display allowed entries */
		i = 0;
		for (j = 0; j<nentries; j++)
		{
			if (j == nentries-1) sciprint("\"%s\" ",entry[j]);
			else sciprint("\"%s\", ",entry[j]);
			i++;
			if (i==6) 
			{
				sciprint("\n");
				i=0;
			}
		}
		sciprint("\n");
		return 0;
	}

	/*******************
	* Check str (rhs 1)
	*******************/
	il_str = (int *) GetData(1); /* get ptr of int header of rsh 1 */
	if(il_str[0] != sci_strings) /* Check if input argument is a character string matrix */
	{
		Scierror(55, _("%s : First argument must be a string.\n"),fname);
		C2F(iop).err = 1;
		return 0;
	}

	/*retrieve dimension of input string matrix*/
	m1 = il_str[1]; /* number of row    */
	n1 = il_str[2]; /* number of column */

	/*Create header of Tlist*/
	if ((dyn_char=MALLOC((1+m1*n1)*sizeof(char *)))==NULL)
	{
		Scierror(999, _("%s : No more memory.\n"),fname);
		return 0;
	}

	/* Type of list is scicosvar */
	if ((dyn_char[0]=MALLOC((strlen("scicosvar")+1)*sizeof(char)))==NULL)
	{
		FREE(dyn_char);
		Scierror(999, _("%s : No more memory.\n"),fname);
		return 0;
	}
	else strcpy(dyn_char[0],"scicosvar");

	/*Check string matrix  */
	for (j=0;j<m1*n1;j++)
	{
		sz_str = il_str[5+j]-il_str[4+j]; /* store the length of str */
		/*get current position in the istk*/
		if (j==0)
			ptr_pos=5+m1*n1; 
		else
			ptr_pos += il_str[5+j-1]-il_str[4+j-1];
		l_str = &il_str[ptr_pos]; /* get ptr of rsh 1 */
		/* codetoascii convertion */
		C2F(cha1).buf[0]=' ';
		C2F(cvstr)(&sz_str,&l_str[0],&C2F(cha1).buf[0],(i=1,&i),sz_str);
		C2F(cha1).buf[sz_str]='\0';
		/* search if string is in accordance with entry*/
		ierr=TRUE;
		for (i=0;i<nentries;i++)
		{
			if (strcmp(C2F(cha1).buf,entry[i]) == 0) 
			{
				/* Store string in header of Tlist */
				if ((dyn_char[j+1]=MALLOC(sizeof(char)*strlen(entry[i])+1))==NULL)
				{
					FREE(dyn_char);
					Scierror(999, _("%s : No more memory.\n"),fname);
					return 0;
				}
				else strcpy(dyn_char[j+1],entry[i]);

				ierr=FALSE;
				break;
			}
		}
		/* if failed then display an error message and exit*/
		if (ierr==TRUE)
		{
			FREE(dyn_char);
			Scierror(999, _("%s : Undefined field in string matrix position : %d.\n"),fname,j+1);
			return 0;
		}
	}

	/* store dyn_char on stack*/
	if (n1*m1>1) str2sci(dyn_char,1,n1*m1+1);

	/* return asked array */
	for (j=0;j<m1*n1;j++)
	{
		sz_str = il_str[5+j]-il_str[4+j]; /* store the length of str */
		/*get current position in the istk of the string*/
		if (j==0)
			ptr_pos=5+m1*n1; 
		else
			ptr_pos += il_str[5+j-1]-il_str[4+j-1];
		l_str = &il_str[ptr_pos]; /* get ptr of rsh 1 */
		/* codetoascii convertion */
		C2F(cha1).buf[0]=' ';
		C2F(cvstr)(&sz_str,&l_str[0],&C2F(cha1).buf[0],(i=1,&i),sz_str);
		C2F(cha1).buf[sz_str]='\0';

		/*****************************************************************
		* entries that can be retrieve by il_state_save global variable
		*****************************************************************/
		if (strcmp(C2F(cha1).buf,"x") == 0)           /* retrieve continuous state */
			ierr=CopyVarFromlistentry(j+2,get_il_state(),2);
		else if (strcmp(C2F(cha1).buf,"z") == 0)      /* retrieve discrete state */
			ierr=CopyVarFromlistentry(j+2,get_il_state(),3);
		else if (strcmp(C2F(cha1).buf,"oz") == 0)     /* retrieve object discrete state */
			ierr=CopyVarFromlistentry(j+2,get_il_state(),4);
		else if (strcmp(C2F(cha1).buf,"outtb") == 0)  /* retrieve outtb */
			ierr=CopyVarFromlistentry(j+2,get_il_state(),9);
		else if (strcmp(C2F(cha1).buf,"tevts") == 0)  /* retrieve tevts */
			ierr=CopyVarFromlistentry(j+2,get_il_state(),6);

		/***************************************************************
		* entries that can be retrieve by il_sim_save global variable
		***************************************************************/
		if (strcmp(C2F(cha1).buf,"opar") == 0)           /* retrieve object parameters */
			ierr=CopyVarFromlistentry(j+2,get_il_sim(),15);

		/*************************************************
		* int variables coming from import structure
		*************************************************/
		else if ((strcmp(C2F(cha1).buf,"mod") == 0)      || /* retrieve mode */
			(strcmp(C2F(cha1).buf,"nmod") == 0)     || /* retrieve nmode */
			(strcmp(C2F(cha1).buf,"iz") == 0)       || /* label int code of blocks */
			(strcmp(C2F(cha1).buf,"nblk") == 0)     || /* number of block */
			(strcmp(C2F(cha1).buf,"izptr") == 0)    || /* label int code of blocks ptr*/
			(strcmp(C2F(cha1).buf,"outtbptr") == 0) || /* outtb ptr */
			(strcmp(C2F(cha1).buf,"outtbsz") == 0)  || /* outtb size */
			(strcmp(C2F(cha1).buf,"outtbtyp") == 0) || /* outtb type */
			(strcmp(C2F(cha1).buf,"nlnk") == 0)     || /* number of link */
			(strcmp(C2F(cha1).buf,"nsubs") == 0)    || /* length of nsubs */
			(strcmp(C2F(cha1).buf,"nevts") == 0)    || /* length of evtspt & tevts */
			(strcmp(C2F(cha1).buf,"niord") == 0)    || /* length of iord */
			(strcmp(C2F(cha1).buf,"noord") == 0)    || /* length of oord */
			(strcmp(C2F(cha1).buf,"nzord") == 0)    || /* length of zord */
			(strcmp(C2F(cha1).buf,"funptr") == 0)   || /* retrieve function ptr */
			(strcmp(C2F(cha1).buf,"ncord") == 0)    || /* retrieve ncord */
			(strcmp(C2F(cha1).buf,"nordptr") == 0)  || /* retrieve nordptr */
			(strcmp(C2F(cha1).buf,"iwa") == 0)      || /* retrieve iwa */
			(strcmp(C2F(cha1).buf,"ng") == 0)       || /* retrieve ng */
			(strcmp(C2F(cha1).buf,"nx") == 0)       || /* retrieve nx */
			(strcmp(C2F(cha1).buf,"nz") == 0)       || /* retrieve nz */
			(strcmp(C2F(cha1).buf,"noz") == 0)      || /* retrieve noz */
			(strcmp(C2F(cha1).buf,"ozptr") == 0)    || /* retrieve ozptr */
			(strcmp(C2F(cha1).buf,"ozsz") == 0)     || /* retrieve ozsz */
			(strcmp(C2F(cha1).buf,"oztyp") == 0)    || /* retrieve oztyp */
			(strcmp(C2F(cha1).buf,"nelem") == 0)    || /* retrieve nelem */
			(strcmp(C2F(cha1).buf,"xptr") == 0)     || /* retrieve xptr */
			(strcmp(C2F(cha1).buf,"zcptr") == 0)    || /* retrieve zcptr */
			(strcmp(C2F(cha1).buf,"zptr") == 0)     || /* retrieve zptr */
			(strcmp(C2F(cha1).buf,"rpptr") == 0)    || /* retrieve rpptr */
			(strcmp(C2F(cha1).buf,"ipar") == 0)     || /* retrieve ipar */
			(strcmp(C2F(cha1).buf,"ipptr") == 0)    || /* retrieve ipptr */
			(strcmp(C2F(cha1).buf,"opptr") == 0)    || /* retrieve opptr */
			(strcmp(C2F(cha1).buf,"oparsz") == 0)   || /* retrieve oparsz */
			(strcmp(C2F(cha1).buf,"opartyp") == 0)  || /* retrieve opartyp */
			(strcmp(C2F(cha1).buf,"inpptr") == 0)   || /* retrieve inpptr */
			(strcmp(C2F(cha1).buf,"outptr") == 0)   || /* retrieve outptr */
			(strcmp(C2F(cha1).buf,"inplnk") == 0)   || /* retrieve inplnk */
			(strcmp(C2F(cha1).buf,"outlnk") == 0)   || /* retrieve outlnk */
			(strcmp(C2F(cha1).buf,"subs") == 0)     || /* retrieve subs */
			(strcmp(C2F(cha1).buf,"iord") == 0)     || /* retrieve iord */
			(strcmp(C2F(cha1).buf,"oord") == 0)     || /* retrieve iord */
			(strcmp(C2F(cha1).buf,"zord") == 0)     || /* retrieve iord */
			(strcmp(C2F(cha1).buf,"funtyp") == 0)   || /* retrieve funtyp */
			(strcmp(C2F(cha1).buf,"ztyp") == 0)     || /* retrieve ztyp */
			(strcmp(C2F(cha1).buf,"cord") == 0)     || /* retrieve cord */
			(strcmp(C2F(cha1).buf,"ordclk") == 0)   || /* retrieve ordclk */
			(strcmp(C2F(cha1).buf,"clkcpr") == 0)   || /* retrieve clkcpr */
			(strcmp(C2F(cha1).buf,"ordptr") == 0)   || /* retrieve ordptr */
			(strcmp(C2F(cha1).buf,"critev") == 0)   || /* retrieve critev */
			/* state */
			(strcmp(C2F(cha1).buf,"evtspt") == 0)   || /* retrieve evtspt */
			(strcmp(C2F(cha1).buf,"pointi") == 0)      /* retrieve nelem */
			)
		{
			/* retrieve dims and prt of asked array with getscicosvarsfromimport */
			ierr = getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

			/* check ierr flag */
			if (ierr==TRUE)
			{
				l_tmp = I_INT32; /* define type of int */
				CreateVar(j+2,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&nv,&mv,&l_tmp); /* Create int32 variable at the top+j+1 pos in the stack */
				il_tmp = (int *) istk(l_tmp);      /* Store value of address of istk(l_tmp) in il_tmp */
				ptr_int = (int *) ptr;             /* cast void* ptr to int* ptr */
				for (i=0;i<nv*mv;i++) il_tmp[i] = ptr_int[i]; /* copy returned array in istk */
			}
		}

		/*************************************************
		* double variables coming from import structure
		*************************************************/
		else if ((strcmp(C2F(cha1).buf,"rpar") == 0)   || /* retrieve rpar      */
			(strcmp(C2F(cha1).buf,"g") == 0)      || /* retrieve g      */
			(strcmp(C2F(cha1).buf,"t0") == 0)     || /* retrieve t0     */
			(strcmp(C2F(cha1).buf,"tf") == 0)     || /* retrieve tf     */
			(strcmp(C2F(cha1).buf,"Atol") == 0)   || /* retrieve Atol   */
			(strcmp(C2F(cha1).buf,"rtol") == 0)   || /* retrieve rtol   */
			(strcmp(C2F(cha1).buf,"ttol") == 0)   || /* retrieve ttol   */
			(strcmp(C2F(cha1).buf,"deltat") == 0) || /* retrieve deltat */
			(strcmp(C2F(cha1).buf,"hmax") == 0)      /* retrieve hmax   */
			)
		{
			/* retrieve dims and prt of asked array with getscicosvarsfromimport */
			ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

			/* check ierr flag */
			if (ierr==TRUE)
			{
				ptr_dd = (double *) ptr;
				CreateVar(j+2,MATRIX_OF_DOUBLE_DATATYPE,&nv,&mv,&l_tmp); /* Create double variable at the top+j+1 addr. of the stack */
				for (i=0;i<nv*mv;i++) *stk(l_tmp+i) = ptr_dd[i]; /* copy returned array in istk */
			}
		}

		/*************************************************
		* scicos_block ptr coming from import structure
		*************************************************/
		else if ((strcmp(C2F(cha1).buf,"blocks") == 0)
			)
		{
			/* retrieve scicos_block prt of asked array with getscicosvarsfromimport */
			ierr = getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

			/* check ierr flag */
			if (ierr==TRUE)
			{
				/* store ptr in ptrscs_blk */
				ptr_scsblk = (scicos_block *) ptr;

				/* retrieve nblk by import structure */
				strcpy(C2F(cha1).buf,"nblk");
				ierr = getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
				ptr_int = (int *) ptr; /* cast void* ptr to int* ptr */
				nblk = *ptr_int;

				/* retrieve ng by import structure */
				strcpy(C2F(cha1).buf,"ng");
				ierr = getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
				ptr_int = (int *) ptr; /* cast void* ptr to int* ptr */
				ng = *ptr_int;

				/*retrieve xptr by import structure*/
				strcpy(C2F(cha1).buf,"xptr");
				ierr = getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
				ptr_int = (int *) ptr;
				xptr = ptr_int;

				/*retrieve zcptr by import structure*/
				strcpy(C2F(cha1).buf,"zcptr");
				ierr = getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
				ptr_int = (int *) ptr;
				zcptr = ptr_int;

				/*retrieve x and xd by import structure*/
				strcpy(C2F(cha1).buf,"x");
				ierr = getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
				ptr_dd = (double *) ptr;
				x = ptr_dd;
				xd = &x[xptr[nblk]-1];

				/*retrieve g by import structure*/
				strcpy(C2F(cha1).buf,"g");
				ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
				ptr_dd = (double *) ptr;
				g = ptr_dd;

				/*retrieve funtyp by import structure*/
				strcpy(C2F(cha1).buf,"funtyp");
				ierr = getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);
				funtyp = (int *) ptr;

				/* store blklst on stack */
				Topsave = Top;     /* save Top counter */
				Top = Top - Rhs + 1 + j; /* adjust Top counter */

				for (k=0;k<nblk;k++)
				{/* for each block, call createblklist */

					/* set flag_imp <0 for createblklst */
					i=-1;

					/* the following test is done in order to know if block k
					* have been already called with callf in scicos.c
					*/
					if (ptr_scsblk[k].nx!=0)
					{
						if (ptr_scsblk[k].x!=&x[xptr[k]-1])
						{
							/*fprintf(stderr,"k=%d,X,xd Non initialise",k);*/
							/* set flag_imp=k for createblklst <0 */
							i=k;
						}
					}
					if (ptr_scsblk[k].ng!=0)
					{
						if ((ptr_scsblk[k].g!=&g[zcptr[k]-1]) && (ptr_scsblk[k].g!=&x[xptr[k]-1]))
						{
							/*fprintf(stderr,"k=%d,g Non initialise",k);*/
							/* set flag_imp=k for createblklst <0 */
							i=k;
						}
					}
					/* call createblklist */
					ierr=createblklist(&ptr_scsblk[k], &errc,i,k+1);

					/* if an error occurs in createblklist */
					if (ierr==FALSE)
					{
						Top=Topsave;
						break;
					}
				}
				/* if success, create a list of Typed list scicos_block */
				if (ierr==TRUE)
				{
					C2F(mklist)(&nblk);
					Top=Topsave; /* adjust Top counter */
					CreateVar(j+2,LIST_DATATYPE,&nblk,(i=1,&i),&l_tmp); /* this is done to inform common intersci */
				}

			}
		}

		/*******************************************
		* outtb_elem coming from import structure
		*******************************************/
		else if ((strcmp(C2F(cha1).buf,"outtb_elem") == 0)) /* retrieve outtb_elem */
		{
			/* retrieve dims and prt of asked array with getscicosvarsfromimport */
			ierr=getscicosvarsfromimport(C2F(cha1).buf,&ptr,&nv,&mv);

			/* check ierr flag */
			if (ierr==TRUE)
			{
				l_tmp = I_INT32; /* define type of int */
				CreateVar(j+2,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&nv,&mv,&l_tmp); /* Create int32 variable at the top+j+1 addr. of the stack */
				il_tmp = (int *) istk(l_tmp);      /* Store value of address of istk(l_tmp) in il_tmp */
				ptr_elem = (outtb_el *) ptr;       /* cast void* ptr to int* ptr */
				for (i=0;i<nv;i++) /* copy returned array in istk */
				{
					il_tmp[i] = ptr_elem[i].lnk + 1; /* +1 is for the connection with outtb list */
					il_tmp[nv+i] = ptr_elem[i].pos + 1;
				}
			}
		}

		/* if return a FALSE value in
		* error flag then display an error message.
		*/
		if(ierr!=TRUE)
		{
			Scierror(999, _("%s : Error.\n"),fname);
			FREE(dyn_char);
			return 0;
		}
	}
	/**********************
	* Create lsh argument
	**********************/
	/* create a tlist if number of string in rhs 1 is >1*/
	if (j>1) 
	{
		Top=Top+j;
		j++;
		C2F(mktlist)(&j);
		Top = Top-1;
		CreateVar(2,TYPED_LIST_DATATYPE,&j,(i=1,&i),&l_tmp); /* this is done to inform common intersci */
	}

	/* put new variable in lsh argument */
	LhsVar(1) = 2;
	C2F(putlhsvar)();

	/* end */
	FREE(dyn_char);
	return 0;
}
/*--------------------------------------------------------------------------*/ 
