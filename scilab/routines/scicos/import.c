/* Copyright INRIA */

/* 21/06/06, Alan : update import structure with
 * noord, nzord, ncord, nordptr, niord, iord, nmod, mod.
 * Rewrite getscicosvars
 *
 * 23-27/06/06, Alan : update import structure with blocks, ng, g.
 *
 * 29/06/06, Alan : update import struc. with t0,tf, Atol, ttol,rtol, deltat,
 * hmax. All is now ptr in this struct.
 *
 * 03/07/06, Alan : update import struc. with nelem, outtb_elem
 *
 * 13/11/06, Alan : fix bugs in getscicosvarsfromimport
 *
 * 08/02/07, Alan : Update with oz,opar
 */

#include "../machine.h"
#include <string.h>
#include "import.h"
#include <stdio.h>

/*********************************************
 * external structure and function declaration
 *********************************************/
extern  int C2F(cvstr)  __PARAMS((integer *,integer *,char *,integer *,unsigned long int));
extern struct {integer kfun;} C2F(curblk);

ScicosImport  scicos_imp={
(double *)  NULL,      /* 1  - x      **  */
(integer *) NULL,      /* 2  - nx         */
(integer *) NULL,      /* 3  - xptr   **  */
(integer *) NULL,      /* 4  - zcptr  **  */
(double *)  NULL,      /* 5  - z      **  */
(integer *) NULL,      /* 6  - nz         */
(integer *) NULL,      /* 7  - zptr   **  */
(integer *) NULL,      /* 8  - noz        */
(void **)   NULL,      /* 9  - oz         */
(integer *) NULL,      /* 10 - ozsz       */
(integer *) NULL,      /* 11 - oztyp      */
(integer *) NULL,      /* 12 - ozptr      */
(double *)  NULL,      /* 13 - g          */
(integer *) NULL,      /* 14 - ng         */
(integer *) NULL,      /* 15 - mod        */
(integer *) NULL,      /* 16 - nmod       */
(integer *) NULL,      /* 17 - modptr **  */
(integer *) NULL,      /* 18 - iz         */
(integer *) NULL,      /* 19 - izptr      */
(integer *) NULL,      /* 20 - inpptr **  */
(integer *) NULL,      /* 21 - inplnk **  */
(integer *) NULL,      /* 22 - outptr **  */
(integer *) NULL,      /* 23 - outlnk **  */
(double *)  NULL,      /* 24 - rpar   **  */
(integer *) NULL,      /* 25 - rpptr  **  */
(integer *) NULL,      /* 26 - ipar   **  */
(integer *) NULL,      /* 27 - ipptr  **  */
(void **)   NULL,      /* 28 - opar       */
(integer *) NULL,      /* 29 - oparsz     */
(integer *) NULL,      /* 30 - opartyp    */
(integer *) NULL,      /* 31 - opptr      */
(integer *) NULL,      /* 32 - nblk       */
(void **)   NULL,      /* 33 - outtbptr   */
(integer *) NULL,      /* 34 - outtbsz    */
(integer *) NULL,      /* 35 - outtbtyp   */
(integer *) NULL,      /* 36 - nlnk       */
(integer *) NULL,      /* 37 - subs   **  */
(integer *) NULL,      /* 38 - nsubs      */
(double *)  NULL,      /* 39 - tevts  **  */
(integer *) NULL,      /* 40 - evtspt **  */
(integer *) NULL,      /* 41 - nevts      */
(integer *) NULL,      /* 42 - pointi **  */
(integer *) NULL,      /* 43 - iord   **  */
(integer *) NULL,      /* 44 - niord      */
(integer *) NULL,      /* 45 - oord   **  */
(integer *) NULL,      /* 46 - noord      */
(integer *) NULL,      /* 47 - zord   **  */
(integer *) NULL,      /* 48 - nzord      */
(integer *) NULL,      /* 49 - funptr     */
(integer *) NULL,      /* 50 - funtyp **  */
(integer *) NULL,      /* 51 - ztyp   **  */
(integer *) NULL,      /* 52 - cord   **  */
(integer *) NULL,      /* 53 - ncord      */
(integer *) NULL,      /* 54 - ordclk **  */
(integer *) NULL,      /* 55 - clkptr **  */
(integer *) NULL,      /* 56 - ordptr **  */
(integer *) NULL,      /* 57 - nordptr    */
(integer *) NULL,      /* 58 - critev **  */
(integer *) NULL,      /* 59 - iwa        */
(integer *) NULL,      /* 60 - mask       */
(scicos_block *) NULL, /* 61 - blocks     */
(double *)  NULL,      /* 62 - t0         */
(double *)  NULL,      /* 63 - tf         */
(double *)  NULL,      /* 64 - Atol       */
(double *)  NULL,      /* 65 - rtol       */
(double *)  NULL,      /* 66 - ttol       */
(double *)  NULL,      /* 67 - deltat     */
(double *)  NULL,      /* 68 - hmax       */
(outtb_el *) NULL,     /* 69 - outtb_elem */
(integer *) NULL,      /* 70 - nelem      */
};

void
C2F(makescicosimport)(x,nx,xptr,zcptr,z,nz,zptr,
                      noz,oz,ozsz,oztyp,ozptr,
                      g,ng,mod,nmod,modptr,iz,izptr,
                      inpptr,inplnk,outptr,outlnk,
                      outtbptr,outtbsz,outtbtyp,
                      outtb_elem,nelem,
                      nlnk,rpar,rpptr,ipar,ipptr,
                      opar,oparsz,opartyp,opptr,
                      nblk,subs,nsubs,
                      tevts,evtspt,nevts,pointi,
                      iord,niord,oord,noord,zord,nzord,
                      funptr,funtyp,ztyp,
                      cord,ncord,ordclk,clkptr,ordptr,nordptr,
                      critev,iwa,blocks,
                      t0,tf,Atol,rtol,ttol,deltat,hmax)

double  *x ,*z,*rpar,*tevts,*g;
integer *xptr,*zcptr,*zptr,*iz,*izptr,*inpptr,*inplnk,*outptr,*outlnk;
void    **outtbptr;
void    **oz;
void    **opar;
integer *outtbsz,*outtbtyp;
integer *ozsz,*oztyp;
integer *oparsz,*opartyp;
outtb_el *outtb_elem;
integer *nelem;
integer *nx,*nz,*ng,*nlnk,*rpptr,*ipar,*ipptr,*nblk,*subs,*nsubs;
integer *noz,*ozptr,*opptr;
integer *evtspt,*nevts,*pointi,*iord,*niord,*oord,*noord,*zord,*nzord;
integer *funptr,*funtyp,*ztyp,*cord,*ncord,*ordclk;
integer *clkptr,*ordptr,*nordptr,*critev, *iwa, *mod,*nmod,*modptr;
double  *t0,*tf,*Atol,*rtol,*ttol,*deltat,*hmax;
scicos_block *blocks;

{
    scicos_imp.x=x;
    scicos_imp.nx=nx;
    scicos_imp.xptr=xptr;
    scicos_imp.zcptr=zcptr;
    scicos_imp.z=z;
    scicos_imp.nz=nz;
    scicos_imp.zptr=zptr;

    scicos_imp.noz=noz;
    scicos_imp.oz=oz;
    scicos_imp.ozsz=ozsz;
    scicos_imp.oztyp=oztyp;
    scicos_imp.ozptr=ozptr;

    scicos_imp.g=g;
    scicos_imp.ng=ng;
    scicos_imp.mod=mod;
    scicos_imp.nmod=nmod;
    scicos_imp.modptr=modptr;
    scicos_imp.iz=iz;
    scicos_imp.izptr=izptr;

    scicos_imp.inpptr=inpptr;
    scicos_imp.inplnk=inplnk;
    scicos_imp.outptr=outptr;
    scicos_imp.outlnk=outlnk;

    scicos_imp.rpar=rpar;
    scicos_imp.rpptr=rpptr;
    scicos_imp.ipar=ipar;
    scicos_imp.ipptr=ipptr;

    scicos_imp.opar=opar;
    scicos_imp.oparsz=oparsz;
    scicos_imp.opartyp=opartyp;
    scicos_imp.opptr=opptr;

    scicos_imp.nblk=nblk;
    scicos_imp.outtbptr=outtbptr;
    scicos_imp.outtbsz=outtbsz;
    scicos_imp.outtbtyp=outtbtyp;
    scicos_imp.outtb_elem=outtb_elem;
    scicos_imp.nelem=nelem;
    scicos_imp.nlnk=nlnk;

    scicos_imp.subs=subs;
    scicos_imp.nsubs=nsubs;

    scicos_imp.tevts=tevts;
    scicos_imp.evtspt=evtspt;
    scicos_imp.nevts=nevts;
    scicos_imp.pointi=pointi;

    scicos_imp.iord=iord;
    scicos_imp.niord=niord;
    scicos_imp.oord=oord;
    scicos_imp.noord=noord;
    scicos_imp.zord=zord;
    scicos_imp.nzord=nzord;

    scicos_imp.funptr=funptr;
    scicos_imp.funtyp=funtyp;

    scicos_imp.ztyp=ztyp;
    scicos_imp.cord=cord;
    scicos_imp.ncord=ncord;
    scicos_imp.ordclk=ordclk;
    scicos_imp.clkptr=clkptr;
    scicos_imp.ordptr=ordptr;
    scicos_imp.nordptr=nordptr;
    scicos_imp.critev=critev;
    scicos_imp.iwa=iwa;
    scicos_imp.blocks=blocks;

    scicos_imp.t0=t0;
    scicos_imp.tf=tf;
    scicos_imp.Atol=Atol;
    scicos_imp.ttol=ttol;
    scicos_imp.rtol=rtol;
    scicos_imp.deltat=deltat;
    scicos_imp.hmax=hmax;
}

void
C2F(clearscicosimport)()
{
    scicos_imp.x=(double *) NULL;
    scicos_imp.nx=(integer *) NULL;
    scicos_imp.xptr=(integer *) NULL;
    scicos_imp.zcptr=(integer *) NULL;
    scicos_imp.z=(double *) NULL;
    scicos_imp.nz=(integer *) NULL;
    scicos_imp.zptr=(integer *) NULL;

    scicos_imp.noz=(integer *) NULL;
    scicos_imp.oz=(void **) NULL;
    scicos_imp.ozsz=(integer *) NULL;
    scicos_imp.oztyp=(integer *) NULL;
    scicos_imp.ozptr=(integer *) NULL;

    scicos_imp.g=(double *) NULL;
    scicos_imp.ng=(integer *) NULL;
    scicos_imp.mod=(integer *) NULL;
    scicos_imp.nmod=(integer *) NULL;
    scicos_imp.modptr=(integer *) NULL;
    scicos_imp.iz=(integer *) NULL;
    scicos_imp.izptr=(integer *) NULL;

    scicos_imp.inpptr=(integer *) NULL;
    scicos_imp.inplnk=(integer *) NULL;
    scicos_imp.outptr=(integer *) NULL;
    scicos_imp.outlnk=(integer *) NULL;

    scicos_imp.rpar=(double *) NULL;
    scicos_imp.rpptr=(integer *) NULL;
    scicos_imp.ipar=(integer *) NULL;
    scicos_imp.ipptr=(integer *) NULL;

    scicos_imp.opar=(void **) NULL;
    scicos_imp.oparsz=(integer *) NULL;
    scicos_imp.opartyp=(integer *) NULL;
    scicos_imp.opptr=(integer *) NULL;

    scicos_imp.nblk=(integer *) NULL;
    scicos_imp.outtbptr=(void **) NULL;
    scicos_imp.outtbsz=(integer *) NULL;
    scicos_imp.outtbtyp=(integer *) NULL;
    scicos_imp.outtb_elem=(outtb_el *) NULL;
    scicos_imp.nelem=(integer *) NULL;
    scicos_imp.nlnk=(integer *) NULL;

    scicos_imp.subs=(integer *) NULL;
    scicos_imp.nsubs=(integer *) NULL;
    scicos_imp.tevts=(double *) NULL;
    scicos_imp.evtspt=(integer *) NULL;
    scicos_imp.nevts=(integer *) NULL;
    scicos_imp.pointi=(integer *) NULL;

    scicos_imp.iord=(integer *) NULL;
    scicos_imp.niord=(integer *) NULL;
    scicos_imp.oord=(integer *) NULL;
    scicos_imp.noord=(integer *) NULL;
    scicos_imp.zord=(integer *) NULL;
    scicos_imp.nzord=(integer *) NULL;

    scicos_imp.funptr=(integer *) NULL;
    scicos_imp.funtyp=(integer *) NULL;

    scicos_imp.ztyp=(integer *) NULL;
    scicos_imp.cord=(integer *) NULL;
    scicos_imp.ncord=(integer *) NULL;
    scicos_imp.ordclk=(integer *) NULL;
    scicos_imp.clkptr=(integer *) NULL;
    scicos_imp.ordptr=(integer *) NULL;
    scicos_imp.nordptr=(integer *) NULL;
    scicos_imp.critev=(integer *) NULL;

    scicos_imp.iwa=(integer *) NULL;
    scicos_imp.mask=(integer *) NULL;
    scicos_imp.blocks=(scicos_block *) NULL;

    scicos_imp.t0=(double *) NULL;
    scicos_imp.tf=(double *) NULL;
    scicos_imp.Atol=(double *) NULL;
    scicos_imp.ttol=(double *) NULL;
    scicos_imp.rtol=(double *) NULL;
    scicos_imp.deltat=(double *) NULL;
    scicos_imp.hmax=(double *) NULL;
}

/* 20/06/06, Alan : review
 * 08/02/07, Alan : update
 */

int getscicosvarsfromimport(what,v,nv,mv)
char *what;   /* data structure selection -see import.h for definition-*/
void **v;     /* Pointer to the beginning of the imported data */
int *nv;      /* size 1 of the imported data */
int *mv;      /* size 1 of the imported data */
{
    /*variable declaration*/
    int nx,nz,noz,nmod,nblk,nlnk,nsubs,nevts,ng;
    int niord,noord,ncord,nordptr,nzord,nelem;

    /*test if scicosim is running*/
    if (scicos_imp.x==(double *)NULL)
    {
     v=(void *) NULL;
     return 0; /* undefined import table scicos is not running */
    }

    /* retrieve length of x register */
    nx = (int) scicos_imp.nx[0];
    /* retrieve length of z register */
    nz = (int) scicos_imp.nz[0];
    /* retrieve length of oz register */
    noz = (int) scicos_imp.noz[0];
    /* retrieve number of block */
    nmod = (int) scicos_imp.nmod[0];
    /* retrieve number of block */
    nblk = (int) scicos_imp.nblk[0];
    /* retrieve number of link */
    nlnk = (int) scicos_imp.nlnk[0];
    /* retrieve number nsubs */
    nsubs = (int) scicos_imp.nsubs[0];
    /* retrieve number nevts */
    nevts = (int) scicos_imp.nevts[0];
    /* retrieve number niord */
    niord = (int) scicos_imp.niord[0];
    /* retrieve number noord */
    noord = (int) scicos_imp.noord[0];
    /* retrieve number ncord */
    ncord = (int) scicos_imp.ncord[0];
    /* retrieve number nordptr */
    nordptr = (int) scicos_imp.nordptr[0];
    /* retrieve number nzord */
    nzord = (int) scicos_imp.nzord[0];
    /* retrieve number ng */
    ng = (int) scicos_imp.ng[0];
    /* retrieve number nelem */
    nelem = (int) scicos_imp.nelem[0];

    /* imported from */
    if (strcmp(what,"x") == 0)
    { /* x - continuous state */
      *nv = (int) (scicos_imp.xptr[nblk]-scicos_imp.xptr[0]);
      *mv = 1;
      *v  = (double *)(scicos_imp.x);
    }
    else if (strcmp(what,"nx") == 0)
    { /* length of x register */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.nx);
    }
    else if (strcmp(what,"xptr") == 0)
    { /* xptr - continuous state splitting array */
      *nv = nblk + 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.xptr);
    }
    else if (strcmp(what,"zcptr") == 0)
    { /* zcptr - zero crossing splitting array */
      *nv = nblk + 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.zcptr);
    }
    else if (strcmp(what,"z") == 0)
    { /* z - discrete state */
      *nv = (int)(scicos_imp.zptr[nblk]-scicos_imp.zptr[0]);
      *mv = 1;
      *v  = (double *) (scicos_imp.z);
    }
    else if (strcmp(what,"nz") == 0)
    { /* length of z register */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.nz);
    }
    else if (strcmp(what,"noz") == 0)
    { /* length of oz register */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.noz);
    }
    else if (strcmp(what,"oz") == 0)
    { /* oz - vector of ptr of object discrete states */
      *nv = (int)(scicos_imp.ozptr[nblk]-scicos_imp.ozptr[0]);
      *mv = 1;
      *v  = (int *) (scicos_imp.oz);
    }
    else if (strcmp(what,"ozptr") == 0)
    { /* ozptr - object discrete states splitting array */
      *nv = nblk + 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.ozptr);
    }
    else if (strcmp(what,"ozsz") == 0)
    { /* oparsz - object discrete states size array */
      *nv = (int)(scicos_imp.ozptr[nblk]-scicos_imp.ozptr[0]);
      *mv = 2;
      *v  = (int *) (scicos_imp.ozsz);
    }
    else if (strcmp(what,"oztyp") == 0)
    { /* opartyp - object discrete states type array */
      *nv = (int)(scicos_imp.ozptr[nblk]-scicos_imp.ozptr[0]);
      *mv = 1;
      *v  = (int *) (scicos_imp.oztyp);
    }
    else if (strcmp(what,"zptr") == 0)
    { /* zptr - discrete state splitting array */
      *nv = nblk + 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.zptr);
    }
    else if (strcmp(what,"mod") == 0)
    { /* modes - block discontinuities array */
      *nv = (int)(scicos_imp.modptr[nblk]-scicos_imp.modptr[0]);
      *mv = 1;
      *v  = (int *) (scicos_imp.mod);
    }
    else if (strcmp(what,"nmod") == 0)
    { /* nmodes - number of block discontinuities array */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.nmod);
    }
    else if (strcmp(what,"modptr") == 0)
    { /* modptr - block discontinuities splitting array */
      *nv = nblk + 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.modptr);
    }
    else if (strcmp(what,"iz") == 0)
    { /* iz - label integer code of blocks array */
      *nv = (int)(scicos_imp.izptr[nblk]-scicos_imp.izptr[0]);
      *mv = 1;
      *v  = (int *) (scicos_imp.iz);
    }
    else if (strcmp(what,"izptr") == 0)
    { /* izptr - label integer code of blocks splitting array */
      *nv = nblk + 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.izptr);
    }
    else if (strcmp(what,"inpptr") == 0)
    { /* inpptr */
      *nv = nblk + 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.inpptr);
    }
    else if (strcmp(what,"inplnk") == 0)
    { /* inplnk */
      *nv = (int)(scicos_imp.inpptr[nblk]-scicos_imp.inpptr[0]);
      *mv = 1;
      *v  = (int *) (scicos_imp.inplnk);
    }
    else if (strcmp(what,"outptr") == 0)
    { /* outptr */
      *nv = nblk + 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.outptr);
    }
    else if (strcmp(what,"outlnk") == 0)
    { /* outlnk */
      *nv = (int)(scicos_imp.outptr[nblk]-scicos_imp.outptr[0]);
      *mv=1;
      *v  = (int *) (scicos_imp.outlnk);
    }
    else if (strcmp(what,"rpar") == 0)
    { /* rpar - vector of real parameters */
      *nv = (int)(scicos_imp.rpptr[nblk]-scicos_imp.rpptr[0]);
      *mv = 1;
      *v  = (double *) (scicos_imp.rpar);
    }
    else if (strcmp(what,"rpptr") == 0)
    { /* rpptr - real parameters splitting array */
      *nv = nblk + 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.rpptr);
    }
    else if (strcmp(what,"ipar") == 0)
    { /* ipar - vector of integer parameters */
      *nv = (int)(scicos_imp.ipptr[nblk]-scicos_imp.ipptr[0]);
      *mv = 1;
      *v  = (int *) (scicos_imp.ipar);
    }
    else if (strcmp(what,"ipptr") == 0)
    { /* ipptr - integer parameters splitting array */
      *nv = nblk + 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.ipptr);
    }
    else if (strcmp(what,"opar") == 0)
    { /* opar - vector of ptr of objects parameters */
      *nv = (int)(scicos_imp.opptr[nblk]-scicos_imp.opptr[0]);
      *mv = 1;
      *v  = (int *) (scicos_imp.opar);
    }
    else if (strcmp(what,"opptr") == 0)
    { /* opptr - object parameters splitting array */
      *nv = nblk + 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.opptr);
    }
    else if (strcmp(what,"oparsz") == 0)
    { /* oparsz - object parameters size array */
      *nv = (int)(scicos_imp.opptr[nblk]-scicos_imp.opptr[0]);
      *mv = 2;
      *v  = (int *) (scicos_imp.oparsz);
    }
    else if (strcmp(what,"opartyp") == 0)
    { /* opartyp - object parameters type array */
      *nv = (int)(scicos_imp.opptr[nblk]-scicos_imp.opptr[0]);
      *mv = 1;
      *v  = (int *) (scicos_imp.opartyp);
    }
    else if (strcmp(what,"nblk") == 0)
    { /* number of block */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.nblk);
    }
    else if (strcmp(what,"outtbptr") == 0)
    { /* ptr on output register */
      *nv = nlnk;
      *mv = 1;
      *v  = (int *) (scicos_imp.outtbptr);
    }
    else if (strcmp(what,"outtbsz") == 0)
    { /* size of output register */
      *nv = nlnk;
      *mv = 2;
      *v  = (int *) (scicos_imp.outtbsz);
    }
    else if (strcmp(what,"outtbtyp") == 0)
    { /* type of output register  */
      *nv = nlnk;
      *mv = 1;
      *v  = (int *) (scicos_imp.outtbtyp);
    }
    else if (strcmp(what,"nlnk") == 0)
    { /* number of link  */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.nlnk);
    }
    else if (strcmp(what,"subs") == 0) /* Aquoisertsubs?? */
    { /*   */
      *nv = nsubs;
      *mv = 1;
      *v  = (int *) (scicos_imp.subs);
    }
    else if (strcmp(what,"nsubs") == 0)
    { /*   */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.nsubs);
    }
    else if (strcmp(what,"tevts") == 0)
    { /*   */
      *nv = nevts;
      *mv = 1;
      *v  = (double *) (scicos_imp.tevts);
    }
    else if (strcmp(what,"evtspt") == 0)
    { /*   */
      *nv = nevts;
      *mv = 1;
      *v  = (int *) (scicos_imp.evtspt);
    }
    else if (strcmp(what,"nevts") == 0)
    { /*   */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.nevts);
    }
    else if (strcmp(what,"pointi") == 0)
    { /* */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.pointi);
    }
    else if (strcmp(what,"iord") == 0)
    { /* */
      *nv = niord;
      *mv = 2;
      *v  = (int *) (scicos_imp.iord);
    }
    else if (strcmp(what,"niord") == 0)
    { /* */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.niord);
    }
    else if (strcmp(what,"oord") == 0)
    { /* */
      *nv = noord;
      *mv = 2;
      *v  = (int *) (scicos_imp.oord);
    }
    else if (strcmp(what,"noord") == 0)
    { /* */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.noord);
    }
    else if (strcmp(what,"zord") == 0)
    { /* */
      *nv = nzord;
      *mv = 2;
      *v  = (int *) (scicos_imp.zord);
    }
    else if (strcmp(what,"nzord") == 0)
    { /* */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.nzord);
    }
    else if (strcmp(what,"funptr") == 0)
    { /* */
      *nv = nblk;
      *mv = 1;
      *v  = (int *) (scicos_imp.funptr);
    }
    else if (strcmp(what,"funtyp") == 0)
    { /* */
      *nv = nblk;
      *mv = 1;
      *v  = (int *) (scicos_imp.funtyp);
    }
    else if (strcmp(what,"ztyp") == 0)
    { /* */
      *nv = nblk;
      *mv = 1;
      *v  = (int *) (scicos_imp.ztyp);
    }
    else if (strcmp(what,"cord") == 0)
    { /* */
      *nv = ncord;
      *mv = 2;
      *v  = (int *) (scicos_imp.cord);
    }
    else if (strcmp(what,"ncord") == 0)
    { /* */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.ncord);
    }
    else if (strcmp(what,"ordclk") == 0)
    { /* */
      *nv = (int)(scicos_imp.ordptr[nordptr]-1);
      *mv = 2;
      *v  = (int *) (scicos_imp.ordclk);
    }
    else if (strcmp(what,"clkptr") == 0)
    { /* */
      *nv = (int)(nblk+1);
      *mv = 1;
      *v  = (int *) (scicos_imp.clkptr);
    }
    else if (strcmp(what,"ordptr") == 0)
    { /* */
      *nv = nordptr;
      *mv = 1;
      *v  = (int *) (scicos_imp.ordptr);
    }
    else if (strcmp(what,"nordptr") == 0)
    { /* */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.nordptr);
    }
    else if (strcmp(what,"critev") == 0)
    { /* */
      *nv = (int)(scicos_imp.clkptr[nblk]-1); /* !! à faire vérifier !! */
      *mv = 1;
      *v  = (int *) (scicos_imp.critev);
    }
    else if (strcmp(what,"iwa") == 0)
    { /* */
      *nv = nevts; /* !! à faire vérifier !! */
      *mv = 1;
      *v  = (int *) (scicos_imp.iwa);
    }
    else if (strcmp(what,"blocks") == 0)
    { /* */
      *nv = 1;
      *mv = 1;
      *v  = (scicos_block *) (scicos_imp.blocks);
    }
    else if (strcmp(what,"ng") == 0)
    { /* */
      *nv = 1;
      *mv = 1;
      *v  = (int *) (scicos_imp.ng);
    }
    else if (strcmp(what,"g") == 0)
    { /* g */
      *nv = ng;
      *mv = 1;
      *v  = (double *) (scicos_imp.g);
    }
    else if (strcmp(what,"t0") == 0)
    { /* g */
      *nv = 1;
      *mv = 1;
      *v  = (double *) (scicos_imp.t0);
    }
    else if (strcmp(what,"tf") == 0)
    { /* g */
      *nv = 1;
      *mv = 1;
      *v  = (double *) (scicos_imp.tf);
    }
    else if (strcmp(what,"Atol") == 0)
    { /* g */
      *nv = 1;
      *mv = 1;
      *v  = (double *) (scicos_imp.Atol);
    }
    else if (strcmp(what,"rtol") == 0)
    { /* g */
      *nv = 1;
      *mv = 1;
      *v  = (double *) (scicos_imp.rtol);
    }
    else if (strcmp(what,"ttol") == 0)
    { /* g */
      *nv = 1;
      *mv = 1;
      *v  = (double *) (scicos_imp.ttol);
    }
    else if (strcmp(what,"deltat") == 0)
    { /* g */
      *nv = 1;
      *mv = 1;
      *v  = (double *) (scicos_imp.deltat);
    }
    else if (strcmp(what,"hmax") == 0)
    { /* g */
      *nv = 1;
      *mv = 1;
      *v  = (double *) (scicos_imp.hmax);
    }
    else if (strcmp(what,"nelem") == 0)
    { /* g */
      *nv = 1;
      *mv = 1;
      *v  = (int*) (scicos_imp.nelem);
    }
    else if (strcmp(what,"outtb_elem") == 0)
    { /* */
      *nv = nelem;
      *mv = 2;
      *v  = (outtb_el *) (scicos_imp.outtb_elem);
    }
    else /*return FALSE_*/
        return 0;

    /* return TRUE_ */
    return 1;
}

/* Used in some scicos block */
void C2F(getlabel)(kfun,label,n)
       integer *n, *kfun;  /* length of the label 
                              as input n gives the max length expected*/
char *label;
{
    int k;
    int job=1;

    k= *kfun;
    if (*n>(integer)(scicos_imp.izptr[k]-scicos_imp.izptr[k-1])){
      *n=(integer)(scicos_imp.izptr[k]-scicos_imp.izptr[k-1]);
    }
    if (*n>0 )
      F2C(cvstr)(n,&(scicos_imp.iz[scicos_imp.izptr[k-1]-1]),label,&job,*n);
}

/*never used, never interfaced */
void C2F(getblockbylabel)(kfun,label,n)
       integer *n, *kfun;  /* length of the label */
char **label;
{
    int k,i,i0,nblk,n1;
    int job=0;
    int lab[40];

    nblk=(integer)(scicos_imp.nblk);
    F2C(cvstr)(n,lab,*label,&job,*n);

    *kfun=0;
    for (k=0;k<nblk;k++) {
      n1=(integer)(scicos_imp.izptr[k]-scicos_imp.izptr[k-1]);
      if (n1==*n) {
	i0=scicos_imp.izptr[k-1]-1;
	i=0;
	while ((lab[i]==scicos_imp.iz[i0+i])&(i<n1)) i++;
	if (i==n1) {
	  *kfun=k+1;
	  return;
	}
      }
    }
}

/*never used, never interfaced */
integer C2F(getsciblockbylabel)(kfun,label,n)
       integer *n, *kfun;  /* length of the label */
       integer label[];
{
    int k,i,i0,nblk,n1;
    if (scicos_imp.x==(double *)NULL){
	return(2); /* undefined import table scicos is not running */
    }
    nblk=(integer)(scicos_imp.nblk);

    *kfun=0;
    for (k=0;k<nblk;k++) {
      n1=(integer)(scicos_imp.izptr[k]-scicos_imp.izptr[k-1]);
      if (n1==*n) {
	i0=scicos_imp.izptr[k-1]-1;
	i=0;
	while ((label[i]==scicos_imp.iz[i0+i])&(i<n1)) i++;
	if (i==n1) {
	  *kfun=k+1;
	  return 0;
	}
      }
    }
    return 0;
}

integer C2F(getscilabel)(kfun,label,n)
       integer *n, *kfun;  /* length of the label */
       integer label[];
{
    int k,i;
    integer *u,*y;

    if (scicos_imp.x==(double *)NULL){
	return(2); /* undefined import table scicos is not running */
    }
    k= *kfun;
    *n=(integer)(scicos_imp.izptr[k]-scicos_imp.izptr[k-1]);
    if (*n>0 ) {
	u=(integer *)&(scicos_imp.iz[scicos_imp.izptr[k-1]-1]);
	y=label;
	for (i=0;i<*n;i++)
	    *(y++)=*(u++);  
	}
    return(0);
}

/* */
integer C2F(getcurblock)()
{
  return(C2F(curblk).kfun);
}

/* used in fscope
 *
 * 30/06/06, Alan : Rewritte to preserve compatibility with fscope.f.
 * Only first element of matrix is delivred and converted to double data.
 *
 */
void C2F(getouttb)(nsize,nvec,outtc)
integer *nsize,*nvec;
double *outtc;

{
  /* declaration of ptr for typed port */
  void **outtbptr;            /*to store outtbptr*/
  SCSREAL_COP *outtbdptr;     /*to store double of outtb*/
  SCSINT8_COP *outtbcptr;     /*to store int8 of outtb*/
  SCSINT16_COP *outtbsptr;    /*to store int16 of outtb*/
  SCSINT32_COP *outtblptr;    /*to store int32 of outtb*/
  SCSUINT8_COP *outtbucptr;   /*to store unsigned int8 of outtb */
  SCSUINT16_COP *outtbusptr;  /*to store unsigned int16 of outtb */
  SCSUINT32_COP *outtbulptr;  /*to store unsigned int32 of outtb */
  int *outtb_nelem;           /*to store maximum number of element*/
  int outtbtyp;               /*to store type of data*/
  int *outtbsz;               /*to store size of data*/
  outtb_el *outtb_elem;       /*to store ptr of outtb_elem structure */

  /*auxiliary variable*/
  int j,sz,lnk,pos;

  /*get outtbptr from import struct.*/
  outtbptr=scicos_imp.outtbptr;
  /*get outtb_elem from import struct.*/
  outtb_elem=scicos_imp.outtb_elem;
  /*get outtbsz from import struct.*/
  outtbsz=scicos_imp.outtbsz;
  /*get max number of elem in outtb*/
  outtb_nelem=scicos_imp.nelem;

  /*initialization of position in outtc */
  j=0;

  while (j<*nsize)
  {
   /*test to know if we are outside outtb_elem*/
   if (nvec[j]>(*outtb_nelem)) {
    set_block_error(-1);
    return;
   }

   lnk=outtb_elem[nvec[j]-1].lnk;
   pos=outtb_elem[nvec[j]-1].pos;
   outtbtyp=scicos_imp.outtbtyp[lnk];

   /*double data type*/
   if (outtbtyp==SCSREAL_N)
   {
    outtbdptr=(SCSREAL_COP *)outtbptr[lnk];
    outtc[j]=(double)outtbdptr[pos];
    j++;
   }
   /*complex data type*/
   else if (outtbtyp==SCSCOMPLEX_N)
   {
    sz = outtbsz[2*lnk]+outtbsz[(2*lnk)+1];
    outtbdptr = (SCSCOMPLEX_COP *)outtbptr[lnk];
    outtc[j] =  (double)outtbdptr[pos];
    /*outtc[j+1] =  (double)outtbdptr[pos+sz];*/
    /*j=j+2;*/
    j++;
   }
   /*integer data type*/
   else
   {
    switch(outtbtyp)
    {
     case SCSINT8_N   : outtbcptr=(SCSINT8_COP *)outtbptr[lnk]; /*int8*/
                        outtc[j]=(double)outtbcptr[pos];
                        j++;
                        break;

     case SCSINT16_N  : outtbsptr=(SCSINT16_COP *)outtbptr[lnk]; /*int16*/
                        outtc[j]=(double)outtbsptr[pos];
                        j++;
                        break;

     case SCSINT32_N  : outtblptr=(SCSINT32_COP *)outtbptr[lnk]; /*int32*/
                        outtc[j]=(double)outtblptr[pos];
                        j++;
                        break;

     case SCSUINT8_N  : outtbucptr=(SCSUINT8_COP *)outtbptr[lnk]; /*uint8*/
                        outtc[j]=(double)outtbucptr[pos];
                        j++;
                        break;

     case SCSUINT16_N : outtbusptr=(SCSUINT16_COP *)outtbptr[lnk]; /*uint16*/
                        outtc[j]=(double)outtbusptr[pos];
                        j++;
                        break;

     case SCSUINT32_N : outtbulptr=(SCSUINT32_COP *)outtbptr[lnk]; /*uint32*/
                        outtc[j]=(double)outtbulptr[pos];
                        j++;
                        break;

     default        : outtc[j]=0;
                      j++;
                      break;
    }
   }
  }
}
