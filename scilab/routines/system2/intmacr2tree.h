/**************************************************/
/* intmacr2tree.h                                 */
/* Functions used for macr2tree() Scilab function */
/* Copyright INRIA                                */
/* V.C. - 2004                                    */
/**************************************************/

/* Generic header files */
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../machine.h"
#include "../stack-c.h" /* stack-def.h, stack1.h, stack2.h, stack3.h included */

/**********************/
/* External functions */
/**********************/

/* Defined in SCI/routines/scicos/str2sci.c */
extern void str2sci(char** x,int n,int m); /* Write a C-string in Scilab stack */

/* Defined in SCI/routines/scicos/itosci.f */
extern void C2F(itosci)(int *intptr,int *nbrows,int *nbcols); /* Write a int in Scilab stack */

/* Defined in SCI/routines/scicos/dtosci.f */
extern void C2F(dtosci)(double *dbleptr,int *nbrows,int *nbcols); /* Write a double in Scilab stack */

/* Defined in SCI/routines/system/cvname.f */
extern int C2F(cvnamel)(int *id,char *str,int *jobptr,int *str_len); 
/* *jobptr==0: Get Scilab codes from C-string */
/* *jobptr==1: Get C-string from Scilab codes */

/* Defined in SCI/routines/default/Funtab.c */
extern int C2F(funtab)(int *id, int *fptr, int *job); 
/* *job==1: Find id-name associated to function pointer */
/* *job==2: Find function pointer associated to id-name */

/* Defined in SCI/routines/system/mklist.f */
extern int C2F(mklist)(int *nbitems); 
extern int C2F(mktlist)(int *nbitems); 

/* Some useful definitions unexistant in stack-c.h */
#define idstk(x,y) (C2F(vstk).idstk+(x-1)+(y-1)*nsiz)
#define CvNameL(id,str,jobptr,str_len) C2F(cvnamel)(id,str,jobptr,str_len);
#define CvStr(n,line,str,jobptr,str_len) C2F(cvstr)(n,line,str,jobptr,str_len);

/**********************/
/* Internal functions */
/**********************/

/****************************************************************
 Function name: macr2tree
 Decription:
 Create on Scilab stack a "tree" composed of imbricated lists
 and tlists using Scilab pseudo codes of Scilab macro
 Resulting "tree" is a new representation for Scilab macro
 Particularly useful for M2SCI
 Tree is emuled by a "program" tlist:
  tlist(["program","name","outputs","inputs","statements","nblines"],..
                   macro_name,..
                           list_of_output_variable_tlists,..
                                     list_of_intput_variable_tlists,..
                                             list_of_macro_statements,..
                                                           total_number_of_lines_in_macro)
 Input:
  - fname: name of calling function for error message
 Output:
  - No output
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails
****************************************************************/
int C2F(macr2tree) _PARAMS((char *fname));

/****************************************************************
 Function name: CreateVariableTList
 Decription:
 Create on Scilab stack a "variable" tlist:
  tlist(["variable","name"],variable_name)
 Input:
  - varname: pointer to a char array
    varname[0]=pointer to variable name (ASCII character string)
 Output:
  - No output
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails
****************************************************************/
static int CreateVariableTList(char **varname);

/****************************************************************
 Function name: CreateEOLList
 Decription:
 Create on Scilab stack a "EOL" list:
  list("EOL")
 Input:
  - No input
 Output:
  - No output
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails
****************************************************************/
static int CreateEOLList(void);

/****************************************************************
 Function name: AddVar
 Decription:
 Add a variable name in variable name table called varnames
 (varnames is global variable for file intmacr2tree.c)
 Input:
  - name: name of variable to add to table
 Output:
  - No output
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails
****************************************************************/
static int AddVar(char *name);

/****************************************************************
 Function name: IsDefinedVar
 Decription:
 Search for a variable name in variable name table called "varnames"
 ("varnames" is global variable for file intmacr2tree.c)
 Input:
  - name: name of variable to add to table
 Output:
  - No output
 Returned value:
  - -1 if variable not does not exists in "varnames"
  - if variable already exists in "varnames", returned value is 
    equal to index where variable was find
****************************************************************/
static int IsDefinedVar(char *name);

/****************************************************************
 Function name: GetInstruction
 Decription: 
 Create on Scilab stack an INSTRUCTION tlist:
      tlist(["equal","expression","lhs"],...)
   OR tlist(["for","expression","statements"],...)
   OR tlist(["while","expression","statements"],...)
   OR tlist(["selectcase","expression","cases","else"],...)
   OR tlist(["ifthenelse","expression","then","elseifs","else"],...)
 See CreateEqualTList and GetControlInstruction
 Input:
  - data: pointer to compiled macro code
  - index: index of current integer in data
  - nblines: pointer to number of lines in macro
 Output:
  - nblines: pointer to number of lines in macro
  - addinstr:
     0 if just a part of statement (For example "variable")
     1 if statement to add to list (For example "equal")
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails
****************************************************************/
static int GetInstruction(int *data,int *index,int *nblines,int *addinstr);

/****************************************************************
 Function name: GetControlInstruction
 Decription: 
 Create on Scilab stack an CONTROL INSTRUCTION tlist:
      tlist(["for","expression","statements"],...)
   OR tlist(["while","expression","statements"],...)
   OR tlist(["selectcase","expression","cases","else"],...)
   OR tlist(["ifthenelse","expression","then","elseifs","else"],...)
 Input:
  - data: pointer to compiled macro code
  - index: index of current integer in data
  - nblines: pointer to number of lines in macro
 Output:
  - nblines: pointer to number of lines in macro
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails
****************************************************************/
static int GetControlInstruction(int *data,int *index,int *nblines);

/****************************************************************
 Function name: CreateCsteTList
 Decription: 
 Create on Scilab stack a "cste" tlist:
   tlist(["cste","value"],constant_value)
 Input:
  - type: type of constant ("number","string","code23" or "emptymatrix")
  - data: pointer to compiled macro code
  - index: index of current integer in data
 Output:
  - No output
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails
****************************************************************/
static int CreateCsteTList(char *type,int *data,int *index);

/****************************************************************
 Function name: CreateOperationTList
 Decription: 
 Create on Scilab stack an "operation" tlist:
   tlist(["operation","operands","operator"],list_of_operands,operator_symbol)
 Input:
  - data: pointer to compiled macro code
  - index: index of current integer in data
 Output:
  - No output
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails
****************************************************************/
static int CreateOperationTList(int *data,int *index);

/****************************************************************
 Function name: CreateFuncallTList
 Decription: 
 Create on Scilab stack a "funcall" tlist:
   tlist(["funcall","rhs","name","lhsnb"],function_name,list_of_rhs,list_of_lhs,number_of_lhs)
 Input:
  - fromwhat: from what it has to be made ("funptr","datacode" or "macro")
  - data: pointer to compiled macro code
  - index: index of current integer in data
 Output:
  - No output
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails
****************************************************************/
static int CreateFuncallTList(char *fromwhat,int *data,int *index);

/****************************************************************
 Function name: CreateEqualTList
 Decription: 
 Create on Scilab stack an "equal" tlist:
   tlist(["equal","expression","lhs"],left_part_of_equal,list_of_lhs)
 Input:
  - fromwhat: from what it has to be made ("code29","code1" or "forexpr")
  - data: pointer to compiled macro code
  - index: index of current integer in data
 Output:
  - No output
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails
****************************************************************/
static int CreateEqualTList(char *fromwhat,int *data,int *index);

/****************************************************************
 Function name: CreateRecursiveIndexList
 Decription: 
  Create on Scilab stack a list for recursive insertion or extraction
  First list item is a matrix which contains number of row/column indexes
 Input:
  - data: pointer to compiled macro code
  - index: index of current integer in data
 Output:
  - No output
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails
****************************************************************/

static int CreateCommentTList(int *data,int *index);
/****************************************************************
 Function name:  CreateCommentTList
 Decription: 
  Create on Scilab stack a "comment" tlist:
  tlist(["comment","text"],text_of_comment)
 Input:
  - data: pointer to compiled macro code
  - index: index of current integer in data
 Output:
  - No output
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails

****************************************************************/
static int CreateRecursiveIndexList(int *data,int *index);

/****************************************************************

 Function name: VCopyObj
 Decription: 
  Copy an object in Scilab stack
 Input:
  - fname: name of calling function for error message
  - orig: position of object to copy
  - dest: position where object has to be copied
  - fname_length: length of character string fname
 Output:
  - No output
 Returned value:
  - 0 if execution succeeds
  - not null if execution fails
****************************************************************/
static int VCopyObj(char *fname,int *orig,int *dest,unsigned long fname_length);

/****************************************************************
 Function name: complexity
 Decription: 
  Eval number of statements (which give addinstr==1 in GetInstruction)
  in a compiled macro code or a part of this code
 Input:
  - data: pointer to compiled macro code
  - index: index of current integer in data
  - lgth: length of code to eval
 Output:
  - No output
 Returned value:
  - number of list items corresponding to code
****************************************************************/
int complexity(int *data,int *index,int *lgth);
