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
extern int C2F(callinterf) __PARAMS((integer *,integer *));
extern int C2F(putfunnam) __PARAMS((char *fname,integer *lw, unsigned long));


/* from intersci */

extern void C2F(cchar) __PARAMS((int *n,char **ip,int *op));
extern void C2F(cdouble) __PARAMS((int *n,double **ip,double *op));
extern void C2F(cint)  __PARAMS((int *n,int **ip,double *op));
extern void C2F(cfloat) __PARAMS ((     int *n,     float *ip[],     double *op));
extern void C2F(cbool) __PARAMS ((     int *n,     int *ip[],     int *op));

/* from stack2.h */

extern integer C2F(overload) __PARAMS(( integer *lw, char *fname, unsigned long l));
extern integer C2F(overloadtype) __PARAMS(( integer *lw, char *fname,char *typ));

extern void  C2F(setfeval) __PARAMS((char *,int *)); 
extern int C2F(objvide)  __PARAMS((char *,integer *,unsigned long));

extern int C2F(convert2sci) __PARAMS((int *)); 

extern int C2F(putlhsvarfd) __PARAMS((void));
extern int C2F(putlhsvar) __PARAMS((void));
extern int C2F(cvstr1old) __PARAMS((integer *n, integer *line, char *str, integer *job, unsigned long str_len));
extern int C2F(tryenhaut) __PARAMS((integer *itopl, integer *i__));
extern int C2F(isref) __PARAMS((integer *lw));
extern integer C2F(vartype) __PARAMS((integer *number));
extern int C2F(createvar) __PARAMS((integer *lw, char *type__, integer *m, integer *n, integer *lr, unsigned long type_len));

extern int C2F(creatework)  __PARAMS((integer *number,integer *m,integer  *lr));

extern int C2F(setworksize)  __PARAMS((integer *number,integer *size));

extern int C2F(createcvar) __PARAMS((integer *lw, char *type__, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long type_len));
extern int C2F(createlist) __PARAMS((integer *lw, integer *nel));
extern int C2F(createvarfrom) __PARAMS((integer *lw, char *type__, integer *m, integer *n, integer *lr, integer *lar, unsigned long type_len));
extern int C2F(createcvarfrom) __PARAMS((integer *lw, char *type__, integer *it, integer *m, integer *n, integer *lr, integer *lc, integer *lar, integer *lac, unsigned long type_len));
extern int C2F(createlistvarfrom) __PARAMS((integer *lnumber, integer *number, char *type__, integer *m, integer *n, integer *lr, integer *lar, unsigned long type_len));
extern int C2F(createlistcvar) __PARAMS((integer *lnumber, integer *number, char *type__, integer *it, integer *m, integer *n, integer *lr, integer *lc, integer *lar, integer *lac, unsigned long type_len));
extern int C2F(fakecresmat2) __PARAMS((integer *lw, integer *nchar, integer *lr));
extern int C2F(getmatdims)  __PARAMS((integer *number, integer *m, integer *n));
extern int C2F(getrhsvar) __PARAMS((integer *number, char *type__, integer *m, integer *n, integer *lr, unsigned long type_len));
extern int C2F(getrhscvar) __PARAMS((integer *number, char *type__, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long type_len));
extern int C2F(getlistrhsvar) __PARAMS((integer *lnumber, integer *number, char *type__, integer *m, integer *n, integer *lr,  unsigned long type_len));
extern int C2F(getlistrhscvar) __PARAMS((integer *lnumber, integer *number, char *type__, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long type_len));


extern int C2F(createvarfromptr) __PARAMS((integer *number, char *type__, integer *m, integer *n,void *iptr, unsigned long type_len)); 
/*
extern int C2F(createcvarfromptr) __PARAMS((integer *number, char *type__, integer *it, integer *m, integer *n, integer *iptr, integer *iptc, unsigned long type_len));
*/
extern int C2F(mklistfromvars) __PARAMS((integer *pos, integer *n));
extern int C2F(mktlistfromvars) __PARAMS((integer *pos, integer *n));
extern int C2F(mkmlistfromvars) __PARAMS((integer *pos, integer *n));
extern int C2F(in2str) __PARAMS((integer *n, integer *line, char *str, unsigned long str_len));
extern int C2F(callscifun) __PARAMS((char *string, unsigned long string_len));
extern int C2F(scifunction) __PARAMS((integer *number, integer *ptr, integer *mlhs, integer *mrhs));
extern int C2F(scistring) __PARAMS((integer *ifirst, char *thestring, integer *mlhs, integer *mrhs, unsigned long thestring_len));
extern integer C2F(getopcode) __PARAMS((char *string, unsigned long string_len));
extern int C2F(scibuiltin) __PARAMS((integer *number, integer *ifun, integer *ifin, integer *mlhs, integer *mrhs));
extern int C2F(sciops) __PARAMS((integer *number, integer *op, integer *mlhs, integer *mrhs));
extern int C2F(getrhssys) __PARAMS((integer *lw, integer *n, integer *m, integer *p, integer *ptra, integer *ptrb, integer *ptrc, integer *ptrd, integer *ptrx0, double *h__));
extern int C2F(errorinfo) __PARAMS((char *fname, integer *info, unsigned long fname_len));
extern integer C2F(maxvol) __PARAMS((integer *lw, char *type__, unsigned long type_len));

extern int  C2F(createlistvarfromptr)  __PARAMS((integer *lnumber, integer *number, char *typex, integer *m, integer *n, void *iptr, long unsigned int type_len));

extern int C2F(createlistcvarfromptr)  __PARAMS((integer * lnumber,integer * number, char * typex,integer *it,integer * m,integer * n, void *iptr, void * iptc, unsigned long ttype_len));

extern int C2F(createdata) __PARAMS((integer *lw, integer n));

char * ArgPosition __PARAMS((int i));
char * ArgsPosition __PARAMS((int i,int j));

int IsRef __PARAMS((int lw));
int Ref2val __PARAMS((int from , int to )) ; 

extern void ConvertData __PARAMS(( char *type, int size,int l));
extern int C2F(createreffromname) __PARAMS((int number, char *name ));
extern int C2F(createreference) __PARAMS(( int number, int pointed ));
extern int C2F(changetoref) __PARAMS(( int number, int pointed ));

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
