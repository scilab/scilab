/* VERSION et DATE */
#define VERSION "3.0"
#define DATE "SEP 2000" 

/* max dimension for the array: can be modified */
#define MAXARG 50
#define MAXCALL 2000
#define MAXEL 50
#define MAXLINE 1000
#define MAXNAM 128
#define MAXVAR 200
/* flag for generation of type and element dimension checking for lists: 
   can be modified */
#define TESTLISTELEMENTS 1

/******************************************/
/* DO NOT CHANGE ANYTHING BELOW THIS LINE */
/******************************************/
#define MAXFUN 99 /* maximum number of SCILAB functions */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifdef __STDC__
#include <stdlib.h>
#else 
#include <malloc.h>
#endif

/* FORTRAN variable types */ 
#define CHAR 1
#define INT 2
#define DOUBLE 3
#define REAL 4
#define EXTERNAL 5
#define CSTRINGV 6
#define LOGICAL  7
#define MPOINTER 8 /* pointer to Mat */
#define PREDEF  9 /* err,rhs,lhs */
#define SMPOINTER 9 /* pointer to SMat */
#define LPOINTER 10 /* pointer to List */
#define BPOINTER 11 /* pointer to BMat */
#define OPOINTER 12 /* pointer to OBJ */
#define BOOLEAN     13 /* as int for boolean matrix **/

/* Fortran Declaration */

#define DEC_CHAR 1
#define DEC_INT 2
#define DEC_DOUBLE 3
#define DEC_REAL 4
#define DEC_LOGICAL  5
#define DEC_DATA  6
#define DEC_UL  7    /* unsigned long */
#define DEC_INIT  8    /* initialisation x=0; etc... */
#define DEC_SMAT  9  
#define DEC_SPARSE 10 
/* pointer declaration */
#define DEC_IPTR  11
#define DEC_DPTR  12
#define DEC_RPTR  13
#define DEC_CPTR  14
#define DEC_SPARSEPTR 15


/* SCILAB and extensions variable types */

#define DIMFOREXT 0   /* dimension (for example of an external variable )*/
#define COLUMN 1
#define LIST 2
#define TLIST 3
#define MATRIX 4
#define POLYNOM 5
#define ROW 6
#define SCALAR 7
#define SEQUENCE 8
#define STRING 9
#define WORK 10
#define EMPTY 11
#define ANY 12
#define VECTOR 13
#define STRINGMAT 14
#define SCIMPOINTER 15
#define IMATRIX 16
#define SCISMPOINTER 17
#define SCILPOINTER 18
#define BMATRIX 19
#define SCIBPOINTER 20
#define SCIOPOINTER 21
#define SPARSE 22
#define MLIST 23

/* SCILAB optional variable types */

#define NAME 1 /* {var default-name} */
#define VALUE 2 /* {var default-value} */

typedef int IVAR; /* variable number */

/* VAR struct: informations for FORTRAN and/or SCILAB variable */


typedef struct var {
  char *name;           /* variable name */
  int  vpos;            /* variable is stored in variables[vpos-1] */
  int type;             /* SCILAB type */
  int length;           /* number of el in the variable */
  IVAR el[MAXEL];       /* list of el IVAR (variable associated with,
		                            typically dimensions) */
  int for_type;         /* FORTRAN type */
  char fexternal[MAXNAM]; /* name of external function when type is
			      external */
  IVAR equal;           /* alias for variable  */
  int nfor_name;        /* number of for_name */
  int kp_state;         /* for pass dealing **/
  char *for_name[MAXARG]; /* list of for_name names (FORTRAN name
			   in generated FORTRAN code) */
  char *C_name[MAXARG]; /* list of C_name : i.e when a for_name is m1*n1 
			 * it cannot be used at C level then C_name is set to 
			 * m1n1 and one will have to properly set m1n1=m1*n1 
			 */
  
  int for_name_orig[MAXARG]; /* list of stack_position of for_name occurences */
  char *list_name;      /* name of the list of which the variable is an element */
  int list_el;          /* element number in the previous list 
			 0 : means that variable is not in a list */
  int opt_type;         /* type of optional variable */
  char *opt_name;       /* name or value default for optional variable */
  int present;          /* 1 if the variable is really present in the 
			   description file
		           0 otherwise used for list elements which might 
			   be not really present */
  int stack_position;   /* position of the variable in the stack 
			   1 is the first position : 
			   the position is : the position in the scilab 
			   calling list or the position in the stack for 
			   internal variables created inside the interface
			   for a variable in a list : it is the position 
			   of the list in the stack 
			   */
  int out_position ;    /* 
			   The position of the variable in the 
			   returned arguments 
			   0 if the variable is not returned 
			   */
  int is_sciarg ;          /* set to one if variable is a scilab 
			      argument of the interfaced function */

} VAR, *VARPTR;

/* BASFUN struct: informations for SCILAB function */

typedef struct basfun {
  char *name; /* function name */
  int nin; /* number of arguments */
  int maxOpt ; /* number of potential optional arguments in function */
  int NewMaxOpt; /* number of new style optional arguments in function */
  IVAR in[MAXARG]; /* list of argument IVAR */
  IVAR out; /* output IVAR : i.e type of output */
} BASFUN, *BASFUNPTR;

/* FORSUB struct: informations for FORTRAN subroutine */

typedef struct forsub {
  char *name; /* subroutine name */
  int narg; /* number of arguments */
  IVAR arg[MAXARG]; /* list of argument IVAR */
} FORSUB, *FORSUBPTR;

/* memory allocators */

/** functions **/

#ifdef __STDC__
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		paramlist
#endif
#else	
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		()
#endif
#endif


extern VARPTR VarAlloc  __PARAMS((void));
extern BASFUNPTR BasfunAlloc  __PARAMS((void));
extern FORSUBPTR ForsubAlloc  __PARAMS((void));

void WriteInfoCode __PARAMS((FILE *f));
int GetNumberInScilabCall __PARAMS((int ivar));
int GetNumberInFortranCall __PARAMS((int ivar));
char *SGetSciType __PARAMS((int type));
char* SGetCDec __PARAMS((int type));
char *SGetForType __PARAMS((int type));
char *SGetForTypeAbrev __PARAMS((VARPTR var));
char *SGetForTypeStack __PARAMS((VARPTR var));
char *SGetForTypeBConvert __PARAMS((VARPTR var));
void AddForName __PARAMS((IVAR ivar, char *name,char *cname,IVAR ivar1));
void ChangeForName1 __PARAMS((VARPTR var, char *name));
void Copyright __PARAMS((void));
char *Forname2Int __PARAMS((VARPTR,int));
void GenFundef __PARAMS((char *file, int interf));
int GetBasType __PARAMS((char *sname));
int GetForType __PARAMS((char *type));
IVAR GetExistOutVar __PARAMS((void));
IVAR CheckOutVar __PARAMS((void));
IVAR GetExistVar __PARAMS((char *name));
IVAR GetOutVar __PARAMS((char *name));
IVAR GetVar __PARAMS((char *name, int p));
void OptVar __PARAMS(());
int ParseLine __PARAMS((char *s, char **words));
int ParseScilabLine __PARAMS((char *s, char **words));
int ReadListElement __PARAMS((FILE *f, char *varlistname, IVAR iivar, int nel,int));
void ReadListFile __PARAMS((char *listname, char *varlistname, IVAR ivar,int));
int ReadFunction __PARAMS((FILE *f));
int TypeToBas __PARAMS(());
void WriteArgCheck __PARAMS((FILE *f, int i));
void WriteCall __PARAMS(());
void WriteCallRest __PARAMS((FILE *f, IVAR ivar, int farg, char *call));
void WriteCallConvertion __PARAMS((FILE *f, IVAR ivar, char *farg, char *barg, char *call));
void WriteCrossCheck __PARAMS((FILE *f));
void WriteEqualCheck __PARAMS((FILE *f));
void WriteExternalVariableOutput __PARAMS((FILE *f, VARPTR var,  int insidelist, int nel));
void WriteFortranCall __PARAMS((FILE *f));
void WriteFunctionCode __PARAMS((FILE *f));
void WriteHeader __PARAMS((FILE *f, char *fname0, char *fname));
void WriteMainHeader __PARAMS((FILE *f, char *fname));
void WriteListAnalysis __PARAMS((FILE *f, int i,char *));
void WriteOutput __PARAMS((FILE *f));
void WriteVariable __PARAMS((FILE *f, VARPTR var, IVAR ivar, int insidelist, int nel));
void WriteVariableOutput __PARAMS((FILE *f, VARPTR var,  int convert, int insidelist, int nel));
void AddForName1 __PARAMS((IVAR ivar, char *name,char *cname, IVAR ivar1));
void ForNameClean __PARAMS((void));
void InitDeclare  __PARAMS((void));
void ResetDeclare __PARAMS((void));
void WriteMain  __PARAMS((FILE *f,char *file));
void FCprintf  __PARAMS((FILE*,char *fmt,...));
void Fprintf   __PARAMS((FILE*,int,char *fmt,...));
void white __PARAMS( (FILE *f, int ind));
void AddDeclare __PARAMS((int type, char *declaration));
void InitDeclare __PARAMS((void));
void ResetDeclare __PARAMS((void));
void WriteDeclaration  __PARAMS((FILE*f));
void WriteCallRestCheck __PARAMS((FILE *f, VARPTR var,char *name, int iel, int flag));
int CreatePredefVar __PARAMS((char *name));
void Check __PARAMS((FILE *f,  VARPTR var,  int nel));
void CheckSquare __PARAMS((FILE *f,  VARPTR var, char *str,char *str1));
void CheckOptSquare __PARAMS((FILE *f,  VARPTR var, char *str));
void CheckOptDim __PARAMS((FILE *f,  VARPTR var,  int nel));
void OptvarGetSize __PARAMS((char *optvar, char *size, char *data));
void WriteAddInter __PARAMS((char *file));


/*** Global variables **/

extern VARPTR variables[MAXVAR]; /* array of VAR structures */
extern int nVariable; /* number of variables */
extern BASFUNPTR basfun; /* SCILAB function structure */
extern int icre;     /* incremental counter for variable creation */
extern int indent; /* incremental counter for code indentation */
extern int pass ;  /* flag for couting pass on code generation */
extern FORSUBPTR forsub; /* FORTRAN subroutine structure */
extern int nFun; /* total number of functions in "desc" file */
extern char *funNames[MAXFUN]; /* array of function names */
extern char target; /* langage for generation F or C */



/* protos */

extern void Generate __PARAMS(( char *file));
extern int ShowVariables  __PARAMS((void)); 
extern int FixStackPositions  __PARAMS((void)); 
extern IVAR GetVar  __PARAMS((char *name, int p)); 
extern IVAR GetExistVar  __PARAMS((char *name)); 
extern int CreatePredefVar  __PARAMS((char *name)); 
extern IVAR GetOutVar  __PARAMS((char *name)); 
extern IVAR GetExistOutVar  __PARAMS((void)); 
extern void AddForName  __PARAMS((IVAR ivar, char *name, char *cname, IVAR ivar1)); 
extern void AddForName1  __PARAMS((IVAR ivar, char *name, char *cname, IVAR ivar1)); 
extern void ForNameClean  __PARAMS((void)); 
extern void ChangeForName2  __PARAMS((VARPTR var, char *format,...));
extern void ChangeForName1  __PARAMS((VARPTR var, char *name)); 
extern int GetBasType  __PARAMS((char *sname)); 
extern char *SGetSciType  __PARAMS((int type)); 
extern int GetForType  __PARAMS((char *type)); 
extern char *SGetForType  __PARAMS((int type)); 
extern char *SGetForTypeAbrev  __PARAMS((VARPTR var)); 
extern int SGetForDec  __PARAMS((int type)); 
extern char *SGetCDec  __PARAMS((int type)); 
extern char *SGetForTypeStack  __PARAMS((VARPTR var)); 
extern char *SGetForTypeBConvert  __PARAMS((VARPTR var)); 
extern char *SGetExtForTypeAbrev  __PARAMS((VARPTR var)); 
extern char *SGetExtForTypeStack  __PARAMS((VARPTR var)); 
extern void StrGen  __PARAMS((char *strl, VARPTR var)); 
extern void ForMATRIX  __PARAMS((VARPTR var)); 
extern void ForSTRING  __PARAMS((VARPTR var)); 
extern void ForIMATRIX  __PARAMS((VARPTR var)); 
extern void ForSPARSE  __PARAMS((VARPTR var)); 
extern void ForROW  __PARAMS((VARPTR var)); 
extern void ForCOLUMN  __PARAMS((VARPTR var)); 
extern void ForVECTOR  __PARAMS((VARPTR var)); 
extern void ForPOLYNOM  __PARAMS((VARPTR var)); 
extern void ForSCALAR  __PARAMS((VARPTR var)); 
extern void ForPOINTER  __PARAMS((VARPTR var)); 
extern void ForANY  __PARAMS((VARPTR var)); 
extern void ForLIST  __PARAMS((VARPTR var)); 
extern void ForTLIST  __PARAMS((VARPTR var)); 
extern void ForSEQUENCE  __PARAMS((VARPTR var)); 
extern void ForEMPTY  __PARAMS((VARPTR var)); 
extern void ForWORK  __PARAMS((VARPTR var)); 
extern void ForDIMFOREXT  __PARAMS((VARPTR var)); 
extern int main  __PARAMS((unsigned int argc, char **argv)); 
extern void Generate  __PARAMS((char *file)); 
extern int FixStackPositions  __PARAMS((void));
extern void FixForNames  __PARAMS(());
extern void WriteMain  __PARAMS((FILE *f, char *file)); 
extern void WriteAddInter  __PARAMS((char *file)); 
extern void Copyright  __PARAMS((void)); 
extern void WriteHeader  __PARAMS((FILE *f, char *fname0, char *fname)); 
extern void WriteFunctionCode  __PARAMS((FILE *f)); 
extern void WriteOptArgPhase0  __PARAMS((FILE *f, int i));
extern void WriteOptArgPhase1  __PARAMS((FILE *f, int i));
extern void WriteOptArgPhase2  __PARAMS((FILE *f, int i));

extern void WriteInfoCode  __PARAMS((FILE *f)); 
extern void WriteArgCheck  __PARAMS((FILE *f, int i)); 
extern void WriteOptArg  __PARAMS((FILE *f , VARPTR var)); 
extern void WriteCrossCheck  __PARAMS((FILE *f)); 
extern void WriteCrossCheckExternal __PARAMS((FILE *f)); 
extern void WriteEqualCheck  __PARAMS((FILE *f)); 
extern void WriteListAnalysis  __PARAMS((FILE *f, int i, char *list_type)); 
extern void AddDeclare1 __PARAMS((int type,char *format,...));

extern void WriteFortranCall  __PARAMS((FILE *f)); 
extern void WriteOutput  __PARAMS((FILE *f)); 
extern void WriteVariable  __PARAMS((FILE *f, VARPTR var, IVAR ivar, int insidelist, int nel)); 
extern int GetNumberInScilabCall  __PARAMS((int ivar)); 
extern int GetNumberInFortranCall  __PARAMS((int ivar)); 
extern char *Forname2Int  __PARAMS((VARPTR var, int i)); 
extern void GenFundef  __PARAMS((char *file, int interf)); 
extern void white  __PARAMS((FILE *f, int ind)); 
extern VARPTR VarAlloc  __PARAMS((void)); 
extern BASFUNPTR BasfunAlloc  __PARAMS((void)); 
extern FORSUBPTR ForsubAlloc  __PARAMS((void)); 
extern int ReadFunction  __PARAMS((FILE *f)); 
extern int ParseScilabLine  __PARAMS((char *s, char **words)); 
extern int ParseLine  __PARAMS((char *s, char **words)); 
extern void ReadListFile  __PARAMS((char *listname, char *varlistname, IVAR ivar, int stack_position)); 
extern int ReadListElement  __PARAMS((FILE *f, char *varlistname, IVAR iivar, int nel, int stack_position)); 

extern int ShowVariables  __PARAMS((void)); 
extern int FixStackPositions  __PARAMS((void)); 
extern IVAR GetVar  __PARAMS((char *name, int p)); 
extern IVAR GetExistVar  __PARAMS((char *name)); 
extern int CreatePredefVar  __PARAMS((char *name)); 
extern IVAR GetOutVar  __PARAMS((char *name)); 
extern IVAR GetExistOutVar  __PARAMS((void)); 
extern void AddForName  __PARAMS((IVAR ivar, char *name, char *cname, IVAR ivar1)); 
extern void AddForName1  __PARAMS((IVAR ivar, char *name, char *cname, IVAR ivar1)); 
extern void ForNameClean  __PARAMS((void)); 
extern void ChangeForName1  __PARAMS((VARPTR var, char *name)); 
extern int GetBasType  __PARAMS((char *sname)); 
extern char *SGetSciType  __PARAMS((int type)); 
extern int GetForType  __PARAMS((char *type)); 
extern char *SGetForType  __PARAMS((int type)); 
extern char *SGetForTypeAbrev  __PARAMS((VARPTR var)); 
extern int SGetForDec  __PARAMS((int type)); 
extern char *SGetCDec  __PARAMS((int type)); 
extern char *SGetForTypeStack  __PARAMS((VARPTR var)); 
extern char *SGetForTypeBConvert  __PARAMS((VARPTR var)); 
extern char *SGetExtForTypeAbrev  __PARAMS((VARPTR var)); 
extern char *SGetExtForTypeStack  __PARAMS((VARPTR var)); 
extern void OutCOLUMN  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutROW  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutVECTOR  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutMATRIX  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutSCALAR  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutCommon  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutBMATRIX  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutSTRING  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutLIST  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutLISTarg  __PARAMS((FILE *f, VARPTR var, VARPTR var1, int insidelist, int nel)); 
extern void OutSPARSE  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutIMATRIX  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutPOLYNOM  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutPOINTER  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutSTRINGMAT  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void OutANY  __PARAMS((FILE *f, VARPTR var, int insidelist, int nel)); 
extern void WriteVariableOutput  __PARAMS((FILE *f, VARPTR var, int convert, int insidelist, int nel)); 
void OptMATRIX  __PARAMS(( FILE *f, VARPTR var));
void OptOpointer  __PARAMS(( FILE *f, VARPTR var));
void GetDim  __PARAMS((char *str,IVAR ivar)) ;






