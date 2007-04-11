#ifndef STACK2_H 
#define  STACK2_H 

/* from sci_tools */

void SciToF77 __PARAMS((double *ptr, int size, int lda));
void F77ToSci __PARAMS((double *ptr, int size, int lda));
void double2z __PARAMS((double *ptr,  double *ptr77z, int size, int lda));
void z2double __PARAMS((double *ptrz, double *ptrsci, int size, int lda));

/* from int */

extern  int C2F(tpconv) __PARAMS((integer *,integer *,integer *,void *, integer *,void *, integer *));

/* from system */

extern int C2F(mklist) __PARAMS((integer *));
extern int C2F(mklistt) __PARAMS((integer *,integer *));
extern int C2F(funs) __PARAMS((integer *));
extern int C2F(putid) __PARAMS((integer *,integer *));
extern int C2F(parse) __PARAMS((void));
extern int C2F(allowptr) __PARAMS((integer *));
extern int C2F(ref2val) __PARAMS((void));
extern int C2F(allops) __PARAMS((void));
extern int C2F(callinterf) __PARAMS((integer *));
extern int C2F(putfunnam) __PARAMS((char *fname,integer *lw, unsigned long));


/* from intersci */

extern void C2F(cchar) __PARAMS((int *n,char **ip,int *op));
extern void C2F(cdouble) __PARAMS((int *n,double **ip,double *op));
extern void C2F(cint)  __PARAMS((int *n,int **ip,double *op));
extern void C2F(cfloat) __PARAMS ((     int *n,     float *ip[],     double *op));
extern void C2F(cbool) __PARAMS ((     int *n,     int *ip[],     int *op));

/* from somewhere (maybe stack1.h) */

extern void C2F(setfeval) __PARAMS((char *,int *)); 
extern int C2F(fakecresmat2) __PARAMS((integer *lw, integer *nchar, integer *lr));
/* from stack2.h */
int C2F(isoptlw)(integer *topk,integer  *lw, char *namex, unsigned long name_len);
integer C2F(numopt)(void);
integer C2F(overload) __PARAMS(( integer *lw, char *fname, unsigned long l));
integer C2F(overloadtype) __PARAMS(( integer *lw, char *fname,char *typ));

int C2F(convert2sci) __PARAMS((int *));


/**
 * int C2F(putlhsvar)()
 *     This function put on the Scilab stack the lhs 
 *     variables which are at the position lhsvar(i) 
 *     on the calling stack 
 *     Warning : this function supposes that the last 
 *     variable on the stack is at position top-rhs+nbvars
 * @return  
 */
int C2F(putlhsvar) __PARAMS((void));

int C2F(isref) __PARAMS((integer *lw));
integer C2F(vartype) __PARAMS((integer *number));
integer C2F(elementtype) __PARAMS((integer *lnumber,integer *number));

/**
 * Create a variable into the Scilab stack
 * You will probably need to do after a istk(xxxx) = yourvalue;
 * TODO : add a @see to the type of data
 * @param lw Position in the stack
 * @param type__ Type of data ('c','d','r','i','l','b')
 * @param m number of line of the matrix
 * @param n number of columns of the matrix
 * @param[out] lr the address of the data in Scilab Stack
 * @param type_len length of the char *type__
 */
int C2F(createvar) __PARAMS((integer *lw, char *type__, integer *m, integer *n, integer *lr, unsigned long type_len));

int C2F(creatework)  __PARAMS((integer *number,integer *m,integer  *lr));

int C2F(setworksize)  __PARAMS((integer *number,integer *size));


/**
 * Create a complex variable into the Scilab stack
 * You will probably need to do after a istk(xxxx) = yourvalue;
 * TODO : add a @see to the type of data
 * @example
 * Add the content of comm_size into the stack of Scilab 
 * @code
 	int n1=1,m1=1,l1=0;
	int comm_size=42;
	CreateVar(1, "i", &m1, &n1 ,&l1);
	*istk(l1)=comm_size;
 * @endcode
 * @param lw Position in the stack
 * @param type__ Type of data ('c','d','r','i','l','b')
 * @param it if it is complex or not (not sure)
 * @param m number of line of the matrix
 * @param n number of columns of the matrix
 * @param[out] lr the address of the real data in Scilab Stack
 * @param[out] lc the address of the complex data in Scilab Stack
 * @param type_len length of the char *type__
 */
int C2F(createcvar) __PARAMS((integer *lw, char *type__, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long type_len));


int C2F(createlist) __PARAMS((integer *lw, integer *nel));
int C2F(createvarfrom) __PARAMS((integer *lw, char *type__, integer *m, integer *n, integer *lr, integer *lar, unsigned long type_len));
int C2F(createcvarfrom) __PARAMS((integer *lw, char *type__, integer *it, integer *m, integer *n, integer *lr, integer *lc, integer *lar, integer *lac, unsigned long type_len));
int C2F(createlistvarfrom) __PARAMS((integer *lnumber, integer *number, char *type__, integer *m, integer *n, integer *lr, integer *lar, unsigned long type_len));

int C2F(getmatdims)  __PARAMS((integer *number, integer *m, integer *n));
int C2F(getrhsvar) __PARAMS((integer *number, char *type__, integer *m, integer *n, integer *lr, unsigned long type_len));
int C2F(getrhscvar) __PARAMS((integer *number, char *type__, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long type_len));
int C2F(getlistrhsvar) __PARAMS((integer *lnumber, integer *number, char *type__, integer *m, integer *n, integer *lr,  unsigned long type_len));
int C2F(getlistrhscvar) __PARAMS((integer *lnumber, integer *number, char *type__, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long type_len));



/**
 * Create a variable into the Scilab stack from a pointer
 * @param lw Position in the stack
 * @param type__ Type of data (i for integer, c for char, b for boolean ...)
 * @param m number of line of the matrix
 * @param n number of columns of the matrix
 * @param iptr the data we want to add into the stack
 * @param type_len length of the char *type__
 */
int C2F(createvarfromptr) __PARAMS((integer *number, char *type__, integer *m, integer *n,void *iptr, unsigned long type_len)); 


/**
 * Create a complex variable into the Scilab stack from a pointer
 * @param lw Position in the stack
 * @param type__ Type of data (i for integer, c for char, b for boolean ...)
 * @param m number of line of the matrix
 * @param n number of columns of the matrix
 * @param iptr the data we want to add into the stack
 * @param iptc the complex data we want to add into the stack
 * @param type_len length of the char *type__
 */
int C2F(createcvarfromptr) __PARAMS((integer *number, char *type__, integer *it, integer *m, integer *n, double *iptr, double *iptc, unsigned long type_len));

int C2F(mklistfromvars) __PARAMS((integer *pos, integer *n));
int C2F(mktlistfromvars) __PARAMS((integer *pos, integer *n));
int C2F(mkmlistfromvars) __PARAMS((integer *pos, integer *n));
int C2F(in2str) __PARAMS((integer *n, integer *line, char *str, unsigned long str_len));
int C2F(callscifun) __PARAMS((char *string, unsigned long string_len));
int C2F(scifunction) __PARAMS((integer *number, integer *ptr, integer *mlhs, integer *mrhs));
int C2F(scistring) __PARAMS((integer *ifirst, char *thestring, integer *mlhs, integer *mrhs, unsigned long thestring_len));
integer C2F(getopcode) __PARAMS((char *string, unsigned long string_len));
int C2F(scibuiltin) __PARAMS((integer *number, integer *ifun, integer *ifin, integer *mlhs, integer *mrhs));
int C2F(sciops) __PARAMS((integer *number, integer *op, integer *mlhs, integer *mrhs));
int C2F(getrhssys) __PARAMS((integer *lw, integer *n, integer *m, integer *p, integer *ptra, integer *ptrb, integer *ptrc, integer *ptrd, integer *ptrx0, double *h__));
int C2F(errorinfo) __PARAMS((char *fname, integer *info, unsigned long fname_len));
integer C2F(maxvol) __PARAMS((integer *lw, char *type__, unsigned long type_len));

int  C2F(createlistvarfromptr)  __PARAMS((integer *lnumber, integer *number, char *typex, integer *m, integer *n, void *iptr, long unsigned int type_len));

int C2F(createlistcvarfromptr)  __PARAMS((integer * lnumber,integer * number, char * typex,integer *it,integer * m,integer * n, void *iptr, void * iptc, unsigned long ttype_len));
int C2F(createlistcvarfrom)(integer *lnumber, integer *number, char *typex, integer *it, integer *m,integer *n,integer *lr,integer *lc,integer *lar,integer *lac, unsigned long type_len);

int C2F(createdata) __PARAMS((integer *lw, integer n));

char * ArgPosition __PARAMS((int i));
char * ArgsPosition __PARAMS((int i,int j));

int IsRef __PARAMS((int lw));
int Ref2val __PARAMS((int from , int to )) ; 

void *GetData __PARAMS((int lw));
void *GetRawData __PARAMS((int lw));
int GetDataSize __PARAMS((int lw));
void *GetDataFromName __PARAMS(( char *name ));
void ConvertData __PARAMS(( char *type, int size,int l));
int C2F(createreffromname) __PARAMS((int number, char *name ));
int C2F(createreference) __PARAMS(( int number, int pointed ));
int C2F(changetoref) __PARAMS(( int number, int pointed ));

int check_square __PARAMS((int pos,int m,int n));
int check_vector __PARAMS((int pos,int m,int n));
int check_row __PARAMS((int pos,int m,int n));
int check_col __PARAMS((int pos,int m,int n));
int check_scalar __PARAMS((int pos,int m,int n));
int check_dims __PARAMS((int pos,int m,int n,int m1,int n1));
int check_length __PARAMS((int pos,int m,int n));
int check_same_dims   __PARAMS((int pos1,int pos2,int m1,int n1,int m2,int n2));
int check_dim_prop  __PARAMS((int pos1,int pos2,int n));
int check_one_dim   __PARAMS((int pos1,int dim,int val,int valref));

int check_list_square __PARAMS((int lpos,int pos,int m,int n));
int check_list_vector __PARAMS((int lpos,int pos,int m,int n));
int check_list_row __PARAMS((int lpos,int pos,int m,int n));
int check_list_col __PARAMS((int lpos,int pos,int m,int n));
int check_list_scalar __PARAMS((int lpos,int pos,int m,int n));
int check_list_one_dim __PARAMS((int lpos,int pos,int dim,int val,int valref));

#endif 
