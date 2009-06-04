#ifndef __SCICOS_BLOCK_H__
#define __SCICOS_BLOCK_H__

/**
* Add header of scicos_malloc
* scicos_free HERE
* to do modify dynamic link with scicos
*/

extern void * scicos_malloc();
extern void scicos_free(void *p);

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
  int *xprop;	   
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
double Get_Jacobian_cj(void);
double Get_Jacobian_ci(void);
double Get_Scicos_SQUR(void);
void Set_Jacobian_flag(int flag);

int Convert_number (char *, double *);
void homotopy(double *);
int hfjac_(double *, double *, int *);
int rhojac_(double *, double *,double *,double *, int *, double *,int *);
int rho_( double *, double *,double *,double *,double *, int *);
int fx_( double *, double *);
int read_xml_initial_states(int ,const char * , char **, double *);
int write_xml_states(int,const char *, char **, double *);
double pow_(double , double );
double exp_(double ); 
double log_(double ); 

#if _MSC_VER
	#ifdef min
		#undef min
	#endif 
	#ifdef max 
		#undef max
	#endif 
#endif 

#ifndef max
	#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif 

#ifndef min
	#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif

extern int s_copy();
extern int s_cmp();


#endif /* __SCICOS_BLOCK_H__ */
