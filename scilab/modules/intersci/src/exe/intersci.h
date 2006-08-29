/* VERSION et DATE */
#define VERSION "2.2"
#define DATE "MAY 1994" 

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

/* Fortran Declaration */

#define DEC_CHAR 1
#define DEC_INT 2
#define DEC_DOUBLE 3
#define DEC_REAL 4
#define DEC_LOGICAL  5
#define DEC_DATA  6

/* SCILAB variable types */

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

/* SCILAB optional variable types */

#define NAME 1 /* {var default-name} */
#define VALUE 2 /* {var default-value} */

typedef int IVAR; /* variable number */

/* VAR struct: informations for FORTRAN and/or SCILAB variable */

typedef struct var {
  char *name; /* variable name */
  int type; /* SCILAB type */
  char Stype[20]; /* Scilab Type as string */
  int length; /* number of el */
  IVAR el[MAXEL]; /* list of el IVAR (variable associated with,
		     typically dimensions) */
  int for_type; /* FORTRAN type */
  char *fexternal[MAXNAM]; /* name of external function when type is
			      external */
  IVAR equal; /* ? */
  int nfor_name; /* number of for_name */
  int kp_state;  /* for pass dealing **/
  char *for_name[MAXARG]; /* list of for_name names (FORTRAN name
			   in generated FORTRAN code) */
  char *list_name; /* name of the list of which the variable is an element */
  int list_el; /* element number in the previous list */
  int opt_type; /* type of optional variable */
  char *opt_name; /* name or value default for optional variable */
  int present; /* 1 if the variable is really present in the description file
		  0 otherwise
		  used for list elements which might be not really present */
} VAR, *VARPTR;

/* BASFUN struct: informations for SCILAB function */
typedef struct basfun {
  char *name; /* function name */
  int nin; /* number of arguments */
  IVAR in[MAXARG]; /* list of argument IVAR */
  IVAR out; /* output IVAR */
} BASFUN, *BASFUNPTR;

/* FORSUB struct: informations for FORTRAN subroutine */
typedef struct forsub {
  char *name; /* subroutine name */
  int narg; /* number of arguments */
  IVAR arg[MAXARG]; /* list of argument IVAR */
} FORSUB, *FORSUBPTR;

/* memory allocators */

VARPTR VarAlloc()
{
  return((VARPTR) malloc(sizeof(VAR)));
}

BASFUNPTR BasfunAlloc()
{
  return((BASFUNPTR) malloc(sizeof(BASFUN)));
}

FORSUBPTR ForsubAlloc()
{
  return((FORSUBPTR) malloc(sizeof(FORSUB)));
}

/* global variables */

VARPTR variables[MAXVAR]; /* array of VAR structures */
int nVariable; /* number of variables */
BASFUNPTR basfun; /* SCILAB function structure */
FORSUBPTR forsub; /* FORTRAN subroutine structure */
int nFun; /* total number of functions in "desc" file */
int maxOpt; /* maximal number of optional variables */
char *funNames[MAXFUN]; /* array of function names */

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

void WriteInfoCode __PARAMS((FILE *f));
int GetNumberInScilabCall __PARAMS((int ivar));
int GetNumberInFortranCall __PARAMS((int ivar));
char *SGetSciType __PARAMS((int type));
char *SGetForType __PARAMS((int type));
void AddForName __PARAMS((IVAR ivar, char *name));
void ChangeForName __PARAMS((IVAR ivar, char *name));
void Copyright __PARAMS((void));
char *Forname2Int __PARAMS((char *str));
void GenFundef __PARAMS((char *file, int interf));
int GetBasType __PARAMS((char *sname));
int GetForType __PARAMS((char *type));
IVAR GetExistOutVar __PARAMS((void));
IVAR GetExistVar __PARAMS((char *name));
IVAR GetOutVar __PARAMS((char *name));
IVAR GetVar __PARAMS((char *name, int p));
void OptVar __PARAMS(());
int ParseLine __PARAMS((char *s, char **words));
int ParseScilabLine __PARAMS((char *s, char **words));
int ReadListElement __PARAMS((FILE *f, char *varlistname, IVAR iivar, int nel));
void ReadListFile __PARAMS((char *listname, char *varlistname, IVAR ivar));
void ISCIReadFile __PARAMS((char *file));
int ReadFunction __PARAMS((FILE *f));
int TypeToBas __PARAMS(());
void WriteArgCheck __PARAMS((FILE *f, int i));
void WriteCall __PARAMS(());
void WriteCallRest __PARAMS((FILE *f, IVAR ivar, int farg, char *call));
void WriteCallConvertion __PARAMS((FILE *f, IVAR ivar, char *farg, char *barg, char *call));
void WriteCrossCheck __PARAMS((FILE *f));
void WriteEqualCheck __PARAMS((FILE *f));
void WriteExternalVariableOutput __PARAMS((FILE *f, VARPTR var, int farg, int insidelist, int nel));
void WriteFortranCall __PARAMS((FILE *f));
void WriteFunctionCode __PARAMS((FILE *f));
void WriteHeader __PARAMS((FILE *f, char *fname0, char *fname));
void WriteMainHeader __PARAMS((FILE *f, char *fname));
void WriteListAnalysis __PARAMS((FILE *f, int i));
void WriteOutput __PARAMS((FILE *f));
void WriteVariable __PARAMS((FILE *f, VARPTR var, IVAR ivar, int insidelist, int nel));
void WriteVariableOutput __PARAMS((FILE *f, VARPTR var, int barg, int farg, int convert, int insidelist, int nel));
void AddForName1 __PARAMS((IVAR ivar, char *name));
void ForNameClean __PARAMS((void));
void InitDeclare  __PARAMS((void));
void ResetDeclare __PARAMS((void));
void WriteMain  __PARAMS((FILE *f,char *file));
void  FCprintf __PARAMS((FILE*,char *fmt,...));
void  Fprintf __PARAMS((FILE*,int,char *format,...));
void white __PARAMS( (FILE *f, int ind));
void AddDeclare __PARAMS((int type, char *declaration));
void InitDeclare __PARAMS((void));
void ResetDeclare __PARAMS((void));
void WriteDeclaration  __PARAMS((FILE*f));
void WriteCallRestCheck __PARAMS((FILE *f, VARPTR var, int farg, char *name, int iel, int flag));
int CreatePredefVar __PARAMS((char *name));
void Check __PARAMS((FILE *f, char *str, VARPTR var, int i1, int nel));
void OptvarGetSize __PARAMS((char *optvar, char *size, char *data));
void WriteAddInter __PARAMS((char *file));
