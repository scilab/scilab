#ifndef SCI_PVM 
#define SCI_PVM 

void C2F(scipvmbcast)(char *group, int *l, 
		      int *pack, int *n, 
		      double *buff,
		      int *msgtag, int *res); 
void C2F(scipvmreduce)(char *func, int *l1,
		       double *buff, int *m, int *n, int *msgtag, 
		       char *group, int *l2, int *rootginst, int *res);

void C2F(scipvmconfig)(int *nhost, int *narch, int **dtid, 
		       char ***name, char ***arch, int **speed, 
		       int *n, int *info);

void C2F(scipvmtasks)(int *where, int *ntask, 
		      int **tid, int **ptid, int **dtid, int **flag,
		      char ***name, int *n, int *info);


void C2F(scipvmbufinfo)(int *bufid, int *bytes, int *msgtag, int * tid, int *res); 

char *scipvm_error_msg(int err); 

void C2F(scipvmstart)(int *res, char *hostfile, int *l);


void C2F(scipvmsettimer)(int *res);
void C2F(scipvmgettimer)(double *res);
void C2F(scipvmspawn)(char *task,  int *l1, 
		      char *win,   int *l2,
		      char *where, int *l3, 
		      int *ntask,  int *tids, int *res);


void C2F(scipvmspawnindependent)(char *task,  int *l1, 
			    int *ntask,
			    char *where, int *l3, 
			    int *tids, int *res);
void C2F(scipvmrecv)(double *beginvar, int *maxsize, int *size,
		int *tid,  int *tag, int *res);
void C2F(scipvmrecvvar)(int *tid,  int *tag, char *buff, int *res);


void  C2F(scipvmsendvar)(int *tids, int *p, char *buff, 
			 int *msgtag, int *res);

void C2F(scipvmsend)(int *tids, int *p, 
		     int *pack, int *n, 
		     double *buff,
		     int *msgtag, int *res);

void C2F(scipvmscitof77)(int *k);
void C2F(scipvmf77tosci)(int *k);

void C2F(scipvmhalt)(int *res);
void C2F(ccomplexf)(int *n, double **ip, double *op); 
void SciToF77(double *ptr, int size, int lda);
void F77ToSci(double *ptr, int size, int lda);
int C2F(varpak)(int *k, int *pack, int *n, int *nMax, int *ierr); 

#endif 
