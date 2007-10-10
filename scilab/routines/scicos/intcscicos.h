#ifndef INTCSCICOS_SCI
#define INTCSCICOS_SCI

#include "../machine.h"
#include "scicos_block4.h"

/* function declaration */
int MlistGetFieldNumber __PARAMS((int *ptr, const char *string));
int C2F(intcscicos)  _PARAMS((void));
int intendscicosim _PARAMS((char *fname, unsigned long fname_len));
int inttimescicos _PARAMS((char *fname, unsigned long fname_len));
int intduplicate  _PARAMS((char *fname, unsigned long fname_len));
int intdiffobjs  _PARAMS((char *fname, unsigned long fname_len));
int intxproperty  _PARAMS((char *fname, unsigned long fname_len));
int intphasesim  _PARAMS((char *fname, unsigned long fname_len));
int intsetxproperty  _PARAMS((char *fname, unsigned long fname_len));
int intsetblockerror _PARAMS((char *fname, unsigned long fname_len));
int inttree2 _PARAMS((char *fname, unsigned long fname_len));
int inttree3 _PARAMS((char *fname, unsigned long fname_len));
int inttree4 _PARAMS((char *fname, unsigned long fname_len));
static void duplicata();
static void comp_size();
int var2sci(void *x,int n,int m,int typ_var);
int createblklist(scicos_block *Blocks, int *ierr, int flag_imp, int kfun);

/* Definition of some global variables and structure */

static int *il_state_save; /*n'est valable que dans inctscicos.c*/
static int *il_sim_save; /*n'est valable que dans inctscicos.c*/

struct {integer kfun;} C2F(curblk); /* declaration of curblk -valable partout- */
struct {integer isrun;} C2F(cosim); /* declaration of cosim -valable partout- */
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
extern int C2F(mklist)();
extern void str2sci(char** x,int n,int m);
extern integer C2F(scierr)();
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

extern int C2F(scicos)(double *x_in, integer *xptr_in, double *z__,
                       void **work,integer *zptr,integer *modptr_in,
                       void **oz,integer *ozsz,integer *oztyp,integer *ozptr,
                       integer *iz,integer *izptr,double *t0_in,
                       double *tf_in,double *tevts_in,integer *evtspt_in,
                       integer *nevts,integer *pointi_in,void **outtbptr_in,
                       integer *outtbsz_in,integer *outtbtyp_in,
                       outtb_el *outtb_elem_in,integer *nelem1,integer *nlnk1,
                       integer *funptr,integer *funtyp_in,integer *inpptr_in,
                       integer *outptr_in, integer *inplnk_in,integer *outlnk_in,
                       double *rpar,integer *rpptr,integer *ipar,integer *ipptr,
                       void **opar,integer *oparsz,integer *opartyp,integer *opptr,
                       integer *clkptr_in,integer *ordptr_in,integer *nordptr1,
                       integer *ordclk_in,integer *cord_in,integer *ncord1,
                       integer *iord_in,integer *niord1,integer *oord_in,
                       integer *noord1,integer *zord_in,integer *nzord1,
                       integer *critev_in,integer *nblk1,integer *ztyp,
                       integer *zcptr_in,integer *subscr,integer *nsubs,
                       double *simpar,integer *flag__,integer *ierr_out);

#endif
