/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2000-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
#include <stdlib.h>


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

/* VAR struct: information for FORTRAN and/or SCILAB variable */


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

/* BASFUN struct: information for SCILAB function */

typedef struct basfun {
  char *name; /* function name */
  int nin; /* number of arguments */
  int maxOpt ; /* number of potential optional arguments in function */
  int NewMaxOpt; /* number of new style optional arguments in function */
  IVAR in[MAXARG]; /* list of argument IVAR */
  IVAR out; /* output IVAR : i.e type of output */
} BASFUN, *BASFUNPTR;

/* FORSUB struct: information for FORTRAN subroutine */

typedef struct forsub {
  char *name; /* subroutine name */
  int narg; /* number of arguments */
  IVAR arg[MAXARG]; /* list of argument IVAR */
} FORSUB, *FORSUBPTR;

/* memory allocators */

/** functions **/


extern VARPTR VarAlloc  (void);
extern BASFUNPTR BasfunAlloc  (void);
extern FORSUBPTR ForsubAlloc  (void);

void WriteInfoCode (FILE *f);
int GetNumberInScilabCall (int ivar);
int GetNumberInFortranCall (int ivar);
char *SGetSciType (int type);
char* SGetCDec (int type);
char *SGetForType (int type);
char *SGetForTypeAbrev (VARPTR var);
char *SGetForTypeStack (VARPTR var);
char *SGetForTypeBConvert (VARPTR var);
void AddForName (IVAR ivar, char *name,char *cname,IVAR ivar1);
void ChangeForName1 (VARPTR var, char *name);
void Copyright (void);
char *Forname2Int (VARPTR,int);
void GenFundef (char *file, int interf);
int GetBasType (char *sname);
int GetForType (char *type);
IVAR GetExistOutVar (void);
IVAR CheckOutVar (void);
IVAR GetExistVar (char *name);
IVAR GetOutVar (char *name);
IVAR GetVar (char *name, int p);
void OptVar ();
int ParseLine (char *s, char **words);
int ParseScilabLine (char *s, char **words);
int ReadListElement (FILE *f, char *varlistname, IVAR iivar, int nel,int);
void ReadListFile (char *listname, char *varlistname, IVAR ivar,int);
int ReadFunction (FILE *f);
int TypeToBas ();
void WriteArgCheck (FILE *f, int i);
void WriteCall ();
void WriteCallRest (FILE *f, IVAR ivar, int farg, char *call);
void WriteCallConvertion (FILE *f, IVAR ivar, char *farg, char *barg, char *call);
void WriteCrossCheck (FILE *f);
void WriteEqualCheck (FILE *f);
void WriteExternalVariableOutput (FILE *f, VARPTR var,  int insidelist, int nel);
void WriteFortranCall (FILE *f);
void WriteFunctionCode (FILE *f);
void WriteHeader (FILE *f, char *fname0, char *fname);
void WriteMainHeader (FILE *f, char *fname);
void WriteListAnalysis (FILE *f, int i,char *);
void WriteOutput (FILE *f);
void WriteVariable (FILE *f, VARPTR var, IVAR ivar, int insidelist, int nel);
void WriteVariableOutput (FILE *f, VARPTR var,  int convert, int insidelist, int nel);
void AddForName1 (IVAR ivar, char *name,char *cname, IVAR ivar1);
void ForNameClean (void);
void InitDeclare  (void);
void ResetDeclare (void);
void WriteMain  (FILE *f,char *file);
void FCprintf(FILE*,char *fmt,...);
void Fprintf(FILE*,int,char *fmt,...);
void white  (FILE *f, int ind);
void AddDeclare (int type, char *declaration);
void InitDeclare (void);
void ResetDeclare (void);
void WriteDeclaration  (FILE*f);
void WriteCallRestCheck (FILE *f, VARPTR var,char *name, int iel, int flag);
int CreatePredefVar (char *name);
void Check (FILE *f,  VARPTR var,  int nel);
void CheckSquare (FILE *f,  VARPTR var, char *str,char *str1);
void CheckOptSquare (FILE *f,  VARPTR var, char *str);
void CheckOptDim (FILE *f,  VARPTR var,  int nel);
void OptvarGetSize (char *optvar, char *size, char *data);
void WriteAddInter (char *file);


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

extern void Generate ( char *file);
extern int ShowVariables  (void);
extern int FixStackPositions  (void);
extern IVAR GetVar  (char *name, int p);
extern IVAR GetExistVar  (char *name);
extern int CreatePredefVar  (char *name);
extern IVAR GetOutVar  (char *name);
extern IVAR GetExistOutVar  (void);
extern void AddForName  (IVAR ivar, char *name, char *cname, IVAR ivar1);
extern void AddForName1  (IVAR ivar, char *name, char *cname, IVAR ivar1);
extern void ForNameClean  (void);
extern void ChangeForName2  (VARPTR var, char *format,...);
extern void ChangeForName1  (VARPTR var, char *name);
extern int GetBasType  (char *sname);
extern char *SGetSciType  (int type);
extern int GetForType  (char *type);
extern char *SGetForType  (int type);
extern char *SGetForTypeAbrev  (VARPTR var);
extern int SGetForDec  (int type);
extern char *SGetCDec  (int type);
extern char *SGetForTypeStack  (VARPTR var);
extern char *SGetForTypeBConvert  (VARPTR var);
extern char *SGetExtForTypeAbrev  (VARPTR var);
extern char *SGetExtForTypeStack  (VARPTR var);
extern void StrGen  (char *strl, VARPTR var);
extern void ForMATRIX  (VARPTR var);
extern void ForSTRING  (VARPTR var);
extern void ForIMATRIX  (VARPTR var);
extern void ForSPARSE  (VARPTR var);
extern void ForROW  (VARPTR var);
extern void ForCOLUMN  (VARPTR var);
extern void ForVECTOR  (VARPTR var);
extern void ForPOLYNOM  (VARPTR var);
extern void ForSCALAR  (VARPTR var);
extern void ForPOINTER  (VARPTR var);
extern void ForANY  (VARPTR var);
extern void ForLIST  (VARPTR var);
extern void ForTLIST  (VARPTR var);
extern void ForSEQUENCE  (VARPTR var);
extern void ForEMPTY  (VARPTR var);
extern void ForWORK  (VARPTR var);
extern void ForDIMFOREXT  (VARPTR var);
extern int main  (int argc, char **argv);
extern void Generate  (char *file);
extern int FixStackPositions  (void);
extern void FixForNames  ();
extern void WriteMain  (FILE *f, char *file);
extern void WriteAddInter  (char *file);
extern void Copyright  (void);
extern void WriteHeader  (FILE *f, char *fname0, char *fname);
extern void WriteFunctionCode  (FILE *f);
extern void WriteOptArgPhase0  (FILE *f, int i);
extern void WriteOptArgPhase1  (FILE *f, int i);
extern void WriteOptArgPhase2  (FILE *f, int i);

extern void WriteInfoCode  (FILE *f);
extern void WriteArgCheck  (FILE *f, int i);
extern void WriteOptArg  (FILE *f , VARPTR var);
extern void WriteCrossCheck  (FILE *f);
extern void WriteCrossCheckExternal (FILE *f);
extern void WriteEqualCheck  (FILE *f);
extern void WriteListAnalysis  (FILE *f, int i, char *list_type);
extern void AddDeclare1 (int type,char *format,...);

extern void WriteFortranCall  (FILE *f);
extern void WriteOutput  (FILE *f);
extern void WriteVariable  (FILE *f, VARPTR var, IVAR ivar, int insidelist, int nel);
extern int GetNumberInScilabCall  (int ivar);
extern int GetNumberInFortranCall  (int ivar);
extern char *Forname2Int  (VARPTR var, int i);
extern void GenFundef  (char *file, int interf);
extern void white  (FILE *f, int ind);
extern VARPTR VarAlloc  (void);
extern BASFUNPTR BasfunAlloc  (void);
extern FORSUBPTR ForsubAlloc  (void);
extern int ReadFunction  (FILE *f);
extern int ParseScilabLine  (char *s, char **words);
extern int ParseLine  (char *s, char **words);
extern void ReadListFile  (char *listname, char *varlistname, IVAR ivar, int stack_position);
extern int ReadListElement  (FILE *f, char *varlistname, IVAR iivar, int nel, int stack_position);

extern int ShowVariables  (void);
extern int FixStackPositions  (void);
extern IVAR GetVar  (char *name, int p);
extern IVAR GetExistVar  (char *name);
extern int CreatePredefVar  (char *name);
extern IVAR GetOutVar  (char *name);
extern IVAR GetExistOutVar  (void);
extern void AddForName  (IVAR ivar, char *name, char *cname, IVAR ivar1);
extern void AddForName1  (IVAR ivar, char *name, char *cname, IVAR ivar1);
extern void ForNameClean  (void);
extern void ChangeForName1  (VARPTR var, char *name);
extern int GetBasType  (char *sname);
extern char *SGetSciType  (int type);
extern int GetForType  (char *type);
extern char *SGetForType  (int type);
extern char *SGetForTypeAbrev  (VARPTR var);
extern int SGetForDec  (int type);
extern char *SGetCDec  (int type);
extern char *SGetForTypeStack  (VARPTR var);
extern char *SGetForTypeBConvert  (VARPTR var);
extern char *SGetExtForTypeAbrev  (VARPTR var);
extern char *SGetExtForTypeStack  (VARPTR var);
extern void OutCOLUMN  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutROW  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutVECTOR  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutMATRIX  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutSCALAR  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutCommon  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutBMATRIX  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutSTRING  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutLIST  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutLISTarg  (FILE *f, VARPTR var, VARPTR var1, int insidelist, int nel);
extern void OutSPARSE  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutIMATRIX  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutPOLYNOM  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutPOINTER  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutSTRINGMAT  (FILE *f, VARPTR var, int insidelist, int nel);
extern void OutANY  (FILE *f, VARPTR var, int insidelist, int nel);
extern void WriteVariableOutput  (FILE *f, VARPTR var, int convert, int insidelist, int nel);
void OptMATRIX  ( FILE *f, VARPTR var);
void OptOpointer  ( FILE *f, VARPTR var);
void GetDim  (char *str,IVAR ivar) ;







