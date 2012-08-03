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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "gw_scicos.h"
#include "stack-c.h"
#include "il_state.h"
#include "il_sim.h"
#include "Scierror.h"
#include "cvstr.h"
#include "scicos-def.h"
#include "scicos.h"
#include "import.h"
#include "localization.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
/* intsicosimc scicosim interface routine.
*
* [state,t] = scicosim(state,tcur,tf,sim,str,tol)
*
* rhs 1 state : Tlist
*        - 1  : state(1)     : !xcs  x  z  oz iz  tevts  evtspt  pointi  outtb  !
*        - 2  : state.x      : column vector of real
*        - 3  : state.z      : column vector of real
*        - 4  : state.oz     : list of scilab object
*        - 5  : state.iz     : column vector of real (empty object with flag "finish")
*        - 6  : state.tevts  : column vector of real
*        - 7  : state.evtspt : column vector of real
*        - 8  : state.pointi : real scalar
*        - 9  : state.outtb  : list of scilab object
* rhs 2 tcur  : real scalar
* rhs 3 tf    : real scalar
* rhs 4 sim   : Tlist
*        - 1  : sim(1) : !scs    funs    xptr    zptr    ozptr   zcptr   inpptr
*                         outptr inplnk  outlnk  rpar    rpptr   ipar    ipptr
*                         opar   opptr   clkptr  ordptr  execlk  ordclk  cord
*                         oord   zord    critev  nb      ztyp    nblk    ndcblk
*                         subscr funtyp  iord    labels  modptr  !
*        - 2  : sim.funs   : list of strings and/or scilab function
*        - 3  : sim.xptr   : column vector of real
*        - 4  : sim.zptr   : column vector of real
*        - 5  : sim.ozptr  : column vector of real
*        - 6  : sim.zcptr  : column vector of real
*        - 7  : sim.inpptr : column vector of real
*        - 8  : sim.outptr : column vector of real
*        - 9  : sim.inplnk : column vector of real
*        - 10 : sim.outlnk : column vector of real
*        - 11 : sim.rpar   : column vector of real
*        - 12 : sim.rpptr  : column vector of real
*        - 13 : sim.ipar   : column vector of real
*        - 14 : sim.ipptr  : column vector of real
*        - 15 : sim.opar   : list of scilab object
*        - 16 : sim.opptr  : column vector of real
*        - 17 : sim.clkptr : column vector of real
*        - 18 : sim.ordptr : column vector of real
*        - 19 : sim.execlk : matrix of real
*        - 20 : sim.ordclk : matrix of real
*        - 21 : sim.cord   : matrix of real
*        - 22 : sim.oord   : matrix of real
*        - 23 : sim.zord   : column vector ? of real
*        - 24 : sim.critev : column vector of real
*        - 25 : sim.nb     : real scalar
*        - 26 : sim.ztyp   : column vector of real
*        - 27 : sim.nblk   : real scalar
*        - 28 : sim.ndcblk : real scalar
*        - 29 : sim.subscr : column vector of real
*        - 30 : sim.funtyp : column vector of real
*        - 31 : sim.iord   : column vector of real
*        - 32 : sim.labels : column vector of strings
*        - 33 : sim.modptr : column vector of real
*
* rhs 5 str   : string flag : 'start','run','finish','linear'
* rhs 6 tol   : real vector of size (7,1) minimum (4,1)
*               [atol rtol ttol [deltat realtimescale solver hmax]]'
*
* 16/03/06, A.Layec : Rewritten from original fortran
*                     source code intsscicos in intcos.f.
*
* 29/03/06, Alan    : Improvement in accordance to c_pass2
*                     (int32 parameters).
*
* 31/05/06, Alan    : Add global variable int *il_state_save
*                     and int *il_sim_save in intcscicos.h to store
*                     stack address of list %cpr.state and %cpr.sim
*                     (to use with get/setscicosvars).
*
* 14/06/06, Alan    : Save common intersci before calling scicos
*                     (to disable scilab crash with scifunc.f).
*
* 13/11/06, Alan    : Get back to double parameters for sim and state
*                     (for better compatibility with scilab-4.x families).
*                     Remove il_sim_save global variable.
*
* 15/12/06, Alan    : Warnings compilation removed.
*                     This can crash scilab/scicos.
*                     Please report.
*
* xx/02/07, Alan    : Add opar/odstate : scilab lists of arbitrary object
*                     parameters/states.
*
*/
/*--------------------------------------------------------------------------*/
#define freeparam \
	FREE(l_sim_iord);\
	FREE(l_sim_funtyp);\
	FREE(l_sim_subscr);\
	FREE(l_sim_ndcblk);\
	FREE(l_sim_nblk);\
	FREE(l_sim_ztyp);\
	FREE(l_sim_nb);\
	FREE(l_sim_critev);\
	FREE(l_sim_zord);\
	FREE(l_sim_oord);\
	FREE(l_sim_cord);\
	FREE(l_sim_ordclk);\
	FREE(l_sim_execlk);\
	FREE(l_sim_ordptr);\
	FREE(l_sim_clkptr);\
	FREE(l_sim_rpptr);\
	FREE(l_sim_ipar);\
	FREE(l_sim_ipptr);\
	FREE(l_sim_opptr);\
	FREE(l_sim_outlnk);\
	FREE(l_sim_inplnk);\
	FREE(l_sim_outptr);\
	FREE(l_sim_inpptr);\
	FREE(l_sim_zcptr);\
	FREE(l_sim_ozptr);\
	FREE(l_sim_zptr);\
	FREE(l_sim_xptr);\
	FREE(l_sim_modptr);\
	FREE(l_state_evtspt);\
	FREE(l_pointi)
/*--------------------------------------------------------------------------*/
COSIM_struct C2F(cosim); /* declaration of cosim -valable partout- */
/*--------------------------------------------------------------------------*/
extern COSERR_struct coserr;    /* declaration of coserr -defined in scicos- */
extern int *listentry(int *header, int i); /* mexlib.h */
extern int C2F(funnum)(char *fname);
extern int C2F(namstr)();
/*--------------------------------------------------------------------------*/
/* variable defined in scicos.c */
extern CURBLK_struct C2F(curblk);
/*--------------------------------------------------------------------------*/
int sci_scicosim(char *fname,unsigned long fname_len)
{
	/************************************
	* variables and constants d?inition
	************************************/
	static int id[nsiz];

	/*declaration of static structure*/
	static struct {int idb;} C2F(dbcos);   /*declaration of dbcos*/

	typedef struct inter_s_
	{
		int iwhere,nbrows,nbcols,itflag,ntypes,lad,ladc,lhsvar;
	} intersci_state ;

	typedef struct inter_l
	{
		intersci_state *state ;
		int nbvars;
	} intersci_list ;

	intersci_list *loc = NULL;
	intersci_state *new = NULL ;


	/* declaration of outtb_elem */
	outtb_el *outtb_elem = NULL;
	static int nelem  = 0;

	/*auxilary variables for dimension and address*/
	static int m1 = 0;                  /*state*/
	static int *il_state = NULL;
	static int m1e2 = 0,n1e2 = 0;           /*state.x*/
	static int *il_state_x = NULL;
	static double *l_state_x = NULL;
	static int m1e3 = 0,n1e3 = 0;           /*state.z*/
	static int *il_state_z = NULL;
	static double *l_state_z = NULL;
	static int *il_state_oz = NULL;        /*state.oz*/
	static int noz = 0;
	static void **oz = NULL;
	static int *ozsz = NULL;
	static int *oztyp = NULL;
	static int m1e5 = 0,n1e5 = 0;           /*state.iz*/
	static int *il_state_iz = NULL;
	static void **l_state_iz = NULL;
	static int m1e6 = 0,n1e6 = 0;           /*state.tevts*/
	static int *il_state_tevts = NULL;
	static double *l_state_tevts = NULL;
	static int m1e7 = 0,n1e7 = 0;           /*state.evtspt*/
	static int *il_state_evtspt = NULL;
	static int *l_state_evtspt = NULL;
	static int m1e8 = 0,n1e8 = 0;           /*state.pointi*/
	static int *il_pointi = NULL;
	static int *l_pointi = NULL;
	static int *il_state_outtb = NULL;     /*state.outtb*/
	static int nlnk = 0;
	static void **outtbptr = NULL;
	static int *outtbsz = NULL;
	static int *outtbtyp = NULL;

	static int m2 = 0,n2 = 0;               /*tcur*/
	static int *il_tcur = NULL;
	static double *l_tcur = NULL;

	static int m3 = 0,n3 = 0;               /*tf*/
	static int *il_tf = NULL;
	static double *l_tf = NULL;

	static int m4 = 0,n4 = 0,l4 = 0,il4 = 0;        /*sim*/
	static int *il_sim = NULL;
	static int l4e2 = 0,il4e2 = 0;          /*sim.funs*/
	static int *il_sim_fun = NULL;
	static int nblk = 0;
	static int m_xptr = 0,n_xptr = 0;       /*sim.xptr*/
	static int *il_sim_xptr = NULL;
	static int *l_sim_xptr = NULL;
	static int m_zptr = 0,n_zptr = 0;       /*sim.zptr*/
	static int *il_sim_zptr = NULL;
	static int *l_sim_zptr = NULL;
	static int m_ozptr = 0,n_ozptr = 0;     /*sim.ozptr*/
	static int *il_sim_ozptr = NULL;
	static int *l_sim_ozptr = NULL;

	static int m_zcptr = 0,n_zcptr = 0;     /*sim.zcptr*/
	static int *il_sim_zcptr = NULL;
	static int *l_sim_zcptr = NULL;
	static int m_inpptr = 0,n_inpptr = 0;   /*sim.inpptr*/
	static int *il_sim_inpptr = NULL;
	static int *l_sim_inpptr = NULL;
	static int m_outptr = 0,n_outptr = 0;   /*sim.outptr*/
	static int *il_sim_outptr = NULL;
	static int *l_sim_outptr = NULL;
	static int m_inplnk = 0,n_inplnk = 0;   /*sim.inplnk*/
	static int *il_sim_inplnk = NULL;
	static int *l_sim_inplnk = NULL;
	static int m_outlnk = 0,n_outlnk = 0;   /*sim.outlnk*/
	static int *il_sim_outlnk = NULL;
	static int *l_sim_outlnk = NULL;
	static int m_rpar = 0,n_rpar = 0;       /*sim.rpar*/
	static int *il_sim_rpar = NULL;
	static double *l_sim_rpar = NULL;
	static int m_rpptr = 0,n_rpptr = 0;     /*sim.rpptr*/
	static int *il_sim_rpptr = NULL;
	static int *l_sim_rpptr = NULL;
	static int m_ipar = 0,n_ipar = 0;       /*sim.ipar*/
	static int *il_sim_ipar = NULL;
	static int *l_sim_ipar = NULL;
	static int m_ipptr = 0,n_ipptr = 0;     /*sim.ipptr*/
	static int *il_sim_ipptr = NULL;
	static int *l_sim_ipptr = NULL;
	static int *il_sim_opar = NULL;        /*sim.opar*/
	static int nopar = 0;
	static void **opar = NULL;
	static int *oparsz = NULL;
	static int *opartyp = NULL;
	static int m_opptr = 0,n_opptr = 0;     /*sim.opptr*/
	static int *il_sim_opptr = NULL;
	static int *l_sim_opptr = NULL;

	static int m_clkptr = 0,n_clkptr = 0;   /*sim.clkptr*/
	static int *il_sim_clkptr = NULL;
	static int *l_sim_clkptr = NULL;
	static int m_ordptr = 0,n_ordptr = 0;   /*sim.ordptr*/
	static int *il_sim_ordptr = NULL;
	static int *l_sim_ordptr = NULL;
	static int m_execlk = 0,n_execlk = 0;   /*sim.execlk*/
	static int *il_sim_execlk = NULL;
	static int *l_sim_execlk = NULL;
	static int m_ordclk = 0,n_ordclk = 0;   /*sim.ordclk*/
	static int *il_sim_ordclk = NULL;
	static int *l_sim_ordclk = NULL;
	static int m_cord = 0,n_cord = 0;       /*sim.cord*/
	static int *il_sim_cord = NULL;
	static int *l_sim_cord = NULL;
	static int m_oord = 0,n_oord = 0;       /*sim.oord*/
	static int *il_sim_oord = NULL;
	static int *l_sim_oord = NULL;
	static int m_zord = 0,n_zord = 0;       /*sim.zord*/
	static int *il_sim_zord = NULL;
	static int *l_sim_zord = NULL;
	static int m_critev = 0,n_critev = 0;   /*sim.critev*/
	static int *il_sim_critev = NULL;
	static int *l_sim_critev = NULL;
	static int m_nb = 0,n_nb = 0;           /*sim.nb*/
	static int *il_sim_nb = NULL;
	static int *l_sim_nb = NULL;
	static int m_ztyp = 0,n_ztyp = 0;       /*sim.ztyp*/
	static int *il_sim_ztyp = NULL;
	static int *l_sim_ztyp = NULL;
	static int m_nblk = 0,n_nblk = 0;       /*sim.nblk*/
	static int *il_sim_nblk = NULL;
	static int *l_sim_nblk = NULL;
	static int m_ndcblk = 0,n_ndcblk = 0;   /*sim.ndcblk*/
	static int *il_sim_ndcblk = NULL;
	static int *l_sim_ndcblk = NULL;
	static int m_subscr = 0,n_subscr = 0;   /*sim.subscr*/
	static int *il_sim_subscr = NULL;
	static int *l_sim_subscr = NULL;
	static int m_funtyp = 0,n_funtyp = 0;   /*sim.funtyp*/
	static int *il_sim_funtyp = NULL;
	static int *l_sim_funtyp = NULL;
	static int m_iord = 0,n_iord = 0;       /*sim.iord*/
	static int *il_sim_iord = NULL;
	static int *l_sim_iord = NULL;
	static int m_lab = 0,n_lab = 0;         /*sim.labels*/
	static int *il_sim_lab = NULL;
	static int *il_sim_labptr = NULL;
	static int *l_sim_lab = NULL;
	static int m_modptr = 0,n_modptr = 0;   /*sim.modptr*/
	static int *il_sim_modptr = NULL;
	static int *l_sim_modptr = NULL;

	static int m5 = 0,n5 = 0;               /*str*/
	static int *il_str = NULL;
	static int *l_str = NULL;
	static int flag;

	static int m6 = 0,n6 = 0;               /*tol*/
	static int *il_tol = NULL;
	static double *l_tol = NULL;

	/*auxilary variables*/
	static double simpar[7];
	static int solver = 0;
	static int *lfunpt = NULL;     /*for function table*/
	static int lf = 0,ilf = 0,ifun = 0; /*for function table*/
	static int ierr = 0,istate = 0; /*error variable of scicos.c*/

	/*local variable*/
	int *subheader = NULL; /*pointer to get address of a subvariable in a list*/
	int i = 0,j = 0,k = 0;        /*local counter variable*/
	int sz_str = 0;     /*local variable to store size of string*/
	int err_check = 0;  /*local variable for cross var. checking dimension*/

	/*definition of min/max input/output argument*/
	static int minlhs = 1, maxlhs = 2;
	static int minrhs = 6, maxrhs = 6;

	/*************************************
	* Check number of inputs and outputs
	*************************************/
	CheckRhs(minrhs,maxrhs);
	CheckLhs(minlhs,maxlhs);

	/****************
	* state (rhs 1)
	****************/
	il_state = (int *) GetData(1);

	/* make a copy of il_state in a global variabe */
	set_il_state(il_state);

	if(il_state[0]!=16) /*Check if state is a tlist*/
	{
		Scierror(56,"%s : First argument must be a Tlist.\n",fname);
		C2F(iop).err=1;
		return 0;
	}
	m1 = il_state[1];

	/*2 : state.x      */
	il_state_x = (int *) (listentry(il_state,2));
	l_state_x  = (double *) (il_state_x+4);
	m1e2 = il_state_x[1];
	n1e2 = il_state_x[2];

	/*3 : state.z      */
	il_state_z = (int *) (listentry(il_state,3));
	l_state_z  = (double *) (il_state_z+4);
	m1e3 = il_state_z[1];
	n1e3 = il_state_z[2];

	/*4 : state.oz     */
	il_state_oz = (int *) (listentry(il_state,4));
	if(il_state_oz[0]!=15) /*check if il_state_oz is a list*/
	{
		Scierror(56, _("%s : oz element of state must be a list.\n"),fname);
		C2F(iop).err=4;
		return 0;
	}
	noz = il_state_oz[1];

	/*5 : state.iz     */
	il_state_iz = (int *) (listentry(il_state,5));
	l_state_iz  = (void **) (il_state_iz+4);
	m1e5 = il_state_iz[1];
	n1e5 = il_state_iz[2];

	/*6 : state.tevts  */
	il_state_tevts = (int *) (listentry(il_state,6));
	l_state_tevts  = (double *) (il_state_tevts+4);
	m1e6 = il_state_tevts[1];
	n1e6 = il_state_tevts[2];

	/*7 : state.evtspt */
	il_state_evtspt = (int *) (listentry(il_state,7));
	m1e7 = il_state_evtspt[1];
	n1e7 = il_state_evtspt[2];
	if ((m1e7*n1e7)==0) l_state_evtspt=NULL;
	else
	{
		if ((l_state_evtspt=(int *) MALLOC((m1e7*n1e7)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			return 0;
		}
		else
		{
			for(i=0;i<(m1e7*n1e7);i++) l_state_evtspt[i]= (int) ((double *)(il_state_evtspt+4))[i];
		}
	}

	/*8 : state.pointi */
	il_pointi = (int *) (listentry(il_state,8));
	m1e8 = il_pointi[1];
	n1e8 = il_pointi[2];
	if ((l_pointi=(int *) MALLOC((m1e8*n1e8)*sizeof(int))) ==NULL )
	{
		Scierror(999,_("%s: Memory allocation error.\n"),fname);
		FREE(l_state_evtspt);
		return 0;
	}
	else
	{
		for(i=0;i<(m1e8*n1e8);i++) l_pointi[i]= (int) ((double *)(il_pointi+4))[i];
	}

	/*9 : state.outtb  */
	il_state_outtb = (int *) (listentry(il_state,9));
	if(il_state_outtb[0]!=15) /*check if il_state_outtb is a list*/
	{
		Scierror(56, _("%s : outtb element of state must be a list.\n"),fname);
		FREE(l_state_evtspt); FREE(l_pointi);
		C2F(iop).err=4;
		return 0;
	}
	nlnk = il_state_outtb[1]; /*nlnk is the dimension of the list state_louttb*/

	/***************
	* tcur (rhs 2)
	***************/
	il_tcur = (int *) GetData(2);
	if(il_tcur[0]!=1) /*Check if tcur is a real or complex matrix*/
	{
		Scierror(53, _("%s : Second argument must be a scalar.\n"),fname);
		FREE(l_state_evtspt); FREE(l_pointi);
		C2F(iop).err=2;
		return 0;
	}
	l_tcur = (double *) (il_tcur+4);
	m2 = il_tcur[1];
	n2 = il_tcur[2];
	CheckScalar(2,m2,n2);
	CheckDims(2,m2,n2,1,1);

	/*************
	* tf (rhs 3)
	*************/
	il_tf = (int *) GetData(3);
	if(il_tf[0]!=1) /*Check if tf is a real or complex matrix*/
	{
		Scierror(53, _("%s : Third argument must be a scalar.\n"),fname);
		FREE(l_state_evtspt); FREE(l_pointi);
		C2F(iop).err=3;
		return 0;
	}
	l_tf = (double *) (il_tf+4);
	m3 = il_tf[1];
	n3 = il_tf[2];
	CheckScalar(3,m3,n3);
	CheckDims(3,m3,n3,1,1);

	/*************
	* sim (rhs 4)
	*************/
	il_sim = (int *) GetData(4);


	set_il_sim(il_sim);

	if(il_sim[0]!=16)  /*Check if sim is a tlist*/
	{
		Scierror(56, _("%s : Fourth argument must be a Tlist.\n"),fname);
		FREE(l_state_evtspt); FREE(l_pointi);
		C2F(iop).err=4;
		return 0;
	}
	m4 = il_sim[1];
	n4 = il_sim[2];

	/*2  : sim.funs*/
	il_sim_fun = (int *) (listentry(il_sim,2));
	if(il_sim_fun[0]!=15) /*check if sim.funs is a list*/
	{
		Scierror(56, _("%s : Second element of sim must be a list.\n"),fname);
		FREE(l_state_evtspt); FREE(l_pointi);
		C2F(iop).err=4;
		return 0;
	}
	nblk = il_sim_fun[1]; /*nblk is the dimension of the list sim.funs*/

	/*3  : sim.xptr   */
	il_sim_xptr = (int *) (listentry(il_sim,3));
	m_xptr = il_sim_xptr[1];
	n_xptr = il_sim_xptr[2];
	if (m_xptr*n_xptr==0) l_sim_xptr=NULL;
	else
	{
		if ((l_sim_xptr=(int *) MALLOC((m_xptr*n_xptr)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_xptr*n_xptr);i++) l_sim_xptr[i]= (int) ((double *)(il_sim_xptr+4))[i];
		}
	}

	/*4  : sim.zptr   */
	il_sim_zptr = (int *) (listentry(il_sim,4));
	m_zptr = il_sim_zptr[1];
	n_zptr = il_sim_zptr[2];
	if (m_zptr*n_zptr==0) l_sim_zptr=NULL;
	else
	{
		if ((l_sim_zptr=(int *) MALLOC((m_zptr*n_zptr)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_zptr*n_zptr);i++) l_sim_zptr[i]= (int) ((double *)(il_sim_zptr+4))[i];
		}
	}

	/*5  : sim.ozptr   */
	il_sim_ozptr = (int *) (listentry(il_sim,5));
	m_ozptr = il_sim_ozptr[1];
	n_ozptr = il_sim_ozptr[2];
	if (m_ozptr*n_ozptr==0) l_sim_ozptr=NULL;
	else
	{
		if ((l_sim_ozptr=(int *) MALLOC((m_ozptr*n_ozptr)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_ozptr*n_ozptr);i++) l_sim_ozptr[i]= (int) ((double *)(il_sim_ozptr+4))[i];
		}
	}

	/*6  : sim.zcptr  */
	il_sim_zcptr = (int *) (listentry(il_sim,6));
	m_zcptr = il_sim_zcptr[1];
	n_zcptr = il_sim_zcptr[2];
	if (m_zcptr*n_zcptr==0) l_sim_zcptr=NULL;
	else
	{
		if ((l_sim_zcptr=(int *) MALLOC((m_zcptr*n_zcptr)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_ozptr); FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_zcptr*n_zcptr);i++) l_sim_zcptr[i]= (int) ((double *)(il_sim_zcptr+4))[i];
		}
	}

	/*7  : sim.inpptr */
	il_sim_inpptr = (int *) (listentry(il_sim,7));
	m_inpptr = il_sim_inpptr[1];
	n_inpptr = il_sim_inpptr[2];
	if (m_inpptr*n_inpptr==0) l_sim_inpptr=NULL;
	else
	{
		if ((l_sim_inpptr=(int *) MALLOC((m_inpptr*n_inpptr)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_ozptr);
			FREE(l_sim_zcptr); FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_inpptr*n_inpptr);i++) l_sim_inpptr[i]= (int) ((double *)(il_sim_inpptr+4))[i];
		}
	}

	/*8  : sim.outptr */
	il_sim_outptr = (int *) (listentry(il_sim,8));
	m_outptr = il_sim_outptr[1];
	n_outptr = il_sim_outptr[2];
	if (m_outptr*n_outptr==0) l_sim_outptr=NULL;
	else
	{
		if ((l_sim_outptr=(int *) MALLOC((m_outptr*n_outptr)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_inpptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zcptr); FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_outptr*n_outptr);i++) l_sim_outptr[i]= (int) ((double *)(il_sim_outptr+4))[i];
		}
	}

	/*9  : sim.inplnk */
	il_sim_inplnk = (int *) (listentry(il_sim,9));
	m_inplnk = il_sim_inplnk[1];
	n_inplnk = il_sim_inplnk[2];
	if (m_inplnk*n_inplnk==0) l_sim_inplnk=NULL;
	else
	{
		if ((l_sim_inplnk=(int *) MALLOC((m_inplnk*n_inplnk)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_inplnk*n_inplnk);i++) l_sim_inplnk[i]= (int) ((double *)(il_sim_inplnk+4))[i];
		}
	}

	/*10  : sim.outlnk */
	il_sim_outlnk = (int *) (listentry(il_sim,10));
	m_outlnk = il_sim_outlnk[1];
	n_outlnk = il_sim_outlnk[2];
	if (m_outlnk*n_outlnk==0) l_sim_outlnk=NULL;
	else
	{
		if ((l_sim_outlnk=(int *) MALLOC((m_outlnk*n_outlnk)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_outlnk*n_outlnk);i++) l_sim_outlnk[i]= (int) ((double *)(il_sim_outlnk+4))[i];
		}
	}

	/*11 : sim.rpar   */
	il_sim_rpar = (int *) (listentry(il_sim,11));
	m_rpar = il_sim_rpar[1];
	n_rpar = il_sim_rpar[2];
	if (m_rpar*n_rpar==0) l_sim_rpar=NULL;
	else
	{
		l_sim_rpar = (double *) (il_sim_rpar+4);
	}

	/*12 : sim.rpptr  */
	il_sim_rpptr = (int *) (listentry(il_sim,12));
	m_rpptr = il_sim_rpptr[1];
	n_rpptr = il_sim_rpptr[2];
	if (m_rpptr*n_rpptr==0) l_sim_rpptr=NULL;
	else
	{
		if ((l_sim_rpptr=(int *) MALLOC((m_rpptr*n_rpptr)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_outlnk); FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_rpptr*n_rpptr);i++) l_sim_rpptr[i]= (int) ((double *)(il_sim_rpptr+4))[i];
		}
	}

	/*13 : sim.ipar   */
	il_sim_ipar = (int *) (listentry(il_sim,13));
	m_ipar = il_sim_ipar[1];
	n_ipar = il_sim_ipar[2];
	if (m_ipar*n_ipar==0) l_sim_ipar=NULL;
	else
	{
		if ((l_sim_ipar=(int *) MALLOC((m_ipar*n_ipar)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_ipar*n_ipar);i++) l_sim_ipar[i]= (int) ((double *)(il_sim_ipar+4))[i];
		}
	}

	/*14 : sim.ipptr  */
	il_sim_ipptr = (int *) (listentry(il_sim,14));
	m_ipptr = il_sim_ipptr[1];
	n_ipptr = il_sim_ipptr[2];
	if (m_ipptr*n_ipptr==0) l_sim_ipptr=NULL;
	else
	{
		if ((l_sim_ipptr=(int *) MALLOC((m_ipptr*n_ipptr)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_ipar);
			FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_ipptr*n_ipptr);i++) l_sim_ipptr[i]= (int) ((double *)(il_sim_ipptr+4))[i];
		}
	}

	/*15 : sim.opar   */
	il_sim_opar = (int *) (listentry(il_sim,15));
	if(il_sim_opar[0]!=15)  /*Check if sim.opar is a list*/
	{
		Scierror(56, _("%s : sim.opar must be a list.\n"),fname);
		FREE(l_sim_ipptr); FREE(l_sim_ipar);
		FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
		FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
		FREE(l_sim_ozptr);
		FREE(l_sim_zptr); FREE(l_sim_xptr);
		FREE(l_state_evtspt); FREE(l_pointi);
		C2F(iop).err=4;
		return 0;
	}
	nopar = il_sim_opar[1];

	/*16 : sim.opptr  */
	il_sim_opptr = (int *) (listentry(il_sim,16));
	m_opptr = il_sim_opptr[1];
	n_opptr = il_sim_opptr[2];
	if (m_opptr*n_opptr==0) l_sim_opptr=NULL;
	else
	{
		if ((l_sim_opptr=(int *) MALLOC((m_opptr*n_opptr)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_ipptr); FREE(l_sim_ipar);
			FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_opptr*n_opptr);i++) l_sim_opptr[i]= (int) ((double *)(il_sim_opptr+4))[i];
		}
	}

	/*17 : sim.clkptr */
	il_sim_clkptr = (int *) (listentry(il_sim,17));
	m_clkptr = il_sim_clkptr[1];
	n_clkptr = il_sim_clkptr[2];
	if (m_clkptr*n_clkptr==0) l_sim_clkptr=NULL;
	else
	{
		if ((l_sim_clkptr=(int *) MALLOC((m_clkptr*n_clkptr)*sizeof(int))) ==NULL )
		{
			Scierror(999,  _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_clkptr);
			FREE(l_sim_opptr);
			FREE(l_sim_ipptr); FREE(l_sim_ipar);
			FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_clkptr*n_clkptr);i++) l_sim_clkptr[i]= (int) ((double *)(il_sim_clkptr+4))[i];
		}
	}

	/*18 : sim.ordptr */
	il_sim_ordptr = (int *) (listentry(il_sim,18));
	m_ordptr = il_sim_ordptr[1];
	n_ordptr = il_sim_ordptr[2];
	if ((l_sim_ordptr=(int *) MALLOC((m_ordptr*n_ordptr)*sizeof(int))) ==NULL )
	{
		Scierror(999, _("%s: Memory allocation error.\n"),fname);
		FREE(l_sim_clkptr);
		FREE(l_sim_opptr);
		FREE(l_sim_ipptr); FREE(l_sim_ipar);
		FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
		FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
		FREE(l_sim_ozptr);
		FREE(l_sim_zptr); FREE(l_sim_xptr);
		FREE(l_state_evtspt); FREE(l_pointi);
		return 0;
	}
	else
	{
		for(i=0;i<(m_ordptr*n_ordptr);i++) l_sim_ordptr[i]= (int) ((double *)(il_sim_ordptr+4))[i];
	}

	/*19 : sim.execlk */
	il_sim_execlk = (int *) (listentry(il_sim,19));
	m_execlk = il_sim_execlk[1];
	n_execlk = il_sim_execlk[2];
	if (m_execlk*n_execlk==0) l_sim_execlk=NULL;
	else
	{
		if ((l_sim_execlk=(int *) MALLOC((m_execlk*n_execlk)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_ordptr); FREE(l_sim_clkptr);
			FREE(l_sim_opptr);
			FREE(l_sim_ipptr); FREE(l_sim_ipar);
			FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_execlk*n_execlk);i++) l_sim_execlk[i]= (int) ((double *)(il_sim_execlk+4))[i];
		}
	}

	/*20 : sim.ordclk */
	il_sim_ordclk = (int *) (listentry(il_sim,20));
	m_ordclk = il_sim_ordclk[1];
	n_ordclk = il_sim_ordclk[2];
	if (m_ordclk*n_ordclk==0) l_sim_ordclk=NULL;
	else
	{
		if ((l_sim_ordclk=(int *) MALLOC((m_ordclk*n_ordclk)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
			FREE(l_sim_opptr);
			FREE(l_sim_ipptr); FREE(l_sim_ipar);
			FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_ordclk*n_ordclk);i++) l_sim_ordclk[i]= (int) ((double *)(il_sim_ordclk+4))[i];
		}
	}

	/*21 : sim.cord   */
	il_sim_cord = (int *) (listentry(il_sim,21));
	m_cord = il_sim_cord[1];
	n_cord = il_sim_cord[2];
	if (m_cord*n_cord==0) l_sim_cord=NULL;
	else
	{
		if ((l_sim_cord=(int *) MALLOC((m_cord*n_cord)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
			FREE(l_sim_opptr);
			FREE(l_sim_ipptr); FREE(l_sim_ipar);
			FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_cord*n_cord);i++) l_sim_cord[i]= (int) ((double *)(il_sim_cord+4))[i];
		}
	}

	/*22 : sim.oord   */
	il_sim_oord = (int *) (listentry(il_sim,22));
	m_oord = il_sim_oord[1];
	n_oord = il_sim_oord[2];
	if (m_oord*n_oord==0) l_sim_oord=NULL;
	else
	{
		if ((l_sim_oord=(int *) MALLOC((m_oord*n_oord)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_cord);
			FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
			FREE(l_sim_opptr);
			FREE(l_sim_ipptr); FREE(l_sim_ipar);
			FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_oord*n_oord);i++) l_sim_oord[i]= (int) ((double *)(il_sim_oord+4))[i];
		}
	}

	/*23 : sim.zord   */
	il_sim_zord = (int *) (listentry(il_sim,23));
	m_zord = il_sim_zord[1];
	n_zord = il_sim_zord[2];
	if (m_zord*n_zord==0) l_sim_zord=NULL;
	else
	{
		if ((l_sim_zord=(int *) MALLOC((m_zord*n_zord)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_oord); FREE(l_sim_cord);
			FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
			FREE(l_sim_opptr);
			FREE(l_sim_ipptr); FREE(l_sim_ipar);
			FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_zord*n_zord);i++) l_sim_zord[i]= (int) ((double *)(il_sim_zord+4))[i];
		}
	}

	/*24 : sim.critev */
	il_sim_critev = (int *) (listentry(il_sim,24));
	m_critev = il_sim_critev[1];
	n_critev = il_sim_critev[2];
	if (m_critev*n_critev==0) l_sim_critev=NULL;
	else
	{
		if ((l_sim_critev=(int *) MALLOC((m_critev*n_critev)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
			FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
			FREE(l_sim_opptr);
			FREE(l_sim_ipptr); FREE(l_sim_ipar);
			FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_critev*n_critev);i++) l_sim_critev[i]= (int) ((double *)(il_sim_critev+4))[i];
		}
	}

	/*25 : sim.nb     */
	il_sim_nb = (int *) (listentry(il_sim,25));
	m_nb = il_sim_nb[1];
	n_nb = il_sim_nb[2];
	if ((l_sim_nb=(int *) MALLOC((m_nb*n_nb)*sizeof(int))) ==NULL )
	{
		Scierror(999, _("%s: Memory allocation error.\n"),fname);
		FREE(l_sim_critev);
		FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
		FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
		FREE(l_sim_opptr);
		FREE(l_sim_ipptr); FREE(l_sim_ipar);
		FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
		FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
		FREE(l_sim_ozptr);
		FREE(l_sim_zptr); FREE(l_sim_xptr);
		FREE(l_state_evtspt); FREE(l_pointi);
		return 0;
	}
	else
	{
		for(i=0;i<(m_nb*n_nb);i++) l_sim_nb[i]= (int) ((double *)(il_sim_nb+4))[i];
	}
	if (l_sim_nb[0]!=nblk) /*value of nb must be equal to nblk*/
	{
		Scierror(42, _("%s : Incompatible sim.nb RHS parameter.\n"),fname);
		return 0;
	}

	/*26 : sim.ztyp   */
	il_sim_ztyp = (int *) (listentry(il_sim,26));
	m_ztyp = il_sim_ztyp[1];
	n_ztyp = il_sim_ztyp[2];
	if ((l_sim_ztyp=(int *) MALLOC((m_ztyp*n_ztyp)*sizeof(int))) ==NULL )
	{
		Scierror(999, _("%s: Memory allocation error.\n"),fname);
		FREE(l_sim_nb); FREE(l_sim_critev);
		FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
		FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
		FREE(l_sim_opptr);
		FREE(l_sim_ipptr); FREE(l_sim_ipar);
		FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
		FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
		FREE(l_sim_ozptr);
		FREE(l_sim_zptr); FREE(l_sim_xptr);
		FREE(l_state_evtspt); FREE(l_pointi);
		return 0;
	}
	else
	{
		for(i=0;i<(m_ztyp*n_ztyp);i++) l_sim_ztyp[i]= (int) ((double *)(il_sim_ztyp+4))[i];
	}

	/*27 : sim.nblk   */
	il_sim_nblk = (int *) (listentry(il_sim,27));
	m_nblk = il_sim_nblk[1];
	n_nblk = il_sim_nblk[2];
	if ((l_sim_nblk=(int *) MALLOC((m_nblk*n_nblk)*sizeof(int))) ==NULL )
	{
		Scierror(999, _("%s: Memory allocation error.\n"),fname);
		FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
		FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
		FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
		FREE(l_sim_opptr);
		FREE(l_sim_ipptr); FREE(l_sim_ipar);
		FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
		FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
		FREE(l_sim_ozptr);
		FREE(l_sim_zptr); FREE(l_sim_xptr);
		FREE(l_state_evtspt); FREE(l_pointi);
		return 0;
	}
	else
	{
		for(i=0;i<(m_nblk*n_nblk);i++) l_sim_nblk[i]= (int) ((double *)(il_sim_ztyp+4))[i];
	}

	/*28 : sim.ndcblk */
	il_sim_ndcblk = (int *) (listentry(il_sim,28));
	m_ndcblk = il_sim_ndcblk[1];
	n_ndcblk = il_sim_ndcblk[2];
	if ((l_sim_ndcblk=(int *) MALLOC((m_ndcblk*n_ndcblk)*sizeof(int))) ==NULL )
	{
		Scierror(999,_("%s: Memory allocation error.\n"),fname);
		FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
		FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
		FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
		FREE(l_sim_opptr);
		FREE(l_sim_ipptr); FREE(l_sim_ipar);
		FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
		FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
		FREE(l_sim_ozptr);
		FREE(l_sim_zptr); FREE(l_sim_xptr);
		FREE(l_state_evtspt); FREE(l_pointi);
		return 0;
	}
	else
	{
		for(i=0;i<(m_ndcblk*n_ndcblk);i++) l_sim_ndcblk[i]= (int) ((double *)(il_sim_ndcblk+4))[i];
	}

	/*29 : sim.subscr */
	il_sim_subscr = (int *) (listentry(il_sim,29));
	m_subscr = il_sim_subscr[1];
	n_subscr = il_sim_subscr[2];
	if (m_subscr*n_subscr==0) l_sim_subscr=NULL;
	else
	{
		if ((l_sim_subscr=(int *) MALLOC((m_subscr*n_subscr)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_ndcblk);
			FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
			FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
			FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
			FREE(l_sim_opptr);
			FREE(l_sim_ipptr); FREE(l_sim_ipar);
			FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_subscr*n_subscr);i++) l_sim_subscr[i]= (int) ((double *)(il_sim_subscr+4))[i];
		}
	}

	/*30 : sim.funtyp */
	il_sim_funtyp = (int *) (listentry(il_sim,30));
	m_funtyp = il_sim_funtyp[1];
	n_funtyp = il_sim_funtyp[2];
	if ((l_sim_funtyp=(int *) MALLOC((m_funtyp*n_funtyp)*sizeof(int))) ==NULL )
	{
		Scierror(999, _("%s: Memory allocation error.\n"),fname);
		FREE(l_sim_subscr); FREE(l_sim_ndcblk);
		FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
		FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
		FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
		FREE(l_sim_opptr);
		FREE(l_sim_ipptr); FREE(l_sim_ipar);
		FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
		FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
		FREE(l_sim_ozptr);
		FREE(l_sim_zptr); FREE(l_sim_xptr);
		FREE(l_state_evtspt); FREE(l_pointi);
		return 0;
	}
	else
	{
		for(i=0;i<(m_funtyp*n_funtyp);i++) l_sim_funtyp[i]= (int) ((double *)(il_sim_funtyp+4))[i];
	}

	/*31 : sim.iord   */
	il_sim_iord = (int *) (listentry(il_sim,31));
	m_iord = il_sim_iord[1];
	n_iord = il_sim_iord[2];
	if (m_iord*n_iord==0) l_sim_iord=NULL;
	else
	{
		if ((l_sim_iord=(int *) MALLOC((m_iord*n_iord)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(l_sim_funtyp); FREE(l_sim_subscr); FREE(l_sim_ndcblk);
			FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
			FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
			FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
			FREE(l_sim_opptr);
			FREE(l_sim_ipptr); FREE(l_sim_ipar);
			FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
			FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
			FREE(l_sim_ozptr);
			FREE(l_sim_zptr); FREE(l_sim_xptr);
			FREE(l_state_evtspt); FREE(l_pointi);
			return 0;
		}
		else
		{
			for(i=0;i<(m_iord*n_iord);i++) l_sim_iord[i]= (int) ((double *)(il_sim_iord+4))[i];
		}
	}

	/*32 : sim.labels */
	il_sim_lab = (int *) (listentry(il_sim,32));
	m_lab = il_sim_lab[1];
	n_lab = il_sim_lab[2];
	il_sim_labptr = &il_sim_lab[4];  /*get address-1 of first pointer in labels*/
	l_sim_lab = (int *) (il_sim_lab+m_lab+5); /*get address of first string in labels*/

	/*33 : sim.modptr */
	il_sim_modptr = (int *) (listentry(il_sim,33));
	m_modptr = il_sim_modptr[1];
	n_modptr = il_sim_modptr[2];
	if ((l_sim_modptr=(int *) MALLOC((m_modptr*n_modptr)*sizeof(int))) ==NULL )
	{
		Scierror(999, _("%s: Memory allocation error.\n"),fname);
		FREE(l_sim_iord); FREE(l_sim_funtyp); FREE(l_sim_subscr); FREE(l_sim_ndcblk);
		FREE(l_sim_nblk); FREE(l_sim_ztyp); FREE(l_sim_nb); FREE(l_sim_critev);
		FREE(l_sim_zord); FREE(l_sim_oord); FREE(l_sim_cord);
		FREE(l_sim_ordclk); FREE(l_sim_execlk); FREE(l_sim_ordptr); FREE(l_sim_clkptr);
		FREE(l_sim_opptr);
		FREE(l_sim_ipptr); FREE(l_sim_ipar);
		FREE(l_sim_rpptr); FREE(l_sim_outlnk); FREE(l_sim_inplnk);
		FREE(l_sim_outptr); FREE(l_sim_inpptr); FREE(l_sim_zcptr);
		FREE(l_sim_ozptr);
		FREE(l_sim_zptr); FREE(l_sim_xptr);
		FREE(l_state_evtspt); FREE(l_pointi);
		return 0;
	}
	else
	{
		for(i=0;i<(m_modptr*n_modptr);i++) l_sim_modptr[i]= (int) ((double *)(il_sim_modptr+4))[i];
	}

	/*************
	* str (rhs 5)
	*************/
	il_str = (int *) GetData(5);
	m5 = il_str[1];
	n5 = il_str[2];
	l_str = (int *) (il_str+6);
	CheckDims(5,m5,n5,m5,1);

	sz_str=il_str[5]-1; /*store the length of str*/
	C2F(cha1).buf[0]=' ';
	C2F(cvstr)(&sz_str,&l_str[0],&C2F(cha1).buf[0],(j=1,&j),sz_str); /*codetoascii*/
	C2F(cha1).buf[sz_str]='\0';
	if (strcmp(C2F(cha1).buf,"start") == 0) flag=1;
	else if (strcmp(C2F(cha1).buf,"run") == 0) flag=2;
	else if (strcmp(C2F(cha1).buf,"finish") == 0) flag=3;
	else if (strcmp(C2F(cha1).buf,"linear") == 0) flag=4;
	else if (strcmp(C2F(cha1).buf,"Kinsol") == 0) flag=5;
	else
	{
		Scierror(44,"%s : Fifth argument is incorrect.\n",fname);
		freeparam;
		C2F(iop).err=5;
		return 0;
	}

	/*************
	* tol (rhs 6)
	*************/
	il_tol = (int *) GetData(6);
	l_tol = (double *) (il_tol+4);
	m6 = il_tol[1];
	n6 = il_tol[2];
	m6 = m6*n6;
	if (m6<4) /*Check if tol has a minimun of four elements*/
	{
		Scierror(89, _("%s : Sixth argument must have at least four elements.\n"),fname);
		freeparam;
		C2F(iop).err=6;
		return 0;
	}
	else if(m6>7) /*Check if tol has a maximum of seven elements*/
	{
		Scierror(89,_("%s : Sixth argument must have a maximum of seven elements.\n"),fname);
		freeparam;
		C2F(iop).err=6;
		return 0;
	}

	/******************
	* set simpar array
	******************/
	if (m6==4) {for(i=0;i<4;i++) simpar[i]=l_tol[i];simpar[4]=0;simpar[5]=0;simpar[6]=0;}
	else if (m6==5) {for(i=0;i<5;i++) simpar[i]=l_tol[i];simpar[5]=0;simpar[6]=0;}
	else if (m6==6) {for(i=0;i<6;i++) simpar[i]=l_tol[i];simpar[6]=0;}
	else for(i=0;i<7;i++) simpar[i]=l_tol[i];
	solver=(int)simpar[5]; /*set solver variable*/

	/******************************
	* cross variable size checking
	******************************/
	err_check = 0;
	if (m1e6!=m1e7) err_check=1;             /*tevts vs evtspt*/
	else if (m_xptr!=m_zptr) err_check=2;    /*xptr vs zptr*/
	else if (m_xptr!=m_ozptr) err_check=3;   /*xptr vs ozptr*/
	else if (m_xptr!=m_zcptr) err_check=4;   /*xptr vs zcptr*/
	else if (m_xptr!=m_inpptr) err_check=5;  /*xptr vs inpptr*/
	else if (m_xptr!=m_outptr) err_check=6;  /*xptr vs outptr*/
	else if (m_xptr!=m_rpptr) err_check=7;   /*xptr vs rpptr*/
	else if (m_xptr!=m_ipptr) err_check=8;   /*xptr vs ipptr*/
	else if (m_xptr!=m_opptr) err_check=8;   /*xptr vs opptr*/
	else if (m_xptr!=m_clkptr) err_check=10; /*xptr vs clkptr*/
	else if ((n_ordclk!=2)&(m_ordclk!=0)) err_check=11; /*sim.ordclk*/
	else if ((n_cord!=2)&(m_cord!=0)) err_check=12;     /*sim.cord*/
	else if ((n_oord!=2)&(m_oord!=0)) err_check=13;     /*sim.oord*/
	else if ((n_zord!=2)&(m_zord!=0)) err_check=14;     /*sim.zord*/
	else if ((n_iord!=2)&(m_iord!=0)) err_check=15;     /*sim.iord*/
	if (err_check!=0)
	{
		/* please write an error table here  */
		Scierror(42,_("%s : error in cross variable size checking : %d\n"),\
			fname,err_check);
		freeparam;
		return 0;
	}

	/*******************************
	* set function table for blocks
	*******************************/
	il4 = iadr(*Lstk(Top-Rhs+4));  /*sim*/
	l4 = sadr(il4+m4+3);
	il4e2 = iadr(l4+*istk(il4+3)-1);
	l4e2 = sadr(il4e2+nblk+3);
	lf = l4e2;  /*first element of the list sim.funs*/
	/*define new variable lfunpt*/
	if ((lfunpt=(int *) MALLOC(nblk*sizeof(int))) ==NULL )
	{
		Scierror(999, _("%s: Memory allocation error.\n"),fname);
		freeparam;
		return 0;
	}

	/*for each block*/
	for (i=0;i<nblk;i++)
	{
		ilf=iadr(lf); /*should be removed later*/
		subheader=(int *)(listentry(il_sim_fun,i+1));
		/*Block is defined by a scilab function*/
		if ((subheader[0]==11)|(subheader[0]==13)) lfunpt[i]=-lf;
		/*Block is defined by a function described by a characater strings*/
		else if(subheader[0]==10)
		{
			sz_str=subheader[5]-1; /*store the length of function name*/
			C2F(cha1).buf[0]=' ';
			C2F(cvstr)(&sz_str,&subheader[6],&C2F(cha1).buf[0],(j=1,&j),sz_str); /*codetoascii*/
			C2F(cha1).buf[sz_str]='\0';
			ifun=C2F(funnum)(C2F(cha1).buf); /*search associated function number of function name*/
			/*Block is defined by a C or Fortran function*/
			if (ifun>0) lfunpt[i]=ifun;
			/*Block is defined by a predefined scilab function*/
			else 
			{
				C2F(namstr)(id,&subheader[6],&sz_str,(j=0,&j));
				C2F(com).fin=0;
				C2F(funs)(id);
				if ((C2F(com).fun==-1)|(C2F(com).fun==-2)) lfunpt[i]=-*Lstk(C2F(com).fin);
				else
				{
					C2F(curblk).kfun=i+1;
					Scierror(888, _("%s : unknown block : %s\n"),fname,C2F(cha1).buf);
					FREE(lfunpt);
					freeparam;
					return 0;
				}
			}
		}
		else
		{
			C2F(iop).err=4;
			Scierror(44,_("%s : error\n"),fname);
			FREE(lfunpt);
			freeparam;
			return 0;
		}
		lf=lf+*istk(il4e2+3+i)-*istk(il4e2+i+2);
	}

	/**********************
	* set oz, ozsz, oztyp
	**********************/
	if (noz==0)
	{
		oz=NULL;
		ozsz=NULL;
		oztyp=NULL;
	}
	else
	{
		/*Allocation of oz*/
		if ((oz=(void **) MALLOC((noz)*sizeof(void *))) ==NULL )
		{
			Scierror(999,_("%s: Memory allocation error.\n"),fname);
			FREE(lfunpt);
			freeparam;
			return 0;
		}
		/*Allocation of ozsz*/
		if ((ozsz=(int *) MALLOC(2*(noz)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(oz);
			FREE(lfunpt);
			freeparam;
			return 0;
		}
		/*Allocation of oztyp*/
		if ((oztyp=(int *) MALLOC((noz)*sizeof(int))) ==NULL )
		{
			Scierror(999, _("%s: Memory allocation error.\n"),fname);
			FREE(oz); FREE(ozsz);
			FREE(lfunpt);
			freeparam;
			return 0;
		}

		/*set vectors of oz*/
		for(j=0;j<noz;j++)
		{
			subheader=(int *)(listentry(il_state_oz,j+1));

			switch (subheader[0]) /*store type and address*/
			{
				/*matrix of double*/
			case 1  : switch (subheader[3])
					  {
			case 0  : oztyp[j]=SCSREAL_N;  /*double real matrix*/
				ozsz[j]=subheader[1];
				ozsz[j+noz]=subheader[2];
				oz[j]=(SCSREAL_COP *)(subheader+4);
				break;

			case 1  : oztyp[j]=SCSCOMPLEX_N;  /*double complex matrix*/
				ozsz[j]=subheader[1];
				ozsz[j+noz]=subheader[2];
				oz[j]=(SCSCOMPLEX_COP *)(subheader+4);
				break;

			default : oztyp[j]=SCSUNKNOW_N;
				ozsz[j]=il_state_oz[3+j]-il_state_oz[2+j];
				ozsz[j+noz]=1;
				oz[j]=(SCSUNKNOW_COP *)subheader;
				break;
					  }
					  break;

					  /*matrix of integers*/
			case 8  : switch (subheader[3])
					  {
			case 1  : oztyp[j]=SCSINT8_N;  /*int8*/
				ozsz[j]=subheader[1];
				ozsz[j+noz]=subheader[2];
				oz[j]=(SCSINT8_COP *)(subheader+4);
				break;

			case 2  : oztyp[j]=SCSINT16_N;  /*int16*/
				ozsz[j]=subheader[1];
				ozsz[j+noz]=subheader[2];
				oz[j]=(SCSINT16_COP *)(subheader+4);
				break;

			case 4  : oztyp[j]=SCSINT32_N;  /*int32*/
				ozsz[j]=subheader[1];
				ozsz[j+noz]=subheader[2];
				oz[j]=(SCSINT32_COP *)(subheader+4);
				break;

			case 11 : oztyp[j]=SCSUINT8_N; /*uint8*/
				ozsz[j]=subheader[1];
				ozsz[j+noz]=subheader[2];
				oz[j]=(SCSUINT8_COP *)(subheader+4);
				break;

			case 12 : oztyp[j]=SCSUINT16_N; /*uint16*/
				ozsz[j]=subheader[1];
				ozsz[j+noz]=subheader[2];
				oz[j]=(SCSUINT16_COP *)(subheader+4);
				break;

			case 14 : oztyp[j]=SCSUINT32_N; /*uint32*/
				ozsz[j]=subheader[1];
				ozsz[j+noz]=subheader[2];
				oz[j]=(SCSUINT32_COP *)(subheader+4);
				break;

			default : oztyp[j]=SCSUNKNOW_N;
				ozsz[j]=il_state_oz[3+j]-il_state_oz[2+j];
				ozsz[j+noz]=1;
				oz[j]=(SCSUNKNOW_COP *)subheader;
				break;
					  }
					  break;

			default : oztyp[j]=SCSUNKNOW_N;
				ozsz[j]=il_state_oz[3+j]-il_state_oz[2+j];
				ozsz[j+noz]=1;
				oz[j]=(SCSUNKNOW_COP *)subheader;
				break;
			}
		}
	}

	/****************************
	* set opar, oparsz, opartyp
	****************************/
	if (nopar==0)
	{
		opar=NULL;
		oparsz=NULL;
		opartyp=NULL;
	}
	else
	{
		/*Allocation of opar*/
		if ((opar=(void **) MALLOC((nopar)*sizeof(void *))) ==NULL )
		{
			Scierror(999,_("%s: Memory allocation error.\n"),fname);
			FREE(oz); FREE(ozsz); FREE(oztyp);
			FREE(lfunpt);
			freeparam;
			return 0;
		}
		/*Allocation of oparsz*/
		if ((oparsz=(int *) MALLOC(2*(nopar)*sizeof(int))) ==NULL )
		{
			Scierror(999,_("%s: Memory allocation error.\n"),fname);
			FREE(opar);
			FREE(oz); FREE(ozsz); FREE(oztyp);
			FREE(lfunpt);
			freeparam;
			return 0;
		}
		/*Allocation of opartyp*/
		if ((opartyp=(int *) MALLOC((nopar)*sizeof(int))) ==NULL )
		{
			Scierror(999,_("%s: Memory allocation error.\n"),fname);
			FREE(opar); FREE(oparsz);
			FREE(oz); FREE(ozsz); FREE(oztyp);
			FREE(lfunpt);
			freeparam;
			return 0;
		}

		/*set vectors of opar*/
		for(j=0;j<nopar;j++)
		{
			subheader=(int *)(listentry(il_sim_opar,j+1));

			switch (subheader[0]) /*store type and address*/
			{
				/*matrix of double*/
			case 1  : switch (subheader[3])
					  {
			case 0  : opartyp[j]=SCSREAL_N;  /*double real matrix*/
				oparsz[j]=subheader[1];
				oparsz[j+nopar]=subheader[2];
				opar[j]=(SCSREAL_COP *)(subheader+4);
				break;

			case 1  : opartyp[j]=SCSCOMPLEX_N;  /*double complex matrix*/
				oparsz[j]=subheader[1];
				oparsz[j+nopar]=subheader[2];
				opar[j]=(SCSCOMPLEX_COP *)(subheader+4);
				break;

			default : opartyp[j]=SCSUNKNOW_N;
				oparsz[j]=il_sim_opar[3+j]-il_sim_opar[2+j];
				oparsz[j+nopar]=1;
				opar[j]=(SCSUNKNOW_COP *)subheader;
				break;
					  }
					  break;

					  /*matrix of integers*/
			case 8  : switch (subheader[3])
					  {
			case 1  : opartyp[j]=SCSINT8_N;  /*int8*/
				oparsz[j]=subheader[1];
				oparsz[j+nopar]=subheader[2];
				opar[j]=(SCSINT8_COP *)(subheader+4);
				break;

			case 2  : opartyp[j]=SCSINT16_N;  /*int16*/
				oparsz[j]=subheader[1];
				oparsz[j+nopar]=subheader[2];
				opar[j]=(SCSINT16_COP *)(subheader+4);
				break;

			case 4  : opartyp[j]=SCSINT32_N;  /*int32*/
				oparsz[j]=subheader[1];
				oparsz[j+nopar]=subheader[2];
				opar[j]=(SCSINT32_COP *)(subheader+4);
				break;

			case 11 : opartyp[j]=SCSUINT8_N; /*uint8*/
				oparsz[j]=subheader[1];
				oparsz[j+nopar]=subheader[2];
				opar[j]=(SCSUINT8_COP *)(subheader+4);
				break;

			case 12 : opartyp[j]=SCSUINT16_N; /*uint16*/
				oparsz[j]=subheader[1];
				oparsz[j+nopar]=subheader[2];
				opar[j]=(SCSUINT16_COP *)(subheader+4);
				break;

			case 14 : opartyp[j]=SCSUINT32_N; /*uint32*/
				oparsz[j]=subheader[1];
				oparsz[j+nopar]=subheader[2];
				opar[j]=(SCSUINT32_COP *)(subheader+4);
				break;

			default : opartyp[j]=SCSUNKNOW_N;
				oparsz[j]=il_sim_opar[3+j]-il_sim_opar[2+j];
				oparsz[j+nopar]=1;
				opar[j]=(SCSUNKNOW_COP *)subheader;
				break;
					  }
					  break;

			default : opartyp[j]=SCSUNKNOW_N;
				oparsz[j]=il_sim_opar[3+j]-il_sim_opar[2+j];
				oparsz[j+nopar]=1;
				opar[j]=(SCSUNKNOW_COP *)subheader;
				break;
			}
		}
	}

	/*******************************
	* set outtbptr,outtbsz,outtbtyp
	*******************************/
	if (nlnk==0)
	{
		outtbptr=NULL;
		outtbsz=NULL;
		outtbtyp=NULL;
		outtb_elem=NULL;
		nelem=0;
	}
	else
	{
		/*Allocation of outtbptr*/
		if ((outtbptr=(void **) MALLOC(nlnk*sizeof(void *)))==NULL )
		{
			Scierror(999,_("%s: Memory allocation error.\n"),fname);
			FREE(opar); FREE(oparsz); FREE(opartyp);
			FREE(oz); FREE(ozsz); FREE(oztyp);
			FREE(lfunpt);
			freeparam;
			return 0;
		}
		/*Allocation of outtbsz*/
		if ((outtbsz=(int *) MALLOC(nlnk*2*sizeof(int)))==NULL )
		{
			Scierror(999,_("%s: Memory allocation error.\n"),fname);
			FREE(outtbptr);
			FREE(opar); FREE(oparsz); FREE(opartyp);
			FREE(oz); FREE(ozsz); FREE(oztyp);
			FREE(lfunpt);
			freeparam;
			return 0;
		}
		/*Allocation of outtbtyp*/
		if ((outtbtyp=(int *) MALLOC(nlnk*sizeof(int)))==NULL )
		{
			Scierror(999,_("%s: Memory allocation error.\n"),fname);
			FREE(outtbsz); FREE(outtbptr);
			FREE(opar); FREE(oparsz); FREE(opartyp);
			FREE(oz); FREE(ozsz); FREE(oztyp);
			FREE(lfunpt);
			freeparam;
			return 0;
		}

		/*initalize nelem*/
		nelem=0;

		/*set vectors of outtb*/
		for (j=0;j<nlnk;j++) /*for each link*/
		{
			subheader=(int *)(listentry(il_state_outtb,j+1)); /*get header of outtbl(j+1)*/
			outtbsz[j]=subheader[1]; /*store dimensions*/
			outtbsz[j+nlnk]=subheader[2];

			switch (subheader[0]) /*store type and address*/
			{
				/*matrix of double*/
			case 1  : switch (subheader[3])
					  {
			case 0  : outtbtyp[j]=SCSREAL_N;  /*double real matrix*/
				outtbptr[j]=(SCSREAL_COP *)(subheader+4);
				break;

			case 1  : outtbtyp[j]=SCSCOMPLEX_N;  /*double complex matrix*/
				outtbptr[j]=(SCSCOMPLEX_COP *)(subheader+4);
				break;

			default : Scierror(888,_("%s : error. Type %d of double scalar matrix not yet supported for outtb.\n"),\
						  fname,subheader[3]);
				FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
				FREE(opar); FREE(oparsz); FREE(opartyp);
				FREE(oz); FREE(ozsz); FREE(oztyp);
				FREE(lfunpt);
				freeparam;
				FREE(outtb_elem);
				break;
					  }
					  break;

					  /*matrix of integers*/
			case 8  : switch (subheader[3])
					  {
			case 1  : outtbtyp[j]=SCSINT8_N;  /*int8*/
				outtbptr[j]=(SCSINT8_COP *)(subheader+4);
				break;

			case 2  : outtbtyp[j]=SCSINT16_N;  /*int16*/
				outtbptr[j]=(SCSINT16_COP *)(subheader+4);
				break;

			case 4  : outtbtyp[j]=SCSINT32_N;  /*int32*/
				outtbptr[j]=(SCSINT32_COP *)(subheader+4);
				break;

			case 11 : outtbtyp[j]=SCSUINT8_N; /*uint8*/
				outtbptr[j]=(SCSUINT8_COP *)(subheader+4);
				break;

			case 12 : outtbtyp[j]=SCSUINT16_N; /*uint16*/
				outtbptr[j]=(SCSUINT16_COP *)(subheader+4);
				break;

			case 14 : outtbtyp[j]=SCSUINT32_N; /*uint32*/
				outtbptr[j]=(SCSUINT32_COP *)(subheader+4);
				break;

			default : Scierror(888,\
						  _("%s : error. Type %d of int scalar matrix not yet supported for outtb.\n"),\
						  fname,subheader[3]);
				FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
				FREE(opar); FREE(oparsz); FREE(opartyp);
				FREE(oz); FREE(ozsz); FREE(oztyp);
				FREE(lfunpt);
				freeparam;
				FREE(outtb_elem);
				break;
					  }
					  break;

			default : Scierror(888,_("%s : error. Type %d not yet supported for outtb.\n"),fname,subheader[0]);
				FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
				FREE(opar); FREE(oparsz); FREE(opartyp);
				FREE(oz); FREE(ozsz); FREE(oztyp);
				FREE(lfunpt);
				freeparam;
				FREE(outtb_elem);
				return 0;
				break;
			}

			/* store lnk and pos in outtb_elem */
			k=nelem;
			nelem+=outtbsz[j]*outtbsz[j+nlnk];
			if ((outtb_elem=(outtb_el *) REALLOC(outtb_elem,nelem*sizeof(outtb_el)))==NULL)
			{
				Scierror(999,_("%s : No more free memory.\n"),fname);
				FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
				FREE(opar); FREE(oparsz); FREE(opartyp);
				FREE(oz); FREE(ozsz); FREE(oztyp);
				FREE(lfunpt);
				freeparam;
				FREE(outtb_elem);
				return 0;
			}
			for (i=0;i<outtbsz[j]*outtbsz[j+nlnk];i++)
			{
				outtb_elem[k+i].lnk=j;
				outtb_elem[k+i].pos=i;
			}
		}
	}

	/********************************
	* save intersci common
	* see intersci_push in stack2.c
	********************************/
	Nbvars = Rhs; /*because of the use of getdata*/
	new = MALLOC(Rhs*sizeof(intersci_state));
	if (new == NULL)
	{
		Scierror(999,_("%s: Memory allocation error.\n"),fname);
		FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
		FREE(opar); FREE(oparsz); FREE(opartyp);
		FREE(oz); FREE(ozsz); FREE(oztyp);
		FREE(lfunpt);
		FREE(outtb_elem);
		freeparam;
		return 0;
	}
	loc = MALLOC(sizeof(intersci_list));
	if (loc == NULL)
	{
		Scierror(999,_("%s: Memory allocation error.\n"),fname);
		FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
		FREE(opar); FREE(oparsz); FREE(opartyp);
		FREE(oz); FREE(ozsz); FREE(oztyp);
		FREE(lfunpt);
		FREE(outtb_elem);
		FREE(new);
		freeparam;
		return 0;
	}
	loc->state = new; 
	loc->nbvars = Nbvars;
	for (i=0;i<Rhs;i++)
	{
		loc->state[i].iwhere = C2F(intersci).iwhere[i];
		loc->state[i].ntypes = C2F(intersci).ntypes[i];
		loc->state[i].lad    = C2F(intersci).lad[i];
		loc->state[i].lhsvar = C2F(intersci).lhsvar[i];
	}

	/************************
	* call scicos simulator
	************************/
	if (C2F(iop).ddt!=0) C2F(dbcos).idb=1;   /*debug mode if ddt=0*/
	C2F(cosim).isrun=1;  /*set isrun=1 to say that we enter in the simulator*/

	/* Calling sequence :
	* int C2F(scicos)(double *x_in, int *xptr_in, double *z__,
	*                 void **work,int *zptr,int *modptr_in,
	*                 void **oz,int *ozsz,int *oztyp,int *ozptr,
	*                 int *iz,int *izptr,double *t0_in,
	*                 double *tf_in,double *tevts_in,int *evtspt_in,
	*                 int *nevts,int *pointi_in,void **outtbptr_in,
	*                 int *outtbsz_in,int *outtbtyp_in,
	*                 outtb_el *outtb_elem_in,int *nelem1,int *nlnk1,
	*                 int *funptr,int *funtyp_in,int *inpptr_in,
	*                 int *outptr_in, int *inplnk_in,int *outlnk_in,
	*                 double *rpar,int *rpptr,int *ipar,int *ipptr,
	*                 void **opar,int *oparsz,int *opartyp,int *opptr,
	*                 int *clkptr_in,int *ordptr_in,int *nordptr1,
	*                 int *ordclk_in,int *cord_in,int *ncord1,
	*                 int *iord_in,int *niord1,int *oord_in,
	*                 int *noord1,int *zord_in,int *nzord1,
	*                 int *critev_in,int *nblk1,int *ztyp,
	*                 int *zcptr_in,int *subscr,int *nsubs,
	*                 double *simpar,int *flag__,int *ierr_out)
	*/

	C2F(scicos)(l_state_x,l_sim_xptr,l_state_z,
		l_state_iz,l_sim_zptr,l_sim_modptr,
		oz,ozsz,oztyp,l_sim_ozptr,
		l_sim_lab,il_sim_labptr,l_tcur,
		l_tf,l_state_tevts,l_state_evtspt,
		&m1e5,l_pointi,outtbptr,
		outtbsz,outtbtyp,
		outtb_elem,&nelem,&nlnk,
		lfunpt,l_sim_funtyp,l_sim_inpptr,
		l_sim_outptr,l_sim_inplnk,l_sim_outlnk,
		l_sim_rpar,l_sim_rpptr,l_sim_ipar,l_sim_ipptr,
		opar,oparsz,opartyp,l_sim_opptr,
		l_sim_clkptr,l_sim_ordptr,&m_ordptr,
		l_sim_ordclk,l_sim_cord,&m_cord,
		l_sim_iord,&m_iord,l_sim_oord,
		&m_oord,l_sim_zord, &m_zord,
		l_sim_critev,&nblk,l_sim_ztyp,
		l_sim_zcptr,l_sim_subscr,&m_subscr,
		simpar,&flag,&ierr);

	C2F(dbcos).idb=0;  /*return in normal mode*/
	C2F(cosim).isrun=0;  /*return in normal mode*/

	/******************************
	* retrieve intersci common
	* see intersci_pop in stack2.c
	******************************/
	Nbvars = loc->nbvars;
	for (i=0;i<Rhs;i++)
	{
		C2F(intersci).iwhere[i] = loc->state[i].iwhere ;
		C2F(intersci).ntypes[i] = loc->state[i].ntypes ;
		C2F(intersci).lad[i]    = loc->state[i].lad    ;
		C2F(intersci).lhsvar[i] = loc->state[i].lhsvar ;
	}
	FREE(loc->state);
	FREE(loc);

	/**********************
	* Free allocated array
	**********************/
	FREE(outtbptr); FREE(outtbtyp); FREE(outtbsz);
	FREE(opar); FREE(oparsz); FREE(opartyp);
	FREE(oz); FREE(ozsz); FREE(oztyp);
	FREE(lfunpt);
	FREE(outtb_elem);

	/*************************************
	* switch to appropriate message error
	*************************************/
	if (ierr>0)
	{
		switch (ierr)
		{
		case 1  : strcpy(C2F(cha1).buf,_("scheduling problem"));
			C2F(curblk).kfun=0;
			break;

		case 2  : strcpy(C2F(cha1).buf,_("input to zero-crossing stuck on zero"));
			C2F(curblk).kfun=0;
			break;

		case 3  : strcpy(C2F(cha1).buf,_("event conflict"));
			C2F(curblk).kfun=0;
			break;

		case 4  : strcpy(C2F(cha1).buf,_("algebraic loop detected"));
			C2F(curblk).kfun=0;
			break;

		case 5  : strcpy(C2F(cha1).buf,_("cannot allocate memory"));
			C2F(curblk).kfun=0;
			break;

		case 6  : sprintf(C2F(cha1).buf,_("the block %d has been called with input out of its domain"),C2F(curblk).kfun);
			break;

		case 7  : strcpy(C2F(cha1).buf,_("singularity in a block"));
			break;

		case 8  : strcpy(C2F(cha1).buf,_("block produces an internal error"));
			break;

		case 10  : strcpy(C2F(cha1).buf,coserr.buf);
			break;

		case 20  : strcpy(C2F(cha1).buf,_("initial conditions not converging"));
			break;

		case 21  : sprintf(C2F(cha1).buf,_("cannot allocate memory in block=%d"),C2F(curblk).kfun);
			break;

		case 22  : strcpy(C2F(cha1).buf,_("sliding mode condition, cannot integrate"));
			break;

		case 23  : strcpy(C2F(cha1).buf,_("Cannot find the initial mode, maybe there is a sliding mode condition"));
			break;
		case 24  : strcpy(C2F(cha1).buf,_("You have changed a parameter in your model, but the model has been compiled to use an XML file containing initial values and parameters. So you should either recompile your Scicos diagram or [re]launch the initialization interface to regenerate the XML file  with new parameters."));
			break;

		case 25  : strcpy(C2F(cha1).buf,_("Undefined data type."));
			break;
		case 26  : strcpy(C2F(cha1).buf,_("The number of parameter provided by Scicos blocks is different from what expected by the code generated by the Modelica compiler. You might have relaxed a parameter using FIXED property (i.e., fixed=false) in a Modelica model. This will be corrected in the next version"));
			break;
			/*In this case, you need to turn off the parameter embedded code generation mode by setting Modelica_ParEmb=%f in the Scilab command window, and  recompile the Scicos diagram*/
			/* IDA error messages*/
		case 201  : strcpy(C2F(cha1).buf,_("IDA_MEM_NULL:The argument ida_mem was NULL"));
			break;

		case 202  : strcpy(C2F(cha1).buf,_("IDA_ILL_INPUT: One of the input arguments was illegal. This errors message may be returned if the linear solver function initialization (called by the user after calling IDACreate) failed to set the linear solver-specific lsolve field in ida_mem."));
			break;

		case 203  : strcpy(C2F(cha1).buf,_("IDA_NO_MALLOC: indicating that ida_mem was not allocated."));
			break;

		case 204  : strcpy(C2F(cha1).buf,_("IDA_TOO_MUCH_WORK: The solver took mxstep internal steps but could not reach tout. The default value for mxstep is MXSTEP_DEFAULT = 500."));
			break;

		case 205  : strcpy(C2F(cha1).buf,_("IDA_TOO_MUCH_ACC: The solver could not satisfy the accuracy demanded by the user for some internal step."));
			break;

		case 206  : strcpy(C2F(cha1).buf,_("IDA_ERR_FAIL: Error test failures occurred too many times (=MXETF = 10) during one internal step or occurred with |h|=h_min"));
			break;

		case 207  : strcpy(C2F(cha1).buf,_("IDA_CONV_FAIL: IDACalcIC->Failed to get convergence of the Newton iterations. or IDA_solve->Convergence test failures occurred too many times (= MXNCF = 10) during one internal step. "));
			break;

		case 208  : strcpy(C2F(cha1).buf,_("IDA_LINIT_FAIL: The linear solver''s initialization routine failed."));
			break;

		case 209  : strcpy(C2F(cha1).buf,_("IDA_LSETUP_FAIL: The linear solver''s setup routine had a non-recoverable error."));
			break;

		case 210  : strcpy(C2F(cha1).buf,_("IDA_LSOLVE_FAIL: The linear solver''s solve routine had a non-recoverable error."));
			break;

		case 211  : strcpy(C2F(cha1).buf,_("IDA_RES_FAIL: The user''s residual routine returned a non-recoverable error flag."));
			break;

		case 212  : strcpy(C2F(cha1).buf,_("IDA_CONSTR_FAIL: IDACalcIC was unable to find a solution satisfying the inequality constraints."));
			break;

		case 213  : strcpy(C2F(cha1).buf,_("IDA_REP_RES_ERR: The user''s residual function repeatedly returned a recoverable error flag, but the solver was unable to recover."));
			break;

		case 214  : strcpy(C2F(cha1).buf,_("IDA_MEM_FAIL: A memory allocation failed."));
			break;

		case 215  : strcpy(C2F(cha1).buf,_("IDA_BAD_T: t is not in the interval [tn-hu,tn]."));
			break;

		case 216  : strcpy(C2F(cha1).buf,_("IDA_BAD_EWT: Some component of the error weight vector is zero (illegal), either for the input value of y0 or a corrected value."));
			break;

		case 217  : strcpy(C2F(cha1).buf,_("IDA_FIRST_RES_FAIL: The user''s residual routine returned  a recoverable error flag on the first call, but IDACalcIC was unable to recover."));
			break;

		case 218  : strcpy(C2F(cha1).buf,_("IDA_LINESEARCH_FAIL: The Linesearch algorithm failed to find a solution with a step larger than steptol in weighted RMS norm."));
			break;

		case 219  : strcpy(C2F(cha1).buf,_("IDA_NO_RECOVERY: The user''s residual routine, or the linear solver''s setup or solve routine had a  recoverable error, but IDACalcIC was unable to recover."));
			break;

		case 220  : strcpy(C2F(cha1).buf,_("IDA_RTFUNC_FAIL: The root founding function failed."));
			break;

		case 228  : strcpy(C2F(cha1).buf,_("IDA_YOUT_NULL: ''yout'' = NULL illegal."));
			break;
		case 229  : strcpy(C2F(cha1).buf,_("IDA_TRET_NULL: ''tret'' = NULL illegal."));
			break;
		case 230  : strcpy(C2F(cha1).buf,_("IDA_BAD_ITASK: Illegal value for itask."));
			break;
		case 231  : strcpy(C2F(cha1).buf,_("IDA_NO_ESTOP: itask = IDA_NORMAL_TSTOP or itask = IDA_ONE_STEP_TSTOP but tstop was not set"));
			break;
		case 232  : strcpy(C2F(cha1).buf,_("IDA_BAD_H0: h0 and tout - t0 inconsistent."));
			break;
		case 233  : strcpy(C2F(cha1).buf,_("IDA_BAD_TSTOP: tstop is behind current simulation time in the direction of integration."));
			break;
		case 234  : strcpy(C2F(cha1).buf,_("IDA_BAD_INIT_ROOT: Root found at and very near initial t."));
			break;
		case 235  : strcpy(C2F(cha1).buf,_("IDA_NO_EFUN: itol = IDA_WF but no EwtSet function was provided."));
			break;
		case 236  : strcpy(C2F(cha1).buf,_("IDA_EWT_FAIL: The user-provide EwtSet function failed."));
			break;
		case 237  : strcpy(C2F(cha1).buf,_("IDACalc cannot find the initial condition at this point. If you are using a Modelica model, you can try with the initialization GUI to try with other nonlinear solvers.")); 
			break;
		case 238  : strcpy(C2F(cha1).buf,_("IDA_LSOLVE_NULL: The linear solver''s solve routine is NULL."));
			break;
		case 239  : strcpy(C2F(cha1).buf,_("IDA_NULL_Y0: y0 = NULL illegal."));
			break;
		case 240  : strcpy(C2F(cha1).buf,_("IDA_BAD_ITOL: Illegal value for itol. The legal values are IDA_SS, IDA_SV, and IDA_WF."));
			break;
		case 241  : strcpy(C2F(cha1).buf,_("IDA_NULL_F: user supplied ODE routine is (NULL) illegal."));
			break;
		case 242  : strcpy(C2F(cha1).buf,_("IDA_BAD_NVECTOR: A required vector operation is not implemented."));
			break;
		case 243  : strcpy(C2F(cha1).buf,_("IDA_NULL_ABSTOL: absolute tolerances is = NULL illegal."));
			break;
		case 244  : strcpy(C2F(cha1).buf,_("IDA_BAD_RELTOL: relative tolerances is reltol < 0 illegal."));
			break;
		case 245  : strcpy(C2F(cha1).buf,_("IDA_BAD_ABSTOL: abstol has negative component(s) (illegal)."));
			break;
		case 246  : strcpy(C2F(cha1).buf,_("IDA_NULL_G: user supplied zero-crossing routine is (NULL) illegal."));
			break;
		case 247  : strcpy(C2F(cha1).buf,_("IDA_BAD_TOUT: Trouble interpolating. ''tout'' too far back in direction of integration"));
			break;
		case 248  : strcpy(C2F(cha1).buf,_("IDA_YP0_NULL: the derivative yp0 = NULL is illegal."));
			break;
		case 249  : strcpy(C2F(cha1).buf,_("IDA_RES_NULL: the returned residual res = NULL is illegal."));
			break;
		case 250  : strcpy(C2F(cha1).buf,_("IDA_YRET_NULL: yret = NULL illegal.."));
			break;
		case 251  : strcpy(C2F(cha1).buf,_("IDA_YPRET_NULL: yret = NULL illegal.."));
			break;
		case 252  : strcpy(C2F(cha1).buf,_("IDA_BAD_HINIT: yret = NULL illegal.."));
			break;
		case 253  : strcpy(C2F(cha1).buf,_("IDA_MISSING_ID :id = NULL (''id'' defines algebraic and differential states) but suppressalg option on."));
			break;
		case 254  : strcpy(C2F(cha1).buf,_("IDA_Y0_FAIL_CONSTR: y0 fails to satisfy constraints."));
			break;
		case 255  : strcpy(C2F(cha1).buf,_("IDA_TOO_CLOSE: ''tout'' too close to ''t0'' to start integration."));
			break;
		case 256  : strcpy(C2F(cha1).buf,_("IDA_CLOSE_ROOTS: Root found at and very near starting time."));
			break;
		case 257  : strcpy(C2F(cha1).buf,_("IDA_Residual_NAN: The residual function returns NAN."));
			break;
		case 258  : strcpy(C2F(cha1).buf,_("IDA_Rootfunction_NAN: The Root function returns NAN."));
			break;

			/* CVODE error messages*/
		case 301  : strcpy(C2F(cha1).buf,_("CV_TOO_MUCH_WORK: The solver took mxstep internal steps but could not reach ''tout''. The default value for mxstep is MXSTEP_DEFAULT = 500."));
			break;

		case 302  : strcpy(C2F(cha1).buf,_("CV_TOO_MUCH_ACC: The solver could not satisfy the accuracy demanded by the user for some internal step"));
			break;

		case 303  : strcpy(C2F(cha1).buf,_("CV_ERR_FAILURE: Error test failures occurred too many times (=MXETF = 7) during one internal step or occurred with |h|=h_min "));
			break;

		case 304  : strcpy(C2F(cha1).buf,_("CV_CONV_FAILURE: Convergence test failures occurred too many times (= MXNCF = 10) during one internal time step or occurred with |h| = hmin."));
			break;

		case 305  : strcpy(C2F(cha1).buf,_("CV_LINIT_FAIL: The linear solver''s initialization function failed."));
			break;

		case 306  : strcpy(C2F(cha1).buf,_("CV_LSETUP_FAIL: The linear solver''s setup routine failed in an unrecoverable manner."));
			break;

		case 307  : strcpy(C2F(cha1).buf,_("CV_LSOLVE_FAIL: The linear solver''s solve routine failed in an unrecoverable manner."));
			break;

		case 308  : strcpy(C2F(cha1).buf,_("CV_RHSFUNC_FAIL: The right-hand side function (user supplied ODE) failed in an unrecoverable manner"));
			break;

		case 309  : strcpy(C2F(cha1).buf,_("CV_FIRST_RHSFUNC_ERR: The right-hand side function (user supplied ODE) had a recoverable error at the first call"));
			break;

		case 310  : strcpy(C2F(cha1).buf,_("CV_REPTD_RHSFUNC_ERR: Convergence tests occurred too many times due to repeated recoverable errors in the right-hand side function (user supplied ODE). This error may be raised due to repeated  recoverable errors during the estimation of an initial step size."));
			break;

		case 311  : strcpy(C2F(cha1).buf,_("CV_UNREC_RHSFUNC_ERR: The right-hand side function (user supplied ODE) had a recoverable error, but no recovery was possible."));
			break;
		case 312  : strcpy(C2F(cha1).buf,_("CV_RTFUNC_FAIL: The root finding routine failed in an unrecoverable manner."));
			break;
		case 320  : strcpy(C2F(cha1).buf,_("CV_MEM_FAIL: a memory allocation failed, including an attempt to increase maxord"));
			break;
		case 321  : strcpy(C2F(cha1).buf,_("CV_MEM_NULL: the cvode memory was NULL"));
			break;
		case 322  : strcpy(C2F(cha1).buf,_("CV_ILL_INPUT: indicating an input argument was illegal. This include the situation where a component of the error weight vector becomes negative during internal time-stepping. This also includes if the linear solver function initialization (called by the user after calling CVodeCreat) failed to set the linear solver-specific ''lsolve'' field in cvode_mem. This error happens if number of root functions is positive but the return surface value is NULL. "));
			break;
		case 323  : strcpy(C2F(cha1).buf,_("CV_NO_MALLOC: indicating that cvode_mem has not been allocated (i.e., CVodeMalloc has not been called)."));
			break;
		case 324  : strcpy(C2F(cha1).buf,_("CV_BAD_K: k (the order of the derivative of y to be computed) is not in the range 0, 1, ..., qu, where qu is the order last used"));
			break;
		case 325  : strcpy(C2F(cha1).buf,_("CV_BAD_T: t is not in the interval [tn-hu,tn]."));
			break;
		case 326  : strcpy(C2F(cha1).buf,_("CV_BAD_DKY:  The dky argument was NULL. dky is the output derivative vector [((d/dt)^k)y](t)."));
			break;
		case 327  : strcpy(C2F(cha1).buf,_("CV_TOO_CLOSE: ''tout'' too close to ''t0'' to start integration."));
			break;
		case 328  : strcpy(C2F(cha1).buf,_("CV_YOUT_NULL: ''yout'' = NULL illegal."));
			break;
		case 329  : strcpy(C2F(cha1).buf,_("CV_TRET_NULL: ''tret'' = NULL illegal."));
			break;
		case 330  : strcpy(C2F(cha1).buf,_("CV_BAD_ITASK: Illegal value for itask."));
			break;
		case 331  : strcpy(C2F(cha1).buf,_("CV_NO_ESTOP: itask = CV_NORMAL_TSTOP or itask = CV_ONE_STEP_TSTOP but tstop was not set"));
			break;
		case 332  : strcpy(C2F(cha1).buf,_("CV_BAD_H0: h0 and tout - t0 inconsistent."));
			break;
		case 333  : strcpy(C2F(cha1).buf,_("CV_BAD_TSTOP: tstop is behind current simulation time in the direction of integration."));
			break;
		case 334  : strcpy(C2F(cha1).buf,_("CV_BAD_INIT_ROOT: Root found at and very near initial t."));
			break;
		case 335  : strcpy(C2F(cha1).buf,_("CV_NO_EFUN: itol = CV_WF but no EwtSet function was provided."));
			break;
		case 336  : strcpy(C2F(cha1).buf,_("CV_EWT_FAIL: The user-provide EwtSet function failed."));
			break;
		case 337  : strcpy(C2F(cha1).buf,_("CV_BAD_EWT: Initial ewt has component(s) equal to zero (illegal)."));
			break;
		case 338  : strcpy(C2F(cha1).buf,_("CV_LSOLVE_NULL: The linear solver''s solve routine is NULL."));
			break;
		case 339  : strcpy(C2F(cha1).buf,_("CV_NULL_Y0: y0 = NULL illegal."));
			break;
		case 340  : strcpy(C2F(cha1).buf,_("CV_BAD_ITOL: Illegal value for itol. The legal values are CV_SS, CV_SV, and CV_WF."));
			break;
		case 341  : strcpy(C2F(cha1).buf,_("CV_NULL_F: user supplied ODE routine is (NULL) illegal."));
			break;
		case 342  : strcpy(C2F(cha1).buf,_("CV_BAD_NVECTOR: A required vector operation is not implemented."));
			break;
		case 343  : strcpy(C2F(cha1).buf,_("CV_NULL_ABSTOL: absolute tolerances is = NULL illegal."));
			break;
		case 344  : strcpy(C2F(cha1).buf,_("CV_BAD_RELTOL: relative tolerances is reltol < 0 illegal."));
			break;
		case 345  : strcpy(C2F(cha1).buf,_("CV_BAD_ABSTOL: abstol has negative component(s) (illegal)."));
			break;
		case 346  : strcpy(C2F(cha1).buf,_("CV_NULL_G: user supplied zero-crossing routine is (NULL) illegal."));
			break;
		case 347  : strcpy(C2F(cha1).buf,_("CV_BAD_TOUT: Trouble interpolating. ''tout'' too far back in direction of integration"));
			break;
		case 348  : strcpy(C2F(cha1).buf,_("CV_CLOSE_ROOTS: Root found at and very near starting time."));
			break;
		case 349  : strcpy(C2F(cha1).buf,_("CV_Derivative_NAN: The Derivatives returned by blocks are NAN."));
			break;
		case 350  : strcpy(C2F(cha1).buf,_("CV_Rootfunction_NAN: The Root function returns NAN."));
			break;
		case 401  : strcpy(C2F(cha1).buf,_("KIN_MEM_NULL: An error in the memory allocation in KINCreate or KINMalloc."));
			break;
		case 402  : strcpy(C2F(cha1).buf,_("KIN_ILL_INPUT: A supplied parameter is invalid (check error message)."));
			break;
		case 403  : strcpy(C2F(cha1).buf,_("KIN_NO_MALLOC: Additional system memory has not yet been  allocated for vector storage (forgot to call the  KINMalloc routine)"));
			break;
		case 404  : strcpy(C2F(cha1).buf,_("KIN_MEM_FAIL: An error occurred during memory allocation (either insufficient system resources are available or the vector kernel has not yet been initialized "));
			break;
		case 405  : strcpy(C2F(cha1).buf,_("KIN_LINESEARCH_NONCONV:  The line-search algorithm was unable to find an iterate sufficiently distinct  from the current iterate failure to satisfy the sufficient decrease condition could mean the current iterate is close to an approximate solution of the given nonlinear system, the finite-difference  approximation of the matrix-vector product  J(u)*v is inaccurate, or the real scalar scsteptol is too large "));
			break;
		case 406  : strcpy(C2F(cha1).buf,_("KIN_MAXITER_REACHED: The maximum number of nonlinear iterations has been reached "));
			break;
		case 407  : strcpy(C2F(cha1).buf,_("KIN_MXNEWT_5X_EXCEEDE: Five consecutive steps have been taken that satisfy the following inequality:  ||uscale*p||_L2 > 0.99*mxnewtstep where p denotes the current step and mxnewtstep is a real scalar upper bound on the scaled step length such a failure may mean ||fscale*func(u)||_L2 asymptotes from above to a finite value, or the real scalar mxnewtstep is too small "));
			break;
		case 408 : strcpy(C2F(cha1).buf,_("KIN_LINESEARCH_BCFAI: The line search algorithm (implemented in KINLineSearch) was unable to satisfy the beta-condition for MXNBCF + 1 nonlinear iterations (not necessarily consecutive), which may indicate the algorithm is making  poor progress "));
			break;
		case 409  : strcpy(C2F(cha1).buf,_("KIN_LINSOLV_NO_RECOVERY: The user-supplied routine psolve encountered a recoverable error, but the preconditioner is already current"));
			break;
		case 410  : strcpy(C2F(cha1).buf,_("KIN_LINIT_FAIL: The linear solver initialization routine (linit) encountered an error."));
			break;
		case 411  : strcpy(C2F(cha1).buf,_("KIN_LSETUP_FAIL: The user-supplied routine pset (used to compute the preconditioner) encountered an unrecoverable error ."));
			break;
		case 412  : strcpy(C2F(cha1).buf,_("KIN_LSOLVE_FAIL: Either the user-supplied routine psolve (used to solve the preconditioned linear system) encountered an unrecoverable error, or the linear solver routine (lsolve) encountered an error condition."));
			break;
		case 413  : strcpy(C2F(cha1).buf,_("KIN_SYSFUNC_FAIL: Error in the computing function. Please verify your model."));
			break;
		case 414  : strcpy(C2F(cha1).buf,_("KIN_FIRST_SYSFUNC_ERR: The system function failed at the first call. Please verify your model. There might be an error in the computing function, for example a function may be called with illegal inputs."));
			break;
		case 415  : strcpy(C2F(cha1).buf,_("KIN_REPTD_SYSFUNC_ERR: The system function failed repeatedly. Please verify your model. There might be an error in the computing function, for example a function may be called with illegal inputs"));
			break;
		case 416  : strcpy(C2F(cha1).buf,_("KIN_NAN_ERR: The residual function returns NAN. Please verify your model: some functions might be called with illegal inputs."));
			break;

		default  : if(ierr>=1000)
					   strcpy(C2F(cha1).buf,_("unknown or erroneous block"));
				   else if (ierr>=100)
				   {
					   istate=-(ierr-100);
					   sprintf(C2F(cha1).buf, _("integration problem istate=%d"),istate);
					   C2F(curblk).kfun=0;
				   }
				   else
				   {
					   strcpy(C2F(cha1).buf,_("scicos unexpected error, please report..."));
					   C2F(curblk).kfun=0;
				   }
				   break;
		}
		if (! (C2F(errgst).err1>0||C2F(iop).err>0))
		{
			Scierror(888,"%s\n",C2F(cha1).buf);
			/*C2F(curblk).kfun=0;*/
			C2F(com).fun=0; /*set common fun=0 (this disable bug in debug mode)*/
			freeparam;
			return 0;
		}
	}

	if (C2F(iop).err>0) 
	{
		freeparam;
		return 0;
	}

	/*C2F(curblk).kfun=0;*/
	C2F(com).fun=0;

	/*********************
	* return Lsh variable
	*********************/
	/*copy int parameters of state in double parameters*/
	for(i=0;i<(m1e7*n1e7);i++) ((double *)(il_state_evtspt+4))[i] = (double) l_state_evtspt[i];
	for(i=0;i<(m1e8*n1e8);i++) ((double *)(il_pointi+4))[i] = (double) l_pointi[i];
	/*set lsh var*/
	if (Lhs>=1) LhsVar(1) = 1; /*return state in LhsVar(1)*/
	if (Lhs==2) LhsVar(2) = 2; /*return tcur in LhsVar(2)*/

    /* end */
    freeparam;

	PutLhsVar();
	
	return 0;
}
/*--------------------------------------------------------------------------*/ 
