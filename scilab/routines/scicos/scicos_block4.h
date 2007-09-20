#ifndef __SCICOS_BLOCK_H__
#define __SCICOS_BLOCK_H__

#ifdef __STDC__
	#include <stdlib.h>
#else
	#ifndef WIN32
		#include <malloc.h>
	#endif
#endif

#ifndef NULL
	#define NULL 0
#endif

/* define scicos flag number */
/**
   \brief DerivativeState is flag 0
*/
#define DerivativeState 0

/**
   \brief OutputUpdate is flag 1
*/
#define OutputUpdate 1

/**
   \brief StateUpdate is flag 2
*/
#define StateUpdate 2

/**
   \brief OutputEventTiming is flag 3
*/
#define OutputEventTiming 3

/**
   \brief Initialization is flag 4
*/
#define Initialization 4

/**
   \brief Ending is flag 5
*/
#define Ending 5

/**
   \brief ReInitialization is flag 6
*/
#define ReInitialization 6

/* define voidg type */
typedef void (*voidg)();

/* scicos_block structure definition */
typedef struct {
  int nevprt;
  voidg funpt ;
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

/* utility function for block declaration */
void do_cold_restart();
int get_phase_simulation();
double get_scicos_time();
int get_block_number();
void set_block_error(int);
void set_pointer_xproperty(int* pointer);
void * scicos_malloc(size_t);
void scicos_free(void *p);
double Get_Jacobian_parameter(void);
double Get_Scicos_SQUR(void);
void Set_Jacobian_flag(int flag);

/* define min max for win32 */
#ifndef WIN32
	#define max(a,b) ((a) >= (b) ? (a) : (b))
	#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif

extern int s_copy();
extern int s_cmp();

/* Define scicos simulator data type number (_N) */
#define SCSREAL_N 10
#define SCSCOMPLEX_N 11
#define SCSINT_N 80
#define SCSINT8_N 81
#define SCSINT16_N 82
#define SCSINT32_N 84
#define SCSUINT_N 800
#define SCSUINT8_N 811
#define SCSUINT16_N 812
#define SCSUINT32_N 814
#define SCSUNKNOW_N -1

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
  * 29 - free
  *
  * 30 - GetNipar(blk)
  * 31 - GetIparPtrs(blk)
  * 32 - GetNrpar(blk)
  * 33 - GetRparPtrs(blk)
  *
  * 34 - GetWorkPtrs(blk)
  * 35 - GetNdstate(blk)
  * 36 - GetDstate(blk)
  *
  * 37 - GetNev(blk)

  * 38 - GetNopar(blk)
  * 39 - GetOparType(blk,x)
  * 40 - GetOparSize(blk,x,y)
  * 41 - GetOparPtrs(blk,x)
  * 42 - GetRealOparPtrs(blk,x)
  * 43 - GetImagOparPtrs(blk,x)
  * 44 - Getint8OparPtrs(blk,x)
  * 45 - Getint16OparPtrs(blk,x)
  * 46 - Getint32OparPtrs(blk,x)
  * 47 - Getuint8OparPtrs(blk,x)
  * 48 - Getuint16OparPtrs(blk,x)
  * 49 - Getuint32OparPtrs(blk,x)
  * 50 - GetNoz(blk)
  * 51 - GetOzType(blk,x)
  * 52 - GetOzSize(blk,x,y)
  * 53 - GetOzPtrs(blk,x)
  * 54 - GetRealOzPtrs(blk,x)
  * 55 - GetImagOzPtrs(blk,x)
  * 56 - Getint8OzPtrs(blk,x)
  * 57 - Getint16OzPtrs(blk,x)
  * 58 - Getint32OzPtrs(blk,x)
  * 59 - Getuint8OzPtrs(blk,x)
  * 60 - Getuint16OzPtrs(blk,x)
  * 61 - Getuint32OzPtrs(blk,x)
  * 62 - GetSizeOfOz(blk,x)
  * 63 - GetSizeOfOpar(blk,x)
  * 64 - GetSizeOfOut(blk,x)
  * 65 - GetSizeOfIn(blk,x)

  */

/**
   \brief Get Number of Regular Input Port
*/
#define GetNin(blk) (blk->nin)

/**
   \brief Get Regular Input Port Pointer of port number x
*/
#define GetInPortPtrs(blk,x) ((((x)>0)&((x)<=(blk->nin))) ? (blk->inptr[x-1]) : NULL)

/**
   \brief Get Number of Regular Output Port
*/
#define GetNout(blk) (blk->nout)

/**
   \brief Get Regular Output Port Pointer of port number x
*/
#define GetOutPortPtrs(blk,x) ((((x)>0)&((x)<=(blk->nout))) ? (blk->outptr[x-1]) : NULL)

/**
   \brief Get number of Rows (first dimension) of Regular Input Port number x
*/
#define GetInPortRows(blk,x) ((((x)>0)&((x)<=(blk->nin))) ? (blk->insz[x-1]) : 0)

/**
   \brief Get number of Columns (second dimension) of  Regular Input Port number x
*/
#define GetInPortCols(blk,x) ((((x)>0)&((x)<=(blk->nin))) ? (blk->insz[blk->nin+(x-1)]) : 0)

/**
   \brief Get Regular Input Port Size
*/
/*  usage :
 *   GetInPortSize(blk,x,1) : get first dimension of input port number x
 *   GetInPortSize(blk,x,2) : get second dimension of input port number x
 */
#define GetInPortSize(blk,x,y) ((((x)>0)&((x)<=(blk->nin))) ? \
              ((((y)>0)&((y)<=2)) ? (blk->insz[(y-1)*blk->nin+(x-1)]) : 0) : 0)

/**
   \brief Get Type of Input Port
*/
#define GetInType(blk,x) ((((x)>0)&((x)<=(blk->nin))) ? \
              (blk->insz[2*(blk->nin)+(x-1)]) : 0)

/**
   \brief Get number of Rows (first dimension) of Regular Output Port number x
*/
#define GetOutPortRows(blk,x) ((((x)>0)&((x)<=(blk->nout))) ? (blk->outsz[x-1]) : 0)

/**
   \brief Get number of Columns (second dimension) of Regular Output Port number x
*/
#define GetOutPortCols(blk,x) ((((x)>0)&((x)<=(blk->nout))) ? (blk->outsz[blk->nout+(x-1)]) : 0)

/**
   \brief Get Regular Output Port Size
*/
/*  usage :
 *   GetOutPortSize(blk,x,1) : get first dimension of output port number x
 *   GetOutPortSize(blk,x,2) : get second dimension of output port number x
 */
#define GetOutPortSize(blk,x,y) ((((x)>0)&((x)<=(blk->nout))) ? \
              ((((y)>0)&((y)<=2)) ? (blk->outsz[(y-1)*blk->nout+(x-1)]) : 0) : 0)

/**
   \brief Get Type of Output Port
*/
#define GetOutType(blk,x) ((((x)>0)&((x)<=(blk->nout))) ? \
              (blk->outsz[2*(blk->nout)+(x-1)]) : 0)

/**
   \brief Get Pointer of Real Part of Regular Input Port
*/
#define GetRealInPortPtrs(blk,x) (SCSREAL_COP *) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of Imaginary Part of Regular Input Port
*/
#define GetImagInPortPtrs(blk,x) (((x)>0)&((x)<=(blk->nin)) ? \
              (SCSREAL_COP *) ((SCSREAL_COP *)blk->inptr[x-1]+ \
               ((blk->insz[(x-1)])*(blk->insz[blk->nin+(x-1)]))) : NULL)

/**
   \brief Get Pointer of Real Part of Regular Output Port
*/
#define GetRealOutPortPtrs(blk,x) (SCSREAL_COP *) GetOutPortPtrs(blk,x)

/**
   \brief Get Pointer of Imaginary Part of Regular Output Port
*/
#define GetImagOutPortPtrs(blk,x) (((x)>0)&((x)<=(blk->nout)) ? \
              (SCSREAL_COP *) ((SCSREAL_COP *)blk->outptr[x-1]+ \
               ((blk->outsz[(x-1)])*(blk->outsz[blk->nout+(x-1)]))) : NULL)

/**
   \brief Get Pointer of int8 typed Regular Input Port
*/
#define Getint8InPortPtrs(blk,x) (SCSINT8_COP *) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of int16 typed Regular Input Port
*/
#define Getint16InPortPtrs(blk,x) (SCSINT16_COP *) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of int32 typed Regular Input Port
*/
#define Getint32InPortPtrs(blk,x) (SCSINT32_COP *) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of uint8 typed Regular Input Port
*/
#define Getuint8InPortPtrs(blk,x) (SCSUINT8_COP *) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of uint16 typed Regular Input Port
*/
#define Getuint16InPortPtrs(blk,x) (SCSUINT16_COP *) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of uint32 typed Regular Input Port
*/
#define Getuint32InPortPtrs(blk,x) (SCSUINT32_COP *) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of int8 typed Regular Output Port
*/
#define Getint8OutPortPtrs(blk,x) (SCSINT8_COP *) GetOutPortPtrs(blk,x)

/**
   \brief Get Pointer of int16 typed Regular Output Port
*/
#define Getint16OutPortPtrs(blk,x) (SCSINT16_COP *) GetOutPortPtrs(blk,x)

/**
   \brief Get Pointer of int32 typed Regular Output Port
*/
#define Getint32OutPortPtrs(blk,x) (SCSINT32_COP *) GetOutPortPtrs(blk,x)

/**
   \brief Get Pointer of uint8 typed Regular Output Port
*/
#define Getuint8OutPortPtrs(blk,x) (SCSUINT8_COP *) GetOutPortPtrs(blk,x)

/**
   \brief Get Pointer of uint16 typed Regular Output Port
*/
#define Getuint16OutPortPtrs(blk,x) (SCSUINT16_COP *) GetOutPortPtrs(blk,x)

/**
   \brief Get Pointer of uint32 typed Regular Output Port
*/
#define Getuint32OutPortPtrs(blk,x) (SCSUINT32_COP *) GetOutPortPtrs(blk,x)

/**
   \brief Get Number of Integer Parameters
*/
#define GetNipar(blk) (blk->nipar)

/**
   \brief Get Pointer of Integer Parameters
*/
#define GetIparPtrs(blk) (blk->ipar)

/**
   \brief Get Number Real Parameters
*/
#define GetNrpar(blk) (blk->nrpar)

/**
   \brief Get Pointer of Real Parameters
*/
#define GetRparPtrs(blk) (blk->rpar)

/**
   \brief Get Pointer of Work
*/
#define GetWorkPtrs(blk) (*(blk->work))

/**
   \brief Get Number of Discrete state
*/
#define GetNdstate(blk) (blk->nz)

/**
   \brief Get Pointer of Discrete state
*/
#define GetDstate(blk) (blk->z)

/**
   \brief Get Nevprt of the block
*/
#define GetNev(blk) (blk->nevprt)

/**
   \brief Get Number of Object Parameters
*/
#define GetNopar(blk) (blk->nopar)

/**
   \brief Get Type of Object Parameters
*/
#define GetOparType(blk,x) (((x>0)&(x<=blk->nopar)) ? (blk->opartyp[x-1]) : 0)

/**
   \brief Get Size of Object Parameters

*/
/*  usage :
 *   GetOparSize(blk,x,1) : get first dimension of opar
 *   GetOparSize(blk,x,2) : get second dimension of opar
 */
#define GetOparSize(blk,x,y) (((x>0)&(x<=blk->nopar)) ? \
                              ((((y)>0)&((y)<=2)) ? (blk->oparsz[(y-1)*blk->nopar+(x-1)]) : 0) : 0)

/**
   \brief Get Pointer of Object Parameters
*/
#define GetOparPtrs(blk,x) (((x>0)&(x<=blk->nopar)) ? (blk->oparptr[x-1]) : 0)

/**
   \brief Get Pointer of Real Object Parameters
*/
#define GetRealOparPtrs(blk,x) (SCSREAL_COP *) GetOparPtrs(blk,x)

/**
   \brief Get Pointer of Imaginary Part of Object Parameters
*/
#define GetImagOparPtrs(blk,x) (((x)>0)&((x)<=(blk->nopar)) ? \
                               (SCSREAL_COP *) ((SCSREAL_COP *)blk->oparptr[x-1]+ \
                               ((blk->oparsz[x-1])*(blk->oparsz[blk->nopar+(x-1)]))) : NULL)

/**
   \brief Get Pointer of int8 typed Object Parameters
*/
#define Getint8OparPtrs(blk,x) (SCSINT8_COP *) GetOparPtrs(blk,x)

/**
   \brief Get Pointer of int16 typed Object Parameters
*/
#define Getint16OparPtrs(blk,x) (SCSINT16_COP *) GetOparPtrs(blk,x)

/**
   \brief Get Pointer of int32 typed Object Parameters
*/
#define Getint32OparPtrs(blk,x) (SCSINT32_COP *) GetOparPtrs(blk,x)

/**
   \brief Get Pointer of uint8 typed Object Parameters
*/
#define Getuint8OparPtrs(blk,x) (SCSUINT8_COP *) GetOparPtrs(blk,x)

/**
   \brief Get Pointer of uint16 typed Object Parameters
*/
#define Getuint16OparPtrs(blk,x) (SCSUINT16_COP *) GetOparPtrs(blk,x)

/**
   \brief Get Pointer of uint32 typed Object Parameters
*/
#define Getuint32OparPtrs(blk,x) (SCSUINT32_COP *) GetOparPtrs(blk,x)

/**
   \brief Get Number of Object State
*/
#define GetNoz(blk) (blk->noz)

/**
   \brief Get Type of Object State
*/
#define GetOzType(blk,x) (((x>0)&(x<=blk->noz)) ? (blk->oztyp[x-1]) : 0)

/**
   \brief Get Size of Object State

*/
/*  usage :
 *   GetOzSize(blk,x,1) : get first dimension of oz
 *   GetOzSize(blk,x,2) : get second dimension of oz
 */
#define GetOzSize(blk,x,y) (((x>0)&(x<=blk->noz)) ? \
                              ((((y)>0)&((y)<=2)) ? (blk->ozsz[(y-1)*blk->noz+(x-1)]) : 0) : 0)

/**
   \brief Get Pointer of Object State
*/
#define GetOzPtrs(blk,x) (((x>0)&(x<=blk->noz)) ? (blk->ozptr[x-1]) : 0)

/**
   \brief Get Pointer of Real Object State
*/
#define GetRealOzPtrs(blk,x) (SCSREAL_COP *) GetOzPtrs(blk,x)

/**
   \brief Get Pointer of Imaginary Part of Object State
*/
#define GetImagOzPtrs(blk,x) (((x)>0)&((x)<=(blk->noz)) ? \
                               (SCSREAL_COP *) ((SCSREAL_COP *)blk->ozptr[x-1]+ \
                               ((blk->ozsz[x-1])*(blk->ozsz[blk->noz+(x-1)]))) : NULL)

/**
   \brief Get Pointer of int8 typed Object State
*/
#define Getint8OzPtrs(blk,x) (SCSINT8_COP *) GetOzPtrs(blk,x)

/**
   \brief Get Pointer of int16 typed Object State
*/
#define Getint16OzPtrs(blk,x) (SCSINT16_COP *) GetOzPtrs(blk,x)

/**
   \brief Get Pointer of int32 typed Object State
*/
#define Getint32OzPtrs(blk,x) (SCSINT32_COP *) GetOzPtrs(blk,x)

/**
   \brief Get Pointer of uint8 typed Object State
*/
#define Getuint8OzPtrs(blk,x) (SCSUINT8_COP *) GetOzPtrs(blk,x)

/**
   \brief Get Pointer of uint16 typed Object State
*/
#define Getuint16OzPtrs(blk,x) (SCSUINT16_COP *) GetOzPtrs(blk,x)

/**
   \brief Get Pointer of uint32 typed Object State
*/
#define Getuint32OzPtrs(blk,x) (SCSUINT32_COP *) GetOzPtrs(blk,x)

/**
   \brief Get The sizeof of the Object State
*/
#define GetSizeOfOz(blk,x)    ((GetOzType(blk,x)==SCSREAL_N) ? (sizeof(SCSREAL_COP)) : \
                               (GetOzType(blk,x)==SCSCOMPLEX_N) ? (2*sizeof(SCSCOMPLEX_COP)) : \
			       ((GetOzType(blk,x)==SCSINT8_N)|(GetOzType(blk,x)==SCSUINT8_N)) ? (sizeof(SCSINT8_COP)) : \
                               ((GetOzType(blk,x)==SCSINT16_N)|(GetOzType(blk,x)==SCSUINT16_N)) ? (sizeof(SCSINT16_COP)) : \
                               ((GetOzType(blk,x)==SCSINT32_N)|(GetOzType(blk,x)==SCSUINT32_N)) ? (sizeof(SCSINT32_COP)) : 0)

/**
   \brief Get The sizeof of the Object Parameters
*/
#define GetSizeOfOpar(blk,x)  ((GetOparType(blk,x)==SCSREAL_N) ? (sizeof(SCSREAL_COP)) : \
                               (GetOparType(blk,x)==SCSCOMPLEX_N) ? (2*sizeof(SCSCOMPLEX_COP)) : \
			       ((GetOparType(blk,x)==SCSINT8_N)|(GetOparType(blk,x)==SCSUINT8_N)) ? (sizeof(SCSINT8_COP)) : \
                               ((GetOparType(blk,x)==SCSINT16_N)|(GetOparType(blk,x)==SCSUINT16_N)) ? (sizeof(SCSINT16_COP)) : \
                               ((GetOparType(blk,x)==SCSINT32_N)|(GetOparType(blk,x)==SCSUINT32_N)) ? (sizeof(SCSINT32_COP)) : 0)

/**
   \brief Get The sizeof of the Output 
*/
#define GetSizeOfOut(blk,x)  ((GetOutType(blk,x)==SCSREAL_N) ? (sizeof(SCSREAL_COP)) : \
                              (GetOutType(blk,x)==SCSCOMPLEX_N) ? (2*sizeof(SCSCOMPLEX_COP)) : \
			      ((GetOutType(blk,x)==SCSINT8_N)|(GetOutType(blk,x)==SCSUINT8_N)) ? (sizeof(SCSINT8_COP)) : \
                              ((GetOutType(blk,x)==SCSINT16_N)|(GetOutType(blk,x)==SCSUINT16_N)) ? (sizeof(SCSINT16_COP)) : \
                              ((GetOutType(blk,x)==SCSINT32_N)|(GetOutType(blk,x)==SCSUINT32_N)) ? (sizeof(SCSINT32_COP)) : 0)
/**
   \brief Get The sizeof of the Input 
*/
#define GetSizeOfIn(blk,x)  ((GetInType(blk,x)==SCSREAL_N) ? (sizeof(SCSREAL_COP)) : \
                             (GetInType(blk,x)==SCSCOMPLEX_N) ? (2*sizeof(SCSCOMPLEX_COP)) : \
                             ((GetInType(blk,x)==SCSINT8_N)|(GetInType(blk,x)==SCSUINT8_N)) ? (sizeof(SCSINT8_COP)) : \
                             ((GetInType(blk,x)==SCSINT16_N)|(GetInType(blk,x)==SCSUINT16_N)) ? (sizeof(SCSINT16_COP)) : \
                             ((GetInType(blk,x)==SCSINT32_N)|(GetInType(blk,x)==SCSUINT32_N)) ? (sizeof(SCSINT32_COP)) : 0)


#endif /* __SCICOS_BLOCK_H__ */
