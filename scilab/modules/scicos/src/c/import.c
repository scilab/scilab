/*  Scicos
*
*  Copyright (C) INRIA -
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
#include <string.h>
#include <stdio.h>
#include "machine.h"
#include "import.h"
#include "scicos.h"
#include "scicos_internal.h"
#include "cvstr.h"
/*--------------------------------------------------------------------------*/
ScicosImport scicos_imp =
{
    (double *)  NULL,      /* x      **  */
    (int *) NULL,      /* nx         */
    (int *) NULL,      /* xptr   **  */
    (int *) NULL,      /* zcptr  **  */
    (double *)  NULL,      /* z      **  */
    (int *) NULL,      /* nz         */
    (int *) NULL,      /* zptr   **  */
    (int *) NULL,      /* noz        */
    (void **)   NULL,      /* oz         */
    (int *) NULL,      /* ozsz       */
    (int *) NULL,      /* oztyp      */
    (int *) NULL,      /* ozptr      */
    (double *)  NULL,      /* g          */
    (int *) NULL,      /* ng         */
    (int *) NULL,      /* mod        */
    (int *) NULL,      /* nmod       */
    (int *) NULL,      /* modptr **  */
    (char **) NULL,      /* iz         */
    (int *) NULL,      /* izptr      */
    (char **) NULL,      /* uid        */
    (int *) NULL,      /* uidptr     */
    (int *) NULL,      /* inpptr **  */
    (int *) NULL,      /* inplnk **  */
    (int *) NULL,      /* outptr **  */
    (int *) NULL,      /* outlnk **  */
    (double *)  NULL,      /* rpar   **  */
    (int *) NULL,      /* rpptr  **  */
    (int *) NULL,      /* ipar   **  */
    (int *) NULL,      /* ipptr  **  */
    (void **)   NULL,      /* opar       */
    (int *) NULL,      /* oparsz     */
    (int *) NULL,      /* opartyp    */
    (int *) NULL,      /* opptr      */
    (int *) NULL,      /* nblk       */
    (void **)   NULL,      /* outtbptr   */
    (int *) NULL,      /* outtbsz    */
    (int *) NULL,      /* outtbtyp   */
    (int *) NULL,      /* nlnk       */
    (int *) NULL,      /* subs   **  */
    (int *) NULL,      /* nsubs      */
    (double *)  NULL,      /* tevts  **  */
    (int *) NULL,      /* evtspt **  */
    (int *) NULL,      /* nevts      */
    (int *) NULL,      /* pointi **  */
    (int *) NULL,      /* iord   **  */
    (int *) NULL,      /* niord      */
    (int *) NULL,      /* oord   **  */
    (int *) NULL,      /* noord      */
    (int *) NULL,      /* zord   **  */
    (int *) NULL,      /* nzord      */
    (int *) NULL,      /* funptr     */
    (int *) NULL,      /* funtyp **  */
    (int *) NULL,      /* ztyp   **  */
    (int *) NULL,      /* cord   **  */
    (int *) NULL,      /* ncord      */
    (int *) NULL,      /* ordclk **  */
    (int *) NULL,      /* nordclk ** */
    (int *) NULL,      /* clkptr **  */
    (int *) NULL,      /* ordptr **  */
    (int *) NULL,      /* nordptr    */
    (int *) NULL,      /* critev **  */
    (int *) NULL,      /* iwa        */
    (int *) NULL,      /* mask       */
    (scicos_block *) NULL, /* blocks     */
    (double *)  NULL,      /* t0         */
    (double *)  NULL,      /* tf         */
    (double *)  NULL,      /* Atol       */
    (double *)  NULL,      /* rtol       */
    (double *)  NULL,      /* ttol       */
    (double *)  NULL,      /* deltat     */
    (double *)  NULL,      /* hmax       */
    (outtb_el *) NULL,     /* outtb_elem */
    (int *) NULL,      /* nelem      */
    (int *) NULL,      /* xprop      */
    (double *)  NULL,      /* xd         */
};

/*--------------------------------------------------------------------------*/
/* getscicosimportptr returns a pointer
 * to the imported structure
 */
ScicosImport* getscicosimportptr(void)
{
    return &scicos_imp;
}
/*--------------------------------------------------------------------------*/
void makescicosimport(double *x, int *nx,
                      int *xptr, int *zcptr,
                      double *z, int *nz, int *zptr,
                      int *noz, void **oz, int *ozsz, int *oztyp, int *ozptr,
                      double *g, int *ng,
                      int *mod, int *nmod, int *modptr,
                      char **iz, int *izptr, char **uid, int *uidptr, int *inpptr, int *inplnk,
                      int *outptr, int *outlnk, void **outtbptr, int *outtbsz, int *outtbtyp,
                      outtb_el *outtb_elem, int *nelem,
                      int *nlnk, double *rpar, int *rpptr, int *ipar, int *ipptr,
                      void **opar, int *oparsz, int *opartyp, int *opptr,
                      int *nblk, int *subs, int *nsubs,
                      double *tevts, int *evtspt, int *nevts, int *pointi,
                      int *iord, int *niord, int *oord, int *noord, int *zord, int *nzord,
                      int *funptr, int *funtyp, int *ztyp,
                      int *cord, int *ncord, int *ordclk, int *nordclk, int *clkptr,
                      int *ordptr, int *nordptr, int *critev,  int *iwa, scicos_block *blocks,
                      double *t0, double *tf, double *Atol, double *rtol, double *ttol, double *deltat, double *hmax,
                      int *xprop, double *xd)
{
    scicos_imp.x = x;
    scicos_imp.nx = nx;
    scicos_imp.xptr = xptr;
    scicos_imp.zcptr = zcptr;
    scicos_imp.z = z;
    scicos_imp.nz = nz;
    scicos_imp.zptr = zptr;

    scicos_imp.noz = noz;
    scicos_imp.oz = oz;
    scicos_imp.ozsz = ozsz;
    scicos_imp.oztyp = oztyp;
    scicos_imp.ozptr = ozptr;

    scicos_imp.g = g;
    scicos_imp.ng = ng;
    scicos_imp.mod = mod;
    scicos_imp.nmod = nmod;
    scicos_imp.modptr = modptr;
    scicos_imp.iz = iz;
    scicos_imp.izptr = izptr;
    scicos_imp.uid = uid;
    scicos_imp.uidptr = uidptr;

    scicos_imp.inpptr = inpptr;
    scicos_imp.inplnk = inplnk;
    scicos_imp.outptr = outptr;
    scicos_imp.outlnk = outlnk;

    scicos_imp.rpar = rpar;
    scicos_imp.rpptr = rpptr;
    scicos_imp.ipar = ipar;
    scicos_imp.ipptr = ipptr;

    scicos_imp.opar = opar;
    scicos_imp.oparsz = oparsz;
    scicos_imp.opartyp = opartyp;
    scicos_imp.opptr = opptr;

    scicos_imp.nblk = nblk;
    scicos_imp.outtbptr = outtbptr;
    scicos_imp.outtbsz = outtbsz;
    scicos_imp.outtbtyp = outtbtyp;
    scicos_imp.outtb_elem = outtb_elem;
    scicos_imp.nelem = nelem;
    scicos_imp.nlnk = nlnk;

    scicos_imp.subs = subs;
    scicos_imp.nsubs = nsubs;

    scicos_imp.tevts = tevts;
    scicos_imp.evtspt = evtspt;
    scicos_imp.nevts = nevts;
    scicos_imp.pointi = pointi;

    scicos_imp.iord = iord;
    scicos_imp.niord = niord;
    scicos_imp.oord = oord;
    scicos_imp.noord = noord;
    scicos_imp.zord = zord;
    scicos_imp.nzord = nzord;

    scicos_imp.funptr = funptr;
    scicos_imp.funtyp = funtyp;

    scicos_imp.ztyp = ztyp;
    scicos_imp.cord = cord;
    scicos_imp.ncord = ncord;
    scicos_imp.ordclk = ordclk;
    scicos_imp.nordclk = nordclk;
    scicos_imp.clkptr = clkptr;
    scicos_imp.ordptr = ordptr;
    scicos_imp.nordptr = nordptr;
    scicos_imp.critev = critev;
    scicos_imp.iwa = iwa;
    scicos_imp.blocks = blocks;

    scicos_imp.t0 = t0;
    scicos_imp.tf = tf;
    scicos_imp.Atol = Atol;
    scicos_imp.ttol = ttol;
    scicos_imp.rtol = rtol;
    scicos_imp.deltat = deltat;
    scicos_imp.hmax = hmax;

    scicos_imp.xprop = xprop;
    scicos_imp.xd = xd;
}
/*--------------------------------------------------------------------------*/
void C2F(clearscicosimport)()
{
    scicos_imp.x = (double *) NULL;
    scicos_imp.nx = (int *) NULL;
    scicos_imp.xptr = (int *) NULL;
    scicos_imp.zcptr = (int *) NULL;
    scicos_imp.z = (double *) NULL;
    scicos_imp.nz = (int *) NULL;
    scicos_imp.zptr = (int *) NULL;

    scicos_imp.noz = (int *) NULL;
    scicos_imp.oz = (void **) NULL;
    scicos_imp.ozsz = (int *) NULL;
    scicos_imp.oztyp = (int *) NULL;
    scicos_imp.ozptr = (int *) NULL;

    scicos_imp.g = (double *) NULL;
    scicos_imp.ng = (int *) NULL;
    scicos_imp.mod = (int *) NULL;
    scicos_imp.nmod = (int *) NULL;
    scicos_imp.modptr = (int *) NULL;
    scicos_imp.iz = (char **) NULL;
    scicos_imp.izptr = (int *) NULL;
    scicos_imp.uid = (char **) NULL;
    scicos_imp.uidptr = (int *) NULL;

    scicos_imp.inpptr = (int *) NULL;
    scicos_imp.inplnk = (int *) NULL;
    scicos_imp.outptr = (int *) NULL;
    scicos_imp.outlnk = (int *) NULL;

    scicos_imp.rpar = (double *) NULL;
    scicos_imp.rpptr = (int *) NULL;
    scicos_imp.ipar = (int *) NULL;
    scicos_imp.ipptr = (int *) NULL;

    scicos_imp.opar = (void **) NULL;
    scicos_imp.oparsz = (int *) NULL;
    scicos_imp.opartyp = (int *) NULL;
    scicos_imp.opptr = (int *) NULL;

    scicos_imp.nblk = (int *) NULL;
    scicos_imp.outtbptr = (void **) NULL;
    scicos_imp.outtbsz = (int *) NULL;
    scicos_imp.outtbtyp = (int *) NULL;
    scicos_imp.outtb_elem = (outtb_el *) NULL;
    scicos_imp.nelem = (int *) NULL;
    scicos_imp.nlnk = (int *) NULL;

    scicos_imp.subs = (int *) NULL;
    scicos_imp.nsubs = (int *) NULL;
    scicos_imp.tevts = (double *) NULL;
    scicos_imp.evtspt = (int *) NULL;
    scicos_imp.nevts = (int *) NULL;
    scicos_imp.pointi = (int *) NULL;

    scicos_imp.iord = (int *) NULL;
    scicos_imp.niord = (int *) NULL;
    scicos_imp.oord = (int *) NULL;
    scicos_imp.noord = (int *) NULL;
    scicos_imp.zord = (int *) NULL;
    scicos_imp.nzord = (int *) NULL;

    scicos_imp.funptr = (int *) NULL;
    scicos_imp.funtyp = (int *) NULL;

    scicos_imp.ztyp = (int *) NULL;
    scicos_imp.cord = (int *) NULL;
    scicos_imp.ncord = (int *) NULL;
    scicos_imp.ordclk = (int *) NULL;
    scicos_imp.nordclk = (int *) NULL;
    scicos_imp.clkptr = (int *) NULL;
    scicos_imp.ordptr = (int *) NULL;
    scicos_imp.nordptr = (int *) NULL;
    scicos_imp.critev = (int *) NULL;

    scicos_imp.iwa = (int *) NULL;
    scicos_imp.mask = (int *) NULL;
    scicos_imp.blocks = (scicos_block *) NULL;

    scicos_imp.t0 = (double *) NULL;
    scicos_imp.tf = (double *) NULL;
    scicos_imp.Atol = (double *) NULL;
    scicos_imp.ttol = (double *) NULL;
    scicos_imp.rtol = (double *) NULL;
    scicos_imp.deltat = (double *) NULL;
    scicos_imp.hmax = (double *) NULL;

    scicos_imp.xprop = (int *) NULL;
    scicos_imp.xd = (double *) NULL;
}
/*--------------------------------------------------------------------------*/
/* 20/06/06, Alan : review
 * 08/02/07, Alan : update
 */

int getscicosvarsfromimport(char *what, void **v, int *nv, int *mv)
/*char *what;   data structure selection -see import.h for definition-*/
/*void **v;     Pointer to the beginning of the imported data */
/*int *nv;      size 1 of the imported data */
/*int *mv; size 1 of the imported data */
{
    /*variable declaration*/
    int nx, nz, noz, nmod, nblk, nlnk, nsubs, nevts, ng;
    int niord, noord, ncord, nordptr, nzord, nelem;

    /*test if scicosim is running*/
    if (scicos_imp.x == (double *)NULL)
    {
        v = (void *) NULL;
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
    if (strcmp(what, "x") == 0)
    {
        /* x - continuous state */
        *nv = (int) (scicos_imp.xptr[nblk] - scicos_imp.xptr[0]);
        *mv = 1;
        *v  = (double *)(scicos_imp.x);
    }
    else if (strcmp(what, "nx") == 0)
    {
        /* length of x register */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.nx);
    }
    else if (strcmp(what, "xptr") == 0)
    {
        /* xptr - continuous state splitting array */
        *nv = nblk + 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.xptr);
    }
    else if (strcmp(what, "zcptr") == 0)
    {
        /* zcptr - zero crossing splitting array */
        *nv = nblk + 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.zcptr);
    }
    else if (strcmp(what, "z") == 0)
    {
        /* z - discrete state */
        *nv = (int)(scicos_imp.zptr[nblk] - scicos_imp.zptr[0]);
        *mv = 1;
        *v  = (double *) (scicos_imp.z);
    }
    else if (strcmp(what, "nz") == 0)
    {
        /* length of z register */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.nz);
    }
    else if (strcmp(what, "noz") == 0)
    {
        /* length of oz register */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.noz);
    }
    else if (strcmp(what, "oz") == 0)
    {
        /* oz - vector of ptr of object discrete states */
        *nv = (int)(scicos_imp.ozptr[nblk] - scicos_imp.ozptr[0]);
        *mv = 1;
        *v  = (int *) (scicos_imp.oz);
    }
    else if (strcmp(what, "ozptr") == 0)
    {
        /* ozptr - object discrete states splitting array */
        *nv = nblk + 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.ozptr);
    }
    else if (strcmp(what, "ozsz") == 0)
    {
        /* oparsz - object discrete states size array */
        *nv = (int)(scicos_imp.ozptr[nblk] - scicos_imp.ozptr[0]);
        *mv = 2;
        *v  = (int *) (scicos_imp.ozsz);
    }
    else if (strcmp(what, "oztyp") == 0)
    {
        /* opartyp - object discrete states type array */
        *nv = (int)(scicos_imp.ozptr[nblk] - scicos_imp.ozptr[0]);
        *mv = 1;
        *v  = (int *) (scicos_imp.oztyp);
    }
    else if (strcmp(what, "zptr") == 0)
    {
        /* zptr - discrete state splitting array */
        *nv = nblk + 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.zptr);
    }
    else if (strcmp(what, "mod") == 0)
    {
        /* modes - block discontinuities array */
        *nv = (int)(scicos_imp.modptr[nblk] - scicos_imp.modptr[0]);
        *mv = 1;
        *v  = (int *) (scicos_imp.mod);
    }
    else if (strcmp(what, "nmod") == 0)
    {
        /* nmodes - number of block discontinuities array */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.nmod);
    }
    else if (strcmp(what, "modptr") == 0)
    {
        /* modptr - block discontinuities splitting array */
        *nv = nblk + 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.modptr);
    }
    else if (strcmp(what, "iz") == 0)
    {
        /* iz - label integer code of blocks array */
        *nv = (int)(scicos_imp.izptr[nblk] - scicos_imp.izptr[0]);
        *mv = 1;
        *v  = (char **) (scicos_imp.iz);
    }
    else if (strcmp(what, "izptr") == 0)
    {
        /* izptr - label integer code of blocks splitting array */
        *nv = nblk + 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.izptr);
    }
    else if (strcmp(what, "uid") == 0)
    {
        /* uid */
        *nv = (int)(scicos_imp.uidptr[nblk] - scicos_imp.uidptr[0]);
        *mv = 1;
        *v  = (char **) (scicos_imp.uid);
    }
    else if (strcmp(what, "uidptr") == 0)
    {
        /* uidptr */
        *nv = nblk + 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.uidptr);
    }
    else if (strcmp(what, "inpptr") == 0)
    {
        /* inpptr */
        *nv = nblk + 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.inpptr);
    }
    else if (strcmp(what, "inplnk") == 0)
    {
        /* inplnk */
        *nv = (int)(scicos_imp.inpptr[nblk] - scicos_imp.inpptr[0]);
        *mv = 1;
        *v  = (int *) (scicos_imp.inplnk);
    }
    else if (strcmp(what, "outptr") == 0)
    {
        /* outptr */
        *nv = nblk + 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.outptr);
    }
    else if (strcmp(what, "outlnk") == 0)
    {
        /* outlnk */
        *nv = (int)(scicos_imp.outptr[nblk] - scicos_imp.outptr[0]);
        *mv = 1;
        *v  = (int *) (scicos_imp.outlnk);
    }
    else if (strcmp(what, "rpar") == 0)
    {
        /* rpar - vector of real parameters */
        *nv = (int)(scicos_imp.rpptr[nblk] - scicos_imp.rpptr[0]);
        *mv = 1;
        *v  = (double *) (scicos_imp.rpar);
    }
    else if (strcmp(what, "rpptr") == 0)
    {
        /* rpptr - real parameters splitting array */
        *nv = nblk + 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.rpptr);
    }
    else if (strcmp(what, "ipar") == 0)
    {
        /* ipar - vector of integer parameters */
        *nv = (int)(scicos_imp.ipptr[nblk] - scicos_imp.ipptr[0]);
        *mv = 1;
        *v  = (int *) (scicos_imp.ipar);
    }
    else if (strcmp(what, "ipptr") == 0)
    {
        /* ipptr - integer parameters splitting array */
        *nv = nblk + 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.ipptr);
    }
    else if (strcmp(what, "opar") == 0)
    {
        /* opar - vector of ptr of objects parameters */
        *nv = (int)(scicos_imp.opptr[nblk] - scicos_imp.opptr[0]);
        *mv = 1;
        *v  = (int *) (scicos_imp.opar);
    }
    else if (strcmp(what, "opptr") == 0)
    {
        /* opptr - object parameters splitting array */
        *nv = nblk + 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.opptr);
    }
    else if (strcmp(what, "oparsz") == 0)
    {
        /* oparsz - object parameters size array */
        *nv = (int)(scicos_imp.opptr[nblk] - scicos_imp.opptr[0]);
        *mv = 2;
        *v  = (int *) (scicos_imp.oparsz);
    }
    else if (strcmp(what, "opartyp") == 0)
    {
        /* opartyp - object parameters type array */
        *nv = (int)(scicos_imp.opptr[nblk] - scicos_imp.opptr[0]);
        *mv = 1;
        *v  = (int *) (scicos_imp.opartyp);
    }
    else if (strcmp(what, "nblk") == 0)
    {
        /* number of block */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.nblk);
    }
    else if (strcmp(what, "outtbptr") == 0)
    {
        /* ptr on output register */
        *nv = nlnk;
        *mv = 1;
        *v  = (int *) (scicos_imp.outtbptr);
    }
    else if (strcmp(what, "outtbsz") == 0)
    {
        /* size of output register */
        *nv = nlnk;
        *mv = 2;
        *v  = (int *) (scicos_imp.outtbsz);
    }
    else if (strcmp(what, "outtbtyp") == 0)
    {
        /* type of output register  */
        *nv = nlnk;
        *mv = 1;
        *v  = (int *) (scicos_imp.outtbtyp);
    }
    else if (strcmp(what, "nlnk") == 0)
    {
        /* number of link  */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.nlnk);
    }
    else if (strcmp(what, "subs") == 0) /* Aquoisertsubs?? */
    {
        /*   */
        *nv = nsubs;
        *mv = 1;
        *v  = (int *) (scicos_imp.subs);
    }
    else if (strcmp(what, "nsubs") == 0)
    {
        /*   */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.nsubs);
    }
    else if (strcmp(what, "tevts") == 0)
    {
        /*   */
        *nv = nevts;
        *mv = 1;
        *v  = (double *) (scicos_imp.tevts);
    }
    else if (strcmp(what, "evtspt") == 0)
    {
        /*   */
        *nv = nevts;
        *mv = 1;
        *v  = (int *) (scicos_imp.evtspt);
    }
    else if (strcmp(what, "nevts") == 0)
    {
        /*   */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.nevts);
    }
    else if (strcmp(what, "pointi") == 0)
    {
        /* */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.pointi);
    }
    else if (strcmp(what, "iord") == 0)
    {
        /* */
        *nv = niord;
        *mv = 2;
        *v  = (int *) (scicos_imp.iord);
    }
    else if (strcmp(what, "niord") == 0)
    {
        /* */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.niord);
    }
    else if (strcmp(what, "oord") == 0)
    {
        /* */
        *nv = noord;
        *mv = 2;
        *v  = (int *) (scicos_imp.oord);
    }
    else if (strcmp(what, "noord") == 0)
    {
        /* */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.noord);
    }
    else if (strcmp(what, "zord") == 0)
    {
        /* */
        *nv = nzord;
        *mv = 2;
        *v  = (int *) (scicos_imp.zord);
    }
    else if (strcmp(what, "nzord") == 0)
    {
        /* */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.nzord);
    }
    else if (strcmp(what, "funptr") == 0)
    {
        /* */
        *nv = nblk;
        *mv = 1;
        *v  = (int *) (scicos_imp.funptr);
    }
    else if (strcmp(what, "funtyp") == 0)
    {
        /* */
        *nv = nblk;
        *mv = 1;
        *v  = (int *) (scicos_imp.funtyp);
    }
    else if (strcmp(what, "ztyp") == 0)
    {
        /* */
        *nv = nblk;
        *mv = 1;
        *v  = (int *) (scicos_imp.ztyp);
    }
    else if (strcmp(what, "cord") == 0)
    {
        /* */
        *nv = ncord;
        *mv = 2;
        *v  = (int *) (scicos_imp.cord);
    }
    else if (strcmp(what, "ncord") == 0)
    {
        /* */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.ncord);
    }
    else if (strcmp(what, "ordclk") == 0)
    {
        /* */
        *nv = (int)(scicos_imp.ordptr[nordptr] - 1);
        *mv = 2;
        *v  = (int *) (scicos_imp.ordclk);
    }
    else if (strcmp(what, "clkptr") == 0)
    {
        /* */
        *nv = (int)(nblk + 1);
        *mv = 1;
        *v  = (int *) (scicos_imp.clkptr);
    }
    else if (strcmp(what, "ordptr") == 0)
    {
        /* */
        *nv = nordptr;
        *mv = 1;
        *v  = (int *) (scicos_imp.ordptr);
    }
    else if (strcmp(what, "nordptr") == 0)
    {
        /* */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.nordptr);
    }
    else if (strcmp(what, "critev") == 0)
    {
        /* */
        *nv = (int)(scicos_imp.clkptr[nblk] - 1); /* !! a faire verifier !! */
        *mv = 1;
        *v  = (int *) (scicos_imp.critev);
    }
    else if (strcmp(what, "iwa") == 0)
    {
        /* */
        *nv = nevts; /* !! a faire verifier !! */
        *mv = 1;
        *v  = (int *) (scicos_imp.iwa);
    }
    else if (strcmp(what, "blocks") == 0)
    {
        /* */
        *nv = 1;
        *mv = 1;
        *v  = (scicos_block *) (scicos_imp.blocks);
    }
    else if (strcmp(what, "ng") == 0)
    {
        /* */
        *nv = 1;
        *mv = 1;
        *v  = (int *) (scicos_imp.ng);
    }
    else if (strcmp(what, "g") == 0)
    {
        /* g */
        *nv = ng;
        *mv = 1;
        *v  = (double *) (scicos_imp.g);
    }
    else if (strcmp(what, "t0") == 0)
    {
        /* g */
        *nv = 1;
        *mv = 1;
        *v  = (double *) (scicos_imp.t0);
    }
    else if (strcmp(what, "tf") == 0)
    {
        /* g */
        *nv = 1;
        *mv = 1;
        *v  = (double *) (scicos_imp.tf);
    }
    else if (strcmp(what, "Atol") == 0)
    {
        /* g */
        *nv = 1;
        *mv = 1;
        *v  = (double *) (scicos_imp.Atol);
    }
    else if (strcmp(what, "rtol") == 0)
    {
        /* g */
        *nv = 1;
        *mv = 1;
        *v  = (double *) (scicos_imp.rtol);
    }
    else if (strcmp(what, "ttol") == 0)
    {
        /* g */
        *nv = 1;
        *mv = 1;
        *v  = (double *) (scicos_imp.ttol);
    }
    else if (strcmp(what, "deltat") == 0)
    {
        /* g */
        *nv = 1;
        *mv = 1;
        *v  = (double *) (scicos_imp.deltat);
    }
    else if (strcmp(what, "hmax") == 0)
    {
        /* g */
        *nv = 1;
        *mv = 1;
        *v  = (double *) (scicos_imp.hmax);
    }
    else if (strcmp(what, "nelem") == 0)
    {
        /* g */
        *nv = 1;
        *mv = 1;
        *v  = (int*) (scicos_imp.nelem);
    }
    else if (strcmp(what, "outtb_elem") == 0)
    {
        /* */
        *nv = nelem;
        *mv = 2;
        *v  = (outtb_el *) (scicos_imp.outtb_elem);
    }
    else /*return FALSE_*/
    {
        return 0;
    }

    /* return TRUE_ */
    return 1;
}
/*--------------------------------------------------------------------------*/
/* Used in some scicos block */
void C2F(getlabel)(int *kfun, char *label, int *n)
/*int *n, *kfun;  length of the label as input n gives the max length expected*/
{
    int k;
    int job = 1;

    k = *kfun;
    if (*n > (int)(scicos_imp.izptr[k] - scicos_imp.izptr[k - 1]))
    {
        *n = (int)(scicos_imp.izptr[k] - scicos_imp.izptr[k - 1]);
    }
    if (*n > 0 )
    {
        strcpy(label, scicos_imp.iz[k]);
    }
}

/*never used, never interfaced */
void C2F(getblockbylabel)(int *kfun, char **label, int *n)
{
    int k, i, i0, nblk, n1;
    int job = 0;

    nblk = scicos_imp.nblk[0];

    *kfun = 0;
    for (k = 0; k < nblk; k++)
    {
        n1 = (int)(scicos_imp.izptr[k] - scicos_imp.izptr[k - 1]);
        if (n1 == *n)
        {
            i0 = scicos_imp.izptr[k - 1] - 1;
            i = 0;
            while ((label[i] == scicos_imp.iz[i0 + i]) & (i < n1))
            {
                i++;
            }
            if (i == n1)
            {
                *kfun = k + 1;
                return;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
/*never used, never interfaced */
int C2F(getsciblockbylabel)(int*kfun, int label[], int *n)
{
    int k, i, i0, nblk, n1;
    int job = 1;
    char* lab[100];
    if (scicos_imp.x == (double *)NULL)
    {
        return (2); /* undefined import table scicos is not running */
    }
    nblk = scicos_imp.nblk[0];

    F2C(cvstr)(n, lab, *label, &job, *n);

    *kfun = 0;
    for (k = 0; k < nblk; k++)
    {
        n1 = (int)(scicos_imp.izptr[k] - scicos_imp.izptr[k - 1]);
        if (n1 == *n)
        {
            i0 = scicos_imp.izptr[k - 1] - 1;
            i = 0;
            while ((lab[i] == scicos_imp.iz[i0 + i]) & (i < n1))
            {
                i++;
            }
            if (i == n1)
            {
                *kfun = k + 1;
                return 0;
            }
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int getscilabel(int *kfun, char *label, int *n)
{
    int k;

    if (scicos_imp.x == (double *)NULL)
    {
        return 2; /* undefined import table scicos is not running */
    }
    k = *kfun;

    *n = (int)(scicos_imp.izptr[k] - scicos_imp.izptr[k - 1]);
    if (*n > 0 )
    {
        strcpy(label, scicos_imp.iz[k]);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(getcurblock)()
{
    return (C2F(curblk).kfun);
}
/*--------------------------------------------------------------------------*/
/* used in fscope
 *
 * 30/06/06, Alan : Rewritte to preserve compatibility with fscope.f.
 * Only first element of matrix is delivred and converted to double data.
 *
 */
void C2F(getouttb)(int *nsize, int *nvec, double *outtc)
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
    int j, sz, lnk, pos;

    /*get outtbptr from import struct.*/
    outtbptr = scicos_imp.outtbptr;
    /*get outtb_elem from import struct.*/
    outtb_elem = scicos_imp.outtb_elem;
    /*get outtbsz from import struct.*/
    outtbsz = scicos_imp.outtbsz;
    /*get max number of elem in outtb*/
    outtb_nelem = scicos_imp.nelem;

    /*initialization of position in outtc */
    j = 0;

    while (j < *nsize)
    {
        /*test to know if we are outside outtb_elem*/
        if (nvec[j] > (*outtb_nelem))
        {
            set_block_error(-1);
            return;
        }

        lnk = outtb_elem[nvec[j] - 1].lnk;
        pos = outtb_elem[nvec[j] - 1].pos;
        outtbtyp = scicos_imp.outtbtyp[lnk];

        /*double data type*/
        if (outtbtyp == SCSREAL_N)
        {
            outtbdptr = (SCSREAL_COP *)outtbptr[lnk];
            outtc[j] = (double)outtbdptr[pos];
            j++;
        }
        /*complex data type*/
        else if (outtbtyp == SCSCOMPLEX_N)
        {
            sz = outtbsz[2 * lnk] + outtbsz[(2 * lnk) + 1];
            outtbdptr = (SCSCOMPLEX_COP *)outtbptr[lnk];
            outtc[j] =  (double)outtbdptr[pos];
            /*outtc[j+1] =  (double)outtbdptr[pos+sz];*/
            /*j=j+2;*/
            j++;
        }
        /*int data type*/
        else
        {
            switch (outtbtyp)
            {
                case SCSINT8_N   :
                    outtbcptr = (SCSINT8_COP *)outtbptr[lnk]; /*int8*/
                    outtc[j] = (double)outtbcptr[pos];
                    j++;
                    break;

                case SCSINT16_N  :
                    outtbsptr = (SCSINT16_COP *)outtbptr[lnk]; /*int16*/
                    outtc[j] = (double)outtbsptr[pos];
                    j++;
                    break;

                case SCSINT32_N  :
                    outtblptr = (SCSINT32_COP *)outtbptr[lnk]; /*int32*/
                    outtc[j] = (double)outtblptr[pos];
                    j++;
                    break;

                case SCSUINT8_N  :
                    outtbucptr = (SCSUINT8_COP *)outtbptr[lnk]; /*uint8*/
                    outtc[j] = (double)outtbucptr[pos];
                    j++;
                    break;

                case SCSUINT16_N :
                    outtbusptr = (SCSUINT16_COP *)outtbptr[lnk]; /*uint16*/
                    outtc[j] = (double)outtbusptr[pos];
                    j++;
                    break;

                case SCSUINT32_N :
                    outtbulptr = (SCSUINT32_COP *)outtbptr[lnk]; /*uint32*/
                    outtc[j] = (double)outtbulptr[pos];
                    j++;
                    break;

                default        :
                    outtc[j] = 0;
                    j++;
                    break;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
