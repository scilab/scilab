#ifndef __SCICOS_H 
#define __SCICOS_H 

/* maximum value for sum of number of inputs and outputs ports of a given 
block of type 2 */
#define SZ_SIZE 60
/* maximum value for sum of number of inputs and outputs of a given block 
of type 0 */
#define TB_SIZE 500

/* common for ? */

extern struct {
  integer ptr;
} C2F(scsptr);

/* scicos common for debug */

extern struct {
    integer idb;
} C2F(dbcos);

typedef void (*voidf)();

/* Blocks prototypes */

#define IP integer*
#define DP double*
#define DPP double**

/*                    flag  nclock ntvec  rpar  nrpar ipar  nipar  u  nu */
#define ARGS_scicosm1 IP,    IP,    IP,    DP,   IP,  IP,   IP,   DP, IP

/* flag  nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar  intabl  ni  outabl no */
#define ARGS_scicos0 IP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DP,IP,DP,IP

/*       flag   nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar  intabl  .... */
#define ARGS_scicos IP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP

/*        flag   nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar   args_in sz_in, n_in  args_out sz_out, n_out  */
#define ARGS_scicos2 IP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DPP,IP,IP,DPP,IP,IP

typedef void (*ScicosFm1) __PARAMS((ARGS_scicosm1));
typedef void (*ScicosF0) __PARAMS((ARGS_scicos0));
typedef void (*ScicosF) __PARAMS((ARGS_scicos));
typedef void (*ScicosF2) __PARAMS((ARGS_scicos2));

typedef  struct  {
  char *name;
  ScicosF fonc;
} OpTab ;

#endif 
