typedef void (*voidg)();

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
  int ng;
  double *g;
  int ztyp;
  int *jroot;
  char *label;
  void **work;
  int mode;
} scicos_block;


void do_cold_restart();
int get_phase_simulation(int p);
int* get_pointer_xproperty(int p);
double get_scicos_time(double p);
int get_block_number();
void set_block_error(int);
void set_pointer_xproperty(int* pointer,int n);
