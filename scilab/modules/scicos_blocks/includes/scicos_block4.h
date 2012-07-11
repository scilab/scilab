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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/

#ifndef __SCICOS_BLOCK4_H__
#define __SCICOS_BLOCK4_H__

#ifndef NULL
#define NULL 0
#endif

#include <math.h>
#include <stdlib.h>

#ifdef _MSC_VER
typedef void (*voidg) ();
#else
typedef void (*voidg) (void);
#endif

/* scicos_block structure definition
* WARNING: take care that this sructure is
* not the same as the one in scicos_block.h
* i.e same name but inptr and outptr are void **
*/
typedef struct
{
    int nevprt;
    voidg funpt;
    int type;
    int scsptr;
    int nz;
    double *z;
    int noz;
    int *ozsz;
    int *oztyp;
    void **ozptr;
    int nx;
    double *x;
    double *xd;
    double *res;
    int *xprop;
    int nin;
    int *insz;
    void **inptr;
    int nout;
    int *outsz;
    void **outptr;
    int nevout;
    double *evout;
    int nrpar;
    double *rpar;
    int nipar;
    int *ipar;
    int nopar;
    int *oparsz;
    int *opartyp;
    void **oparptr;
    int ng;
    double *g;
    int ztyp;
    int *jroot;
    char *label;
    void **work;
    int nmode;
    int *mode;
} scicos_block;

/** define scicos flag number */
typedef enum
{
    /* Should not be used directly, set through the API. */
    CosError = -5,

    /* Valid simulation states */
    DerivativeState = 0,
    OutputUpdate = 1,
    StateUpdate = 2,
    OutputEventTiming = 3,
    Initialization = 4,
    Ending = 5,
    ReInitialization = 6,
    ContinousPropertiesUpdate = 7,
    ZeroCrossing = 9,
    Residute = 10
} scicos_flag;

/* utility function for block declaration */
int get_block_error(void);
int *get_pointer_xproperty(void);
int get_npointer_xproperty(void);
double Get_Jacobian_cj(void);
double Get_Jacobian_ci(void);
double Get_Scicos_SQUR(void);
void Set_Jacobian_flag(int flag);

int Convert_number(char *, double *);
void homotopy(double *);
int hfjac_(double *, double *, int *);
int rhojac_(double *, double *, double *, double *, int *, double *, int *);
int rho_(double *, double *, double *, double *, double *, int *);
int fx_(double *, double *);
int read_xml_initial_states(int nvar, const char *xmlfile, char **ids, double *svars);
int write_xml_states(int, const char *, char **, double *);

/** Define scicos simulator data type number (_N) */
typedef enum
{
    SCSREAL_N = 10,
    SCSCOMPLEX_N = 11,
    SCSINT_N = 80,
    SCSINT8_N = 81,
    SCSINT16_N = 82,
    SCSINT32_N = 84,
    SCSUINT_N = 800,
    SCSUINT8_N = 811,
    SCSUINT16_N = 812,
    SCSUINT32_N = 814,
    SCSUNKNOW_N = -1
} scicos_datatype_number;

/* Define scicos simulator data type C operators (_COP) */
#define SCSREAL_COP double
#define SCSCOMPLEX_COP double
#define SCSINT_COP int
#define SCSINT8_COP char
#define SCSINT16_COP short
#define SCSINT32_COP long
#define SCSUINT_COP unsigned int
#define SCSUINT8_COP unsigned char
#define SCSUINT16_COP unsigned short
#define SCSUINT32_COP unsigned long
#define SCSUNKNOW_COP double

/* scicos_block macros definition :
*
* 1  - GetNin(blk)
* 2  - GetInPortPtrs(blk,x)
* 3  - GetNout(blk)
* 4  - GetOutPortPtrs(blk,x)
* 5  - GetInPortRows(blk,x)
* 6  - GetInPortCols(blk,x)
* 7  - GetInPortSize(blk,x,y)
* 8  - GetInType(blk,x)
* 9  - GetOutPortRows(blk,x)
* 10 - GetOutPortCols(blk,x)
* 11 - GetOutPortSize(blk,x,y)
* 12 - GetOutType(blk,x)
* 13 - GetRealInPortPtrs(blk,x)
* 14 - GetImagInPortPtrs(blk,x)
* 15 - GetRealOutPortPtrs(blk,x)
* 16 - GetImagOutPortPtrs(blk,x)
* 17 - Getint8InPortPtrs(blk,x)
* 18 - Getint16InPortPtrs(blk,x)
* 19 - Getint32InPortPtrs(blk,x)
* 20 - Getuint8InPortPtrs(blk,x)
* 21 - Getuint16InPortPtrs(blk,x)
* 22 - Getuint32InPortPtrs(blk,x)
* 23 - Getint8OutPortPtrs(blk,x)
* 24 - Getint16OutPortPtrs(blk,x)
* 25 - Getint32OutPortPtrs(blk,x)
* 26 - Getuint8OutPortPtrs(blk,x)
* 27 - Getuint16OutPortPtrs(blk,x)
* 28 - Getuint32OutPortPtrs(blk,x)
*
* 30 - GetNipar(blk)
* 31 - GetIparPtrs(blk)
* 32 - GetNrpar(blk)
* 33 - GetRparPtrs(blk)
*
* 34 - GetWorkPtrs(blk)
* 35 - GetNstate(blk)
* 36 - GetState(blk)
* 37 - GetDerState(blk))
* 38 - GetResState(blk)
* 39 - GetXpropPtrs(blk)
*
* 40 - GetNdstate(blk)
* 41 - GetDstate(blk)
*
* 42 - GetNevIn(blk)
* 43 - GetNevOut(blk)
* 44 - GetNevOutPtrs(blk)
*
* 45 - GetNopar(blk)
* 46 - GetOparType(blk,x)
* 47 - GetOparSize(blk,x,y)
* 48 - GetOparPtrs(blk,x)
* 49 - GetRealOparPtrs(blk,x)
* 50 - GetImagOparPtrs(blk,x)
* 51 - Getint8OparPtrs(blk,x)
* 52 - Getint16OparPtrs(blk,x)
* 53 - Getint32OparPtrs(blk,x)
* 54 - Getuint8OparPtrs(blk,x)
* 55 - Getuint16OparPtrs(blk,x)
* 56 - Getuint32OparPtrs(blk,x)
* 57 - GetNoz(blk)
* 58 - GetOzType(blk,x)
* 59 - GetOzSize(blk,x,y)
* 60 - GetOzPtrs(blk,x)
* 61 - GetRealOzPtrs(blk,x)
* 62 - GetImagOzPtrs(blk,x)
* 63 - Getint8OzPtrs(blk,x)
* 64 - Getint16OzPtrs(blk,x)
* 65 - Getint32OzPtrs(blk,x)
* 66 - Getuint8OzPtrs(blk,x)
* 67 - Getuint16OzPtrs(blk,x)
* 68 - Getuint32OzPtrs(blk,x)
* 69 - GetSizeOfOz(blk,x)
* 70 - GetSizeOfOpar(blk,x)
* 71 - GetSizeOfOut(blk,x)
* 72 - GetSizeOfIn(blk,x)
*
* 73 - GetNg(blk)
* 74 - GetGPtrs(blk)
* 75 - GetJrootPtrs(blk)
* 76 - GetNmode(blk)
* 77 - GetModePtrs(blk)
* 78 - GetLabelPtrs(blk)
*/

/**
\brief Get number of regular input port.
*/
#define GetNin(blk) (blk->nin)

/**
\brief Get regular input port pointer of port number x.
*/
#define GetInPortPtrs(blk,x) ((((x)>0)&((x)<=(blk->nin))) ? (blk->inptr[x-1]) : NULL)

/**
\brief Get number of regular output port.
*/
#define GetNout(blk) (blk->nout)

/**
\brief Get regular output port pointer of port number x.
*/
#define GetOutPortPtrs(blk,x) ((((x)>0)&((x)<=(blk->nout))) ? (blk->outptr[x-1]) : NULL)

/**
\brief Get number of rows (first dimension) of regular input port number x.
*/
#define GetInPortRows(blk,x) ((((x)>0)&((x)<=(blk->nin))) ? (blk->insz[x-1]) : 0)

/**
\brief Get number of columns (second dimension) of regular input port number x.
*/
#define GetInPortCols(blk,x) ((((x)>0)&((x)<=(blk->nin))) ? (blk->insz[blk->nin+(x-1)]) : 0)

/**
\brief Get regular input port size number x.
*/
/*  usage :
*   GetInPortSize(blk,x,1) : get first dimension of input port number x
*   GetInPortSize(blk,x,2) : get second dimension of input port number x
*/
#define GetInPortSize(blk,x,y) ((((x)>0)&((x)<=(blk->nin))) ? \
	((((y)>0)&((y)<=2)) ? (blk->insz[(y-1)*blk->nin+(x-1)]) : 0) : 0)

/**
\brief Get type of regular input port number x.
*/
#define GetInType(blk,x) ((((x)>0)&((x)<=(blk->nin))) ? \
	(blk->insz[2*(blk->nin)+(x-1)]) : 0)

/**
\brief Get number of rows (first dimension) of regular output port number x.
*/
#define GetOutPortRows(blk,x) ((((x)>0)&((x)<=(blk->nout))) ? (blk->outsz[x-1]) : 0)

/**
\brief Get number of columns (second dimension) of regular output port number x.
*/
#define GetOutPortCols(blk,x) ((((x)>0)&((x)<=(blk->nout))) ? (blk->outsz[blk->nout+(x-1)]) : 0)

/**
\brief Get regular output port size number x.
*/
/*  usage :
*   GetOutPortSize(blk,x,1) : get first dimension of output port number x
*   GetOutPortSize(blk,x,2) : get second dimension of output port number x
*/
#define GetOutPortSize(blk,x,y) ((((x)>0)&((x)<=(blk->nout))) ? \
	((((y)>0)&((y)<=2)) ? (blk->outsz[(y-1)*blk->nout+(x-1)]) : 0) : 0)

/**
\brief Get type of regular output port number x.
*/
#define GetOutType(blk,x) ((((x)>0)&((x)<=(blk->nout))) ? \
	(blk->outsz[2*(blk->nout)+(x-1)]) : 0)

/**
\brief Get pointer of real part of regular input port number x.
*/
#define GetRealInPortPtrs(blk,x) (SCSREAL_COP *) GetInPortPtrs(blk,x)

/**
\brief Get pointer of imaginary part of regular input port number x.
*/
#define GetImagInPortPtrs(blk,x) (((x)>0)&((x)<=(blk->nin)) ? \
	(SCSREAL_COP *) ((SCSREAL_COP *)blk->inptr[x-1]+ \
	((blk->insz[(x-1)])*(blk->insz[blk->nin+(x-1)]))) : NULL)

/**
\brief Get pointer of real part of regular output port number x.
*/
#define GetRealOutPortPtrs(blk,x) (SCSREAL_COP *) GetOutPortPtrs(blk,x)

/**
\brief Get pointer of imaginary part of regular output port number x.
*/
#define GetImagOutPortPtrs(blk,x) (((x)>0)&((x)<=(blk->nout)) ? \
	(SCSREAL_COP *) ((SCSREAL_COP *)blk->outptr[x-1]+ \
	((blk->outsz[(x-1)])*(blk->outsz[blk->nout+(x-1)]))) : NULL)

/**
\brief Get pointer of int8 typed regular input port number x.
*/
#define Getint8InPortPtrs(blk,x) (SCSINT8_COP *) GetInPortPtrs(blk,x)

/**
\brief Get pointer of int16 typed regular input port number x.
*/
#define Getint16InPortPtrs(blk,x) (SCSINT16_COP *) GetInPortPtrs(blk,x)

/**
\brief Get pointer of int32 typed regular input port number x.
*/
#define Getint32InPortPtrs(blk,x) (SCSINT32_COP *) GetInPortPtrs(blk,x)

/**
\brief Get pointer of uint8 typed regular input port number x.
*/
#define Getuint8InPortPtrs(blk,x) (SCSUINT8_COP *) GetInPortPtrs(blk,x)

/**
\brief Get pointer of uint16 typed regular input port number x.
*/
#define Getuint16InPortPtrs(blk,x) (SCSUINT16_COP *) GetInPortPtrs(blk,x)

/**
\brief Get pointer of uint32 typed regular input port number x.
*/
#define Getuint32InPortPtrs(blk,x) (SCSUINT32_COP *) GetInPortPtrs(blk,x)

/**
\brief Get pointer of int8 typed regular output port number x.
*/
#define Getint8OutPortPtrs(blk,x) (SCSINT8_COP *) GetOutPortPtrs(blk,x)

/**
\brief Get pointer of int16 typed regular output port number x.
*/
#define Getint16OutPortPtrs(blk,x) (SCSINT16_COP *) GetOutPortPtrs(blk,x)

/**
\brief Get pointer of int32 typed regular output port number x.
*/
#define Getint32OutPortPtrs(blk,x) (SCSINT32_COP *) GetOutPortPtrs(blk,x)

/**
\brief Get pointer of uint8 typed regular output port number x.
*/
#define Getuint8OutPortPtrs(blk,x) (SCSUINT8_COP *) GetOutPortPtrs(blk,x)

/**
\brief Get pointer of uint16 typed regular output port number x.
*/
#define Getuint16OutPortPtrs(blk,x) (SCSUINT16_COP *) GetOutPortPtrs(blk,x)

/**
\brief Get pointer of uint32 typed regular output port number x.
*/
#define Getuint32OutPortPtrs(blk,x) (SCSUINT32_COP *) GetOutPortPtrs(blk,x)

/**
\brief Get number of integer parameters.
*/
#define GetNipar(blk) (blk->nipar)

/**
\brief Get pointer of the integer parameters register
*/
#define GetIparPtrs(blk) (blk->ipar)

/**
\brief Get number of real parameters.
*/
#define GetNrpar(blk) (blk->nrpar)

/**
\brief Get pointer of the real parameters register.
*/
#define GetRparPtrs(blk) (blk->rpar)

/**
\brief Get the pointer of the Work array.
*/
#define GetWorkPtrs(blk) (*(blk->work))

/**
\brief Get number of continuous state.
*/
#define GetNstate(blk) (blk->nx)

/**
\brief Get pointer of the continuous state register.
*/
#define GetState(blk) (blk->x)

/**
\brief Get pointer of the derivative continuous state register.
*/
#define GetDerState(blk) (blk->xd)

/**
\brief Get pointer of the residual continuous state register.
*/
#define GetResState(blk) (blk->res)

/**
\brief Get pointer of continuous state properties register.
*/
#define GetXpropPtrs(blk) (blk->xprop)

/**
\brief Get number of discrete state.
*/
#define GetNdstate(blk) (blk->nz)

/**
\brief Get pointer of the discrete state register.
*/
#define GetDstate(blk) (blk->z)

/**
\brief Get the input event number.
*/
#define GetNevIn(blk) (blk->nevprt)

/**
\brief Get number of event output port.
*/
#define GetNevOut(blk) (blk->nevout)

/**
\brief Get pointer of event output register.
*/
#define GetNevOutPtrs(blk) (blk->evout)

/**
\brief Get number of object parameters.
*/
#define GetNopar(blk) (blk->nopar)

/**
\brief Get type of object parameters number x.
*/
#define GetOparType(blk,x) (((x>0)&(x<=blk->nopar)) ? (blk->opartyp[x-1]) : 0)

/**
\brief Get size of object parameters number x.

*/
/*  usage :
*   GetOparSize(blk,x,1) : get first dimension of opar
*   GetOparSize(blk,x,2) : get second dimension of opar
*/
#define GetOparSize(blk,x,y) (((x>0)&(x<=blk->nopar)) ? \
	((((y)>0)&((y)<=2)) ? (blk->oparsz[(y-1)*blk->nopar+(x-1)]) : 0) : 0)

/**
\brief Get pointer of object parameters number x.
*/
#define GetOparPtrs(blk,x) (((x>0)&(x<=blk->nopar)) ? (blk->oparptr[x-1]) : 0)

/**
\brief Get pointer of real object parameters number x.
*/
#define GetRealOparPtrs(blk,x) (SCSREAL_COP *) GetOparPtrs(blk,x)

/**
\brief Get pointer of imaginary part of object parameters number x.
*/
#define GetImagOparPtrs(blk,x) (((x)>0)&((x)<=(blk->nopar)) ? \
	(SCSREAL_COP *) ((SCSREAL_COP *)blk->oparptr[x-1]+ \
	((blk->oparsz[x-1])*(blk->oparsz[blk->nopar+(x-1)]))) : NULL)

/**
\brief Get pointer of int8 typed object parameters number x.
*/
#define Getint8OparPtrs(blk,x) (SCSINT8_COP *) GetOparPtrs(blk,x)

/**
\brief Get pointer of int16 typed object parameters number x.
*/
#define Getint16OparPtrs(blk,x) (SCSINT16_COP *) GetOparPtrs(blk,x)

/**
\brief Get pointer of int32 typed object parameters number x.
*/
#define Getint32OparPtrs(blk,x) (SCSINT32_COP *) GetOparPtrs(blk,x)

/**
\brief Get pointer of uint8 typed object parameters number x.
*/
#define Getuint8OparPtrs(blk,x) (SCSUINT8_COP *) GetOparPtrs(blk,x)

/**
\brief Get pointer of uint16 typed object parameters number x.
*/
#define Getuint16OparPtrs(blk,x) (SCSUINT16_COP *) GetOparPtrs(blk,x)

/**
\brief Get pointer of uint32 typed object parameters number x.
*/
#define Getuint32OparPtrs(blk,x) (SCSUINT32_COP *) GetOparPtrs(blk,x)

/**
\brief Get number of object state.
*/
#define GetNoz(blk) (blk->noz)

/**
\brief Get type of object state number x.
*/
#define GetOzType(blk,x) (((x>0)&(x<=blk->noz)) ? (blk->oztyp[x-1]) : 0)

/**
\brief Get size of object state number x.

*/
/*  usage :
*   GetOzSize(blk,x,1) : get first dimension of oz
*   GetOzSize(blk,x,2) : get second dimension of oz
*/
#define GetOzSize(blk,x,y) (((x>0)&(x<=blk->noz)) ? \
	((((y)>0)&((y)<=2)) ? (blk->ozsz[(y-1)*blk->noz+(x-1)]) : 0) : 0)

/**
\brief Get pointer of object state number x.
*/
#define GetOzPtrs(blk,x) (((x>0)&(x<=blk->noz)) ? (blk->ozptr[x-1]) : 0)

/**
\brief Get pointer of real object state number x.
*/
#define GetRealOzPtrs(blk,x) (SCSREAL_COP *) GetOzPtrs(blk,x)

/**
\brief Get pointer of imaginary part of object state number x.
*/
#define GetImagOzPtrs(blk,x) (((x)>0)&((x)<=(blk->noz)) ? \
	(SCSREAL_COP *) ((SCSREAL_COP *)blk->ozptr[x-1]+ \
	((blk->ozsz[x-1])*(blk->ozsz[blk->noz+(x-1)]))) : NULL)

/**
\brief Get pointer of int8 typed object state number x.
*/
#define Getint8OzPtrs(blk,x) (SCSINT8_COP *) GetOzPtrs(blk,x)

/**
\brief Get pointer of int16 typed object state number x.
*/
#define Getint16OzPtrs(blk,x) (SCSINT16_COP *) GetOzPtrs(blk,x)

/**
\brief Get pointer of int32 typed object state number x.
*/
#define Getint32OzPtrs(blk,x) (SCSINT32_COP *) GetOzPtrs(blk,x)

/**
\brief Get pointer of uint8 typed object state number x.
*/
#define Getuint8OzPtrs(blk,x) (SCSUINT8_COP *) GetOzPtrs(blk,x)

/**
\brief Get pointer of uint16 typed object state number x.
*/
#define Getuint16OzPtrs(blk,x) (SCSUINT16_COP *) GetOzPtrs(blk,x)

/**
\brief Get pointer of uint32 typed object state number x.
*/
#define Getuint32OzPtrs(blk,x) (SCSUINT32_COP *) GetOzPtrs(blk,x)

/**
\brief Get the sizeof of the object state number x.
*/
#define GetSizeOfOz(blk,x)    ((GetOzType(blk,x)==SCSREAL_N) ? (sizeof(SCSREAL_COP)) : \
	(GetOzType(blk,x)==SCSCOMPLEX_N) ? (2*sizeof(SCSCOMPLEX_COP)) : \
	((GetOzType(blk,x)==SCSINT8_N)|(GetOzType(blk,x)==SCSUINT8_N)) ? (sizeof(SCSINT8_COP)) : \
	((GetOzType(blk,x)==SCSINT16_N)|(GetOzType(blk,x)==SCSUINT16_N)) ? (sizeof(SCSINT16_COP)) : \
	((GetOzType(blk,x)==SCSINT32_N)|(GetOzType(blk,x)==SCSUINT32_N)) ? (sizeof(SCSINT32_COP)) : 0)

/**
\brief Get the sizeof of the object parameters number x.
*/
#define GetSizeOfOpar(blk,x)  ((GetOparType(blk,x)==SCSREAL_N) ? (sizeof(SCSREAL_COP)) : \
	(GetOparType(blk,x)==SCSCOMPLEX_N) ? (2*sizeof(SCSCOMPLEX_COP)) : \
	((GetOparType(blk,x)==SCSINT8_N)|(GetOparType(blk,x)==SCSUINT8_N)) ? (sizeof(SCSINT8_COP)) : \
	((GetOparType(blk,x)==SCSINT16_N)|(GetOparType(blk,x)==SCSUINT16_N)) ? (sizeof(SCSINT16_COP)) : \
	((GetOparType(blk,x)==SCSINT32_N)|(GetOparType(blk,x)==SCSUINT32_N)) ? (sizeof(SCSINT32_COP)) : 0)

/**
\brief Get the sizeof of the regular output port number x.
*/
#define GetSizeOfOut(blk,x)  ((GetOutType(blk,x)==SCSREAL_N) ? (sizeof(SCSREAL_COP)) : \
	(GetOutType(blk,x)==SCSCOMPLEX_N) ? (2*sizeof(SCSCOMPLEX_COP)) : \
	((GetOutType(blk,x)==SCSINT8_N)|(GetOutType(blk,x)==SCSUINT8_N)) ? (sizeof(SCSINT8_COP)) : \
	((GetOutType(blk,x)==SCSINT16_N)|(GetOutType(blk,x)==SCSUINT16_N)) ? (sizeof(SCSINT16_COP)) : \
	((GetOutType(blk,x)==SCSINT32_N)|(GetOutType(blk,x)==SCSUINT32_N)) ? (sizeof(SCSINT32_COP)) : 0)
/**
\brief Get the sizeof of the regular input port number x.
*/
#define GetSizeOfIn(blk,x)  ((GetInType(blk,x)==SCSREAL_N) ? (sizeof(SCSREAL_COP)) : \
	(GetInType(blk,x)==SCSCOMPLEX_N) ? (2*sizeof(SCSCOMPLEX_COP)) : \
	((GetInType(blk,x)==SCSINT8_N)|(GetInType(blk,x)==SCSUINT8_N)) ? (sizeof(SCSINT8_COP)) : \
	((GetInType(blk,x)==SCSINT16_N)|(GetInType(blk,x)==SCSUINT16_N)) ? (sizeof(SCSINT16_COP)) : \
	((GetInType(blk,x)==SCSINT32_N)|(GetInType(blk,x)==SCSUINT32_N)) ? (sizeof(SCSINT32_COP)) : 0)

/**
\brief Get number of zero crossing surface.
*/
#define GetNg(blk) (blk->ng)

/**
\brief Get pointer of the zero crossing register.
*/
#define GetGPtrs(blk) (blk->g)

/**
\brief Get pointer of the direction of the zero crossing register.
*/
#define GetJrootPtrs(blk) (blk->jroot)

/**
\brief Get number of modes.
*/
#define GetNmode(blk) (blk->nmode)

/**
\brief Get pointer of the mode register.
*/
#define GetModePtrs(blk) (blk->mode)

/**
\brief Get pointer of the block label
*/
#define GetLabelPtrs(blk) (blk->label)

#endif /* __SCICOS_BLOCK_H__ */
