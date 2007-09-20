#ifndef __SCICOS_BLOCK_H__
#define __SCICOS_BLOCK_H__

#ifdef __STDC__
#include <stdlib.h>
#else 
	#ifndef WIN32
		#include <malloc.h>
	#endif
#endif 

typedef void (*voidg)();

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
  double **inptr;
  int nout;
  int *outsz;
  double **outptr;
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

#ifndef WIN32
#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif

extern int s_copy();
extern int s_cmp();


#endif /* __SCICOS_BLOCK_H__ */
