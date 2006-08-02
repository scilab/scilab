#ifndef INTCSCICOS_SCI 
#define INTCSCICOS_SCI 

/* function declaration */
int MlistGetFieldNumber __PARAMS((int *ptr, const char *string));
int C2F(gw_cscicos)  _PARAMS((void));
int inttimescicos  _PARAMS((char *fname, unsigned long fname_len));
int intduplicate  _PARAMS((char *fname, unsigned long fname_len));
int intdiffobjs  _PARAMS((char *fname, unsigned long fname_len));
int intxproperty  _PARAMS((char *fname, unsigned long fname_len));
int intphasesim  _PARAMS((char *fname, unsigned long fname_len));
int intsetxproperty  _PARAMS((char *fname, unsigned long fname_len));
int intcpass2  _PARAMS((char *fname, unsigned long fname_len));
int intsetblockerror _PARAMS((char *fname, unsigned long fname_len));
int inttree2 _PARAMS((char *fname, unsigned long fname_len));
int inttree3 _PARAMS((char *fname, unsigned long fname_len));
int inttree4 _PARAMS((char *fname, unsigned long fname_len));
static void duplicata();
static void comp_size();
int var2sci(void *x,int n,int m,int typ_var);
int createblklist(scicos_block *Blocks, int *ierr, int flag_imp);

/*external declaration*/
/* declaration of getscicosvarsfromimport -see import.c- */
extern int getscicosvarsfromimport(char *what,void *v,int *nv,int *mv);
extern void C2F(vvtosci)();
extern int C2F(mktlist)();
extern int C2F(mklist)();
extern void str2sci(char** x,int n,int m);
extern integer C2F(scierr)();

extern int Max1(int* vect);
extern int *listentry(int *header, int i);
extern int Message(char* code);
extern int cpass2();

/* Definition of some global variables and structure */
static int *il_state_save; /*n'est valable que dans inctscicos.c*/
static int *il_sim_save; /*n'est valable que dans inctscicos.c*/
struct {integer kfun;} C2F(curblk); /* declaration of curblk -valable partout- */
typedef struct /* declaration of outtb_elem -valable dans inctscicos.c- */
{
  int lnk;
  int pos;
} outtb_el;

#endif 
