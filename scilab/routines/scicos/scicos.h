#ifndef __SCICOS_H 
#define __SCICOS_H 

#include "scicos_block.h"
/* maximum value for sum of number of inputs and outputs ports of a given 
block of type 2 */
#define SZ_SIZE 60
/* maximum value for sum of number of inputs and outputs of a given block 
of type 0 */
#define TB_SIZE 500

/* common for ? */

extern struct {
  int ptr;
} C2F(scsptr);

/* scicos common for debug */

extern struct {
    int idb;
} C2F(dbcos);

typedef void (*voidf)();

/* Blocks prototypes */
#define I int
#define IP int*
#define DP double*
#define DPP double**
#define DB scicos_block*

/*                    flag  nclock ntvec  rpar  nrpar ipar  nipar  u  nu */
#define ARGS_scicosm1 IP,    IP,    IP,    DP,   IP,  IP,   IP,   DP, IP

/* flag  nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar  intabl  ni  outabl no */
#define ARGS_scicos0 IP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DP,IP,DP,IP

/*       flag   nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar  intabl  .... */
#define ARGS_scicos IP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP

/*        flag   nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar   args_in sz_in, n_in  args_out sz_out, n_out  */
#define ARGS_scicos2 IP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DPP,IP,IP,DPP,IP,IP

/*        flag   nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar   args_in sz_in, n_in  args_out sz_out, n_out g ng */
#define ARGS_scicos2z IP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DPP,IP,IP,DPP,IP,IP,DP,IP

/*       flag   nclock t    residual xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar  intabl  .... */

#define ARGS_scicosi IP,IP,DP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP

/*        flag   nclockf t   residual xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar   args_in sz_in, n_in  args_out sz_out, n_out  */
#define ARGS_scicosi2 IP,IP,DP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DPP,IP,IP,DPP,IP,IP

/*        flag   nclockf t   residual xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar   args_in sz_in, n_in  args_out sz_out, n_out g ng */
#define ARGS_scicosi2z IP,IP,DP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DPP,IP,IP,DPP,IP,IP,DP,IP

/*         block     flag*/
#define ARGS_scicos4 DB, I


typedef void (*ScicosFm1) (ARGS_scicosm1);
typedef void (*ScicosF0) (ARGS_scicos0);
typedef void (*ScicosF) (ARGS_scicos);
typedef void (*ScicosF2) (ARGS_scicos2);
typedef void (*ScicosF2z) (ARGS_scicos2z);
typedef void (*ScicosFi) (ARGS_scicosi);
typedef void (*ScicosFi2) (ARGS_scicosi2);
typedef void (*ScicosFi2z) (ARGS_scicosi2z);
typedef void (*ScicosF4) (ARGS_scicos4);

typedef  struct  {
  char *name;
  ScicosF fonc;
} OpTab ;

#endif 
