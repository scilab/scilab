/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
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
#ifndef INTCSCICOS_SCI
#define INTCSCICOS_SCI

#include "scicos_block4.h"

/* function declaration */
int MlistGetFieldNumber(int *ptr, const char *string);
int C2F(intcscicos)(void);
int intendscicosim(char *fname, unsigned long fname_len);
int inttimescicos(char *fname, unsigned long fname_len);
int intduplicate(char *fname, unsigned long fname_len);
int intdiffobjs(char *fname, unsigned long fname_len);
int intxproperty(char *fname, unsigned long fname_len);
int intphasesim(char *fname, unsigned long fname_len);
int intsetxproperty(char *fname, unsigned long fname_len);
int intsetblockerror(char *fname, unsigned long fname_len);
int inttree2(char *fname, unsigned long fname_len);
int inttree3(char *fname, unsigned long fname_len);
int inttree4(char *fname, unsigned long fname_len);
static void duplicata();
static void comp_size();
int var2sci(void *x,int n,int m,int typ_var);
int createblklist(scicos_block *Blocks, int *ierr, int flag_imp, int kfun);
int intbuildouttb(char *fname,unsigned long fname_len);
int intcurblkc(char *fname,unsigned long fname_len);
int intgetscicosvarsc(char *fname,unsigned long fname_len);
int intpermutobj_c(char *fname,unsigned long fname_len);
int intscicosimc(char *fname,unsigned long fname_len);
/* Definition of some global variables and structure */

static int *il_state_save; /*n'est valable que dans inctscicos.c*/
static int *il_sim_save; /*n'est valable que dans inctscicos.c*/

typedef struct /* declaration of outtb_elem -valable dans inctscicos.c- */
{
  int lnk;
  int pos;
} outtb_el;

/*external function declaration*/

extern int getscicosvarsfromimport(char *what,void *v,int *nv,int *mv);
extern void C2F(vvtosci)();
extern void C2F(itosci)();
extern int C2F(mktlist)();
extern void str2sci(char** x,int n,int m);
extern int C2F(scierr)();
extern int C2F(namstr)();

extern int Max1(int* vect);
extern int *listentry(int *header, int i);
extern int Message(char* code);

extern int ctree2(int* vect,int nb,int* deput,
                  int* depuptr,int* outoin,
                  int* outoinptr, int* ord,
                  int* nord,int* ok);

extern int ctree3(int*vec,int nb,int* depu,
                  int* depuptr,int* typl,int* bexe,
                  int* boptr,int* blnk,int* blptr,
                  int* ord,int* nord,int* ok);

extern int ctree4(int* vec,int nb,int *nd,
                  int nnd,int* typ_r,int* outoin,
                  int* outoinptr,int* r1,int* r2,
                  int* nr);

extern int C2F(scicos)(double *x_in, int *xptr_in, double *z__,
                       void **work,int *zptr,int *modptr_in,
                       void **oz,int *ozsz,int *oztyp,int *ozptr,
                       int *iz,int *izptr,double *t0_in,
                       double *tf_in,double *tevts_in,int *evtspt_in,
                       int *nevts,int *pointi_in,void **outtbptr_in,
                       int *outtbsz_in,int *outtbtyp_in,
                       outtb_el *outtb_elem_in,int *nelem1,int *nlnk1,
                       int *funptr,int *funtyp_in,int *inpptr_in,
                       int *outptr_in, int *inplnk_in,int *outlnk_in,
                       double *rpar,int *rpptr,int *ipar,int *ipptr,
                       void **opar,int *oparsz,int *opartyp,int *opptr,
                       int *clkptr_in,int *ordptr_in,int *nordptr1,
                       int *ordclk_in,int *cord_in,int *ncord1,
                       int *iord_in,int *niord1,int *oord_in,
                       int *noord1,int *zord_in,int *nzord1,
                       int *critev_in,int *nblk1,int *ztyp,
                       int *zcptr_in,int *subscr,int *nsubs,
                       double *simpar,int *flag__,int *ierr_out);

#endif
