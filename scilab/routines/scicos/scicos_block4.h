#ifndef __SCICOS_BLOCK_H__
#define __SCICOS_BLOCK_H__

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

typedef void (*voidg)();

/* scicos_block structure definition */
typedef struct {
  int nevprt;
  voidg funpt ;
  int type;
  int scsptr;
  int nz;
  double *z;
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
void * scicos_malloc(size_t );
void scicos_free(void *p);
double Get_Jacobian_parameter(void);
double Get_Scicos_SQUR(void);
void Set_Jacobian_flag(int flag);

#if WIN32
#undef min
#undef max
#endif

#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))

extern int s_copy();
extern int s_cmp();

 /* scicos_block macros definition :
  *
  * 1  - GetNin(blk)
  * 2  - GetInPortPtrs(blk,x)
  * 3  - GetNout(blk)
  * 4  - GetOutPortPtrs(blk,x)
  * 5  - GetInPortRows(blk,x)
  * 6  - GetInPortCols(blk,x)
  * 7  - GetInPortSize(blk,x,y)
  * 8  - GetOutPortRows(blk,x)
  * 9  - GetOutPortCols(blk,x)
  * 10 - GetOutPortSize(blk,x,y)
  * 11 - GetRealInPortPtrs(blk,x)
  * 12 - GetImagInPortPtrs(blk,x)
  * 13 - GetRealOutPortPtrs(blk,x)
  * 14 - GetImagOutPortPtrs(blk,x)
  * 15 - Getint8InPortPtrs(blk,x)
  * 16 - Getint16InPortPtrs(blk,x)
  * 17 - Getint32InPortPtrs(blk,x)
  * 18 - Getuint8InPortPtrs(blk,x)
  * 19 - Getuint16InPortPtrs(blk,x)
  * 20 - Getuint32InPortPtrs(blk,x)
  * 21 - Getint8OutPortPtrs(blk,x)
  * 22 - Getint16OutPortPtrs(blk,x)
  * 23 - Getint32OutPortPtrs(blk,x)
  * 24 - Getuint8OutPortPtrs(blk,x)
  * 25 - Getuint16OutPortPtrs(blk,x)
  * 26 - Getuint32OutPortPtrs(blk,x)
  *
  */

/* Define integer type and integer pointers */
#define int8  char
#define int16 short
#define int32 long
#define uint8  unsigned char
#define uint16 unsigned short
#define uint32 unsigned long
#define int8Ptrs  char *
#define int16Ptrs short *
#define int32Ptrs long *
#define uint8Ptrs  unsigned char *
#define uint16Ptrs unsigned short *
#define uint32Ptrs unsigned long *

/* Get Number of Regular Input Port */
#define GetNin(blk) (blk->nin)

/* Get Regular Input Port Pointer of port number x */
#define GetInPortPtrs(blk,x) ((((x)>=0)&((x)<(blk->nin)) ? (blk->inptr[x]) : NULL)

/* Get Number of Regular Output Port */
#define GetNout(blk) (blk->nout)

/* Get Regular Output Port Pointer of port number x */
#define GetOutPortPtrs(blk,x) ((((x)>=0)&((x)<(blk->nout))) ? (blk->outptr[x]) : NULL)

/* Get number of Rows (first dimension) of Regular Input Port number x */
#define GetInPortRows(blk,x) ((((x)>=0)&((x)<(blk->nin))) ? (blk->insz[x]) : NULL)

/* Get number of Columns (second dimension) of  Regular Input Port number x */
#define GetInPortCols(blk,x) ((((x)>=0)&((x)<(blk->nin))) ? (blk->insz[blk->nin+(x)]) : NULL)

/* Get Regular Input Port Size 
 *  usage :
 *   GetInPortSize(blk,x,1) : get first dimension of input port number x
 *   GetInPortSize(blk,x,2) : get second dimension of input port number x
 */
#define GetInPortSize(blk,x,y) ((((x)>=0)&((x)<(blk->nin))) ? \
              ((((y)>0)&((y)<=2)) ? (blk->insz[(y-1)*blk->nin+(x)]) : 0) : 0)

/* Get number of Rows (first dimension) of Regular Output Port number x */
#define GetOutPortRows(blk,x) ((((x)>=0)&((x)<(blk->nout))) ? (blk->outsz[x]) : 0)

/* Get number of Columns (second dimension) of Regular Output Port number x */
#define GetOutPortCols(blk,x) ((((x)>=0)&((x)<(blk->nout))) ? (blk->outsz[blk->nout+(x)]) : 0)

/* Get Regular Output Port Size 
 *  usage :
 *   GetOutPortSize(blk,x,1) : get first dimension of output port number x
 *   GetOutPortSize(blk,x,2) : get second dimension of output port number x
 */
#define GetOutPortSize(blk,x,y) ((((x)>=0)&((x)<(blk->nout))) ? \
              ((((y)>0)&((y)<=2)) ? (blk->outsz[(y-1)*blk->nout+(x)]) : 0) : 0)

/* Get Pointer of Real Part of Regular Input Port */
#define GetRealInPortPtrs(blk,x) (double *) GetInPortPtrs(blk,x)

/* Get Pointer of Imaginary Part of Regular Input Port */
#define GetImagInPortPtrs(blk,x) (((x)>=0)&((x)<(blk->nin)) ? \
              (double *) ((double *)blk->inptr[x]+((blk->insz[(x)])*(blk->insz[blk->nin+(x)]))) : NULL)

/* Get Pointer of Real Part of Regular Output Port */
#define GetRealOutPortPtrs(blk,x) (double *) GetOutPortPtrs(blk,x)

/* Get Pointer of Imaginary Part of Regular Output Port */
#define GetImagOutPortPtrs(blk,x) (((x)>=0)&((x)<(blk->nout)) ? \
              (double *) ((double *)blk->outptr[x]+((blk->outsz[(x)])*(blk->outsz[blk->nout+(x)]))) : NULL)

/* Get Pointer of int8 typed Regular Input Port */
#define Getint8InPortPtrs(blk,x) (int8Ptrs) GetInPortPtrs(blk,x)

/* Get Pointer of int16 typed Regular Input Port */
#define Getint16InPortPtrs(blk,x) (int16Ptrs) GetInPortPtrs(blk,x)

/* Get Pointer of int32 typed Regular Input Port */
#define Getint32InPortPtrs(blk,x) (int32Ptrs) GetInPortPtrs(blk,x)

/* Get Pointer of uint8 typed Regular Input Port */
#define Getuint8InPortPtrs(blk,x) (uint8Ptrs) GetInPortPtrs(blk,x)

/* Get Pointer of uint16 typed Regular Input Port */
#define Getuint16InPortPtrs(blk,x) (uint16Ptrs) GetInPortPtrs(blk,x)

/* Get Pointer of uint32 typed Regular Input Port */
#define Getuint32InPortPtrs(blk,x) (uint32Ptrs) GetInPortPtrs(blk,x)

/* Get Pointer of int8 typed Regular Output Port */
#define Getint8OutPortPtrs(blk,x) (int8Ptrs) GetOutPortPtrs(blk,x)

/* Get Pointer of int16 typed Regular Output Port */
#define Getint16OutPortPtrs(blk,x) (int16Ptrs) GetOutPortPtrs(blk,x)

/* Get Pointer of int32 typed Regular Output Port */
#define Getint32OutPortPtrs(blk,x) (int32Ptrs) GetOutPortPtrs(blk,x)

/* Get Pointer of uint8 typed Regular Output Port */
#define Getuint8OutPortPtrs(blk,x) (uint8Ptrs) GetOutPortPtrs(blk,x)

/* Get Pointer of uint16 typed Regular Output Port */
#define Getuint16OutPortPtrs(blk,x) (uint16Ptrs) GetOutPortPtrs(blk,x)

/* Get Pointer of uint32 typed Regular Output Port */
#define Getuint32OutPortPtrs(blk,x) (uint32Ptrs) GetOutPortPtrs(blk,x)

#endif /* __SCICOS_BLOCK_H__ */
