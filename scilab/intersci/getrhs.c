#include <stdlib.h>
#include "intersci-n.h"
#include "getrhs.h"

/*****************************************************************
 * For each possible data type we have here a function 
 * [1] which generate the code for <<Getting>> a variable 
 * and checking some properties 
 *     if needed check if the variable is square 
 *     if needed check if some dimensions must be of fixed sizes
 * [2] generate the code for type convertion if needed
 * [3] computes the string for the C or Fortran calling sequence 
 *     which is stored in the variable structure 
 * All the possible Getfunction are stored in a function table 
 ******************************************************************/

/******************************************************
 * the functions in the following table must follow  the 
 * order given by the type list defined in  intersci-n.h 
 * The correct ordering is checked when using this table.
 * (see intersci.c) 
 ********************************************************/

GetRhsTab RHSTAB[] = {
  {DIMFOREXT,GetDIMFOREXT},
  {COLUMN,GetCOLUMN},
  {LIST,GetLIST},
  {TLIST,GetTLIST},
  {MATRIX,GetMATRIX},
  {POLYNOM,GetPOLYNOM},
  {ROW,GetROW},
  {SCALAR,GetSCALAR},
  {SEQUENCE,GetSEQUENCE},
  {STRING,GetSTRING},
  {WORK,GetWORK},
  {EMPTY,GetEMPTY},
  {ANY,GetANY},
  {VECTOR,GetVECTOR},
  {STRINGMAT,GetSTRINGMAT},
  {SCIMPOINTER,GetPOINTER},
  {IMATRIX,GetIMATRIX},
  {SCISMPOINTER,GetPOINTER},
  {SCILPOINTER,GetPOINTER},
  {BMATRIX,GetBMATRIX},
  {SCIBPOINTER,GetPOINTER},
  {SCIOPOINTER,GetPOINTER},
  {SPARSE,GetSPARSE}
};

extern  int indent ; /* incremental counter for code indentation */
extern  int pass;  /* flag for couting pass on code generation */

static  char str[MAXNAM];
static  char str1[MAXNAM];
static  char str2[MAXNAM];

/***********************************************
 * Matrix OK
 *   flag is used for optional variables 
 *   f(..... x=val) 
 ***********************************************/

void GetMATRIX(f,var,flag) 
     FILE *f;   VARPTR var ;int flag;
{
  GetCom(f,var,flag);
  /** str1 was set by GetCom */
  CheckSquare(f,var,str1,str2); 
  Check(f,var,0);
  Check(f,var,1);
}

/** common function for different data types */

void GetCom(f,var,flag) 
     FILE *f;   VARPTR var ;int flag;
{
  static char C[]="GetRhsVar(%s,\"%s\",&m%d,&n%d,&l%d);\n";
  static char LC[]="GetListRhsVar(%s,%d,\"%s\",&m%s,&n%s,&l%s);\n";
  int i1= var->stack_position;
  if ( flag == 1 ) 
    sprintf(str2,"k");
  else
    sprintf(str2,"%d",i1);
  if (var->list_el ==0 )
    {
      /** A scilab matrix argument **/
      sprintf(str1,"%d",i1);
      Fprintf(f,indent,C,str2,SGetForTypeAbrev(var),i1,i1,i1);
      /* Adding the calling sequence in the for_names  */ 
      ChangeForName2(var,"%s(l%s)",SGetForTypeStack(var),str1);
    }
  else
    {
      /** A scilab matrix argument inside a list **/
      sprintf(str1,"%de%d",i1,var->list_el);
      Fprintf(f,indent,LC,str2,var->list_el,SGetForTypeAbrev(var),str1,str1,str1,str1);
      /* Adding the calling sequence in the for_names  */ 
      ChangeForName2(var,"%s(l%s)",SGetForTypeStack(var),str1);
    }
  AddDeclare1(DEC_INT,"m%s",str1);
  AddDeclare1(DEC_INT,"n%s",str1);
  AddDeclare1(DEC_INT,"l%s",str1);
}




/***********************************************
 * STRING : OK
 ***********************************************/
  
void GetSTRING(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  if (var->for_type != CHAR) 
    {
      printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
	     SGetSciType(STRING),SGetForType(var->for_type),var->name);
      exit(1);
    }
  GetCom(f,var,flag);
  Check(f,var,0);
}

/***********************************************
 * Boolean matrix OK
 ***********************************************/


void GetBMATRIX(f,var,flag) 
     FILE *f;   VARPTR var ;int flag;
{
  if (var->for_type != INT && var->for_type != BOOLEAN)
    {
      printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
	     SGetSciType(var->type),SGetForType(var->for_type),var->name);
      exit(1);
    }
  var->for_type = BOOLEAN;
  GetCom(f,var,flag);
  /** str1 was set by GetCom */
  CheckSquare(f,var,str1,str2); 
  Check(f,var,0);
  Check(f,var,1);
}

/***********************************************
 * Complex Matrix 
 ***********************************************/

void GetIMATRIX(f,var,flag) 
     FILE *f;   VARPTR var ;int flag;
{
  static char C[]="GetRhsCVar(%s,\"%s\",&it%d,&m%d,&n%d,&lr%d,&lc%d);\n";
  static char LC[]="GetListRhsCVar(%s,%d,\"%s\",&it%s,&m%s,&n%s,&lr%s,&lc%s,&lar%s,&lac%s);\n";
  int i1= var->stack_position;
  if ( flag == 1 ) 
    sprintf(str2,"k");
  else
    sprintf(str2,"%d",i1);
  if (var->list_el ==0 )
    {
      /** A scilab matrix argument **/
      sprintf(str1,"%d",i1);
      Fprintf(f,indent,C,str2,SGetForTypeAbrev(var),
	      i1,i1,i1,i1,i1);
      /* Adding the calling sequence in the for_names  */ 
      ChangeForName2(var,"%s(lr%s),%s(lc%s),&it%s",
		     SGetForTypeStack(var),str1,
		     SGetForTypeStack(var),str1,str1);
    }
  else
    {
      /** A scilab matrix argument inside a list **/
      sprintf(str1,"%de%d",i1,var->list_el);
      AddDeclare1(DEC_INT,"lar%s",str1);
      AddDeclare1(DEC_INT,"lac%s",str1);
      Fprintf(f,indent,LC,str2,var->list_el,SGetForTypeAbrev(var),
	      str1,str1,str1,str1,str1,str1,str1);
      /* Adding the calling sequence in the for_names  */ 
      ChangeForName2(var,"%s(lr%s),%s(lc%s),&it%s",
		     SGetForTypeStack(var),str1,
		     SGetForTypeStack(var),str1,str1);
    }
  
  AddDeclare1(DEC_INT,"m%s",str1);
  AddDeclare1(DEC_INT,"n%s",str1);
  AddDeclare1(DEC_INT,"lr%s",str1);
  AddDeclare1(DEC_INT,"lc%s",str1);
  AddDeclare1(DEC_INT,"it%s",str1);

  /** str1 was set by GetCom */
  CheckSquare(f,var,str1,str2); 
  Check(f,var,0);
  Check(f,var,1);
}


/***********************************************
 * Sparse Matrix 
 ***********************************************/

void GetSPARSE(f,var,flag) 
     FILE *f;   VARPTR var ;int flag;
{
  static char C[]="GetRhsVar(%s,\"s\",&m%d,&n%d,&S%d);\n";
  static char LC[]="GetListRhsVar(%s,%d,\"s\",&m%s,&n%s,&S%s);\n";
  int i1= var->stack_position;
  if ( flag == 1 ) 
    sprintf(str2,"k");
  else
    sprintf(str2,"%d",i1);
  if (var->list_el ==0 )
    {
      /** A scilab matrix argument **/
      sprintf(str1,"%d",i1);
      Fprintf(f,indent,C,str2, i1,i1,i1);
      /* Adding the calling sequence in the for_names  */ 
      ChangeForName2(var,"&S%d",i1);
    }
  else
    {
      /** A scilab matrix argument inside a list **/
      sprintf(str1,"%de%d",i1,var->list_el);
      Fprintf(f,indent,LC,str2,var->list_el, str1,str1,str1);
      /* Adding the calling sequence in the for_names  */ 
      ChangeForName2(var,"&S%s",str1);
    }

  AddDeclare1(DEC_INT,"m%s",str1);
  AddDeclare1(DEC_INT,"n%s",str1);
  AddDeclare1(DEC_SPARSE,"S%s",str1);

  /** str1 was set by GetCom */
  CheckSquare(f,var,str1,str2); 
  Check(f,var,0);
  Check(f,var,1);
}




/***********************************************
 * Stringmat 
 ***********************************************/

void GetSTRINGMAT(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  if ( flag == 1 ) 
    sprintf(str2,"k");
  else
    sprintf(str2,"%d",i1);
  if (var->list_el ==0 )
    {
      AddDeclare1(DEC_SMAT,"Str%d",i1);
      AddDeclare1(DEC_INT,"m%d",i1);
      AddDeclare1(DEC_INT,"n%d",i1);
      Fprintf(f,indent,"GetRhsVar(%s,\"S\",&m%d,&n%d,&Str%d);\n",str2,i1,i1,i1);
      sprintf(str,"&Str%d",i1);
      ChangeForName1(var,str);
    }
  else
    {
      sprintf(str1,"%de%d",i1,var->list_el);
      AddDeclare1(DEC_SMAT,"Str%s",str1);
      AddDeclare1(DEC_INT,"m%s",str1);
      AddDeclare1(DEC_INT,"n%s",str1);
      Fprintf(f,indent,"GetListRhsVar(%s,%d,\"S\",&m%s,&n%s,&Str%s);\n",str2,var->list_el,
	      str1,str1,str1);
      sprintf(str,"&Str%s",str1);
      ChangeForName1(var,str);
    }
  /* square matrix */
  CheckSquare(f,var,str1,str2); 
  Check(f,var,0);
  Check(f,var,1);
  if (var->for_type != CSTRINGV) 
    {
      printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
	     SGetSciType(STRINGMAT),SGetForType(var->for_type),var->name);
      exit(1);
    }
}

/***********************************************
 * Row 
 ***********************************************/

void GetROW(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  GetCom(f,var,flag);
  Check(f,var,0);
  if (var->list_el ==0 )
    {
      Fprintf(f,indent,"CheckRow(%d,m%d,n%d);\n",i1,i1,i1);
      Fprintf(f,indent,"mn%d=m%d*n%d;\n",i1,i1,i1);
      AddDeclare1(DEC_INT,"mn%d",i1);
    }
  else 
    {
      sprintf(str1,"%de%d",i1,var->list_el);
      Fprintf(f,indent,"CheckListRow(%d,%d,m%s,n%s);\n",i1,var->list_el,str1,str1);
      Fprintf(f,indent,"l%dmn%d=m%s*n%s;\n",i1,var->list_el,str1,str1);
      AddDeclare1(DEC_INT,"l%dmn%d",i1,var->list_el);
    }
}


/***********************************************
 * Column 
 ***********************************************/

void GetCOLUMN(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  GetCom(f,var,flag);
  Check(f,var,0);
  if (var->list_el ==0 )
    {
      Fprintf(f,indent,"CheckColumn(%d,m%d,n%d);\n",i1,i1,i1);
      Fprintf(f,indent,"mn%d=m%d*n%d;\n",i1,i1,i1);
      AddDeclare1(DEC_INT,"mn%d",i1);
    }
  else 
    {
      sprintf(str1,"%de%d",i1,var->list_el);
      Fprintf(f,indent,"CheckListColumn(%d,%d,m%s,n%s);\n",i1,var->list_el,str1,str1);
      Fprintf(f,indent,"l%dmn%d=m%s*n%s;\n",i1,var->list_el,str1,str1);
      AddDeclare1(DEC_INT,"l%dmn%d",i1,var->list_el);
    }
}

/***********************************************
 * Vector 
 ***********************************************/

void GetVECTOR(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  GetCom(f,var,flag);
  Check(f,var,0);
  if (var->list_el ==0 )
    {
      Fprintf(f,indent,"CheckVector(%d,m%d,n%d);\n",i1,i1,i1);
      Fprintf(f,indent,"mn%d=m%d*n%d;\n",i1,i1,i1);
      AddDeclare1(DEC_INT,"mn%d",i1);
    }
  else 
    {
      sprintf(str1,"%de%d",i1,var->list_el);
      Fprintf(f,indent,"CheckListVector(%d,%d,m%s,n%s);\n",i1,var->list_el,str1,str1);
      Fprintf(f,indent,"l%dmn%d=m%s*n%s;\n",i1,var->list_el,str1,str1);
      AddDeclare1(DEC_INT,"l%dmn%d",i1,var->list_el);
    }
}

/***********************************************
 * Polynom 
 ***********************************************/

void GetPOLYNOM(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  if (var->list_el ==0 )
    {
      sprintf(str1,"%d",i1);
      AddDeclare(DEC_LOGICAL,"getonepoly");
      AddDeclare(DEC_CHAR,str);
      Fprintf(f,indent,"if(.not.getonepoly(fname,top,top-rhs+%d,it%d,m%d,namelr%d,namellr%d,lr%d,lc%d)\n",i1,i1,i1,i1,i1,i1,i1);
    }
  else
    {
      sprintf(str1,"%de%d",i1,var->list_el);
      AddDeclare(DEC_LOGICAL,"getlistpoly");
      Fprintf(f,indent,"if(.not.getlistpoly(fname,topk,top-rhs+%d,%d,it%s,m%s,n%s,name%s,\n",
	      i1,var->list_el,str1,str1,str1,str1);
      Fprintf(f,indent,"$     namel%s,ilp%s,lr%s,lc%s)\n",str1,str1,str1,str1);
    }
  Check(f,var,0);
  /* Convertion */ 
  switch (var->for_type) 
    {
    case INT:
      Fprintf(f,indent,"call entier(n%s,stk(lr%s),istk(iadr(lr%s)))\n",
	      str1,str1,str1);
      sprintf(str,"istk(iadr(lr%s))",str1);
      ChangeForName1(var,str);
      break;
    case REAL:
      Fprintf(f,indent,"call simple(n%s,stk(lr%s),stk(lr%s))\n",
	      str1,str1,str1);
      sprintf(str,"stk(lr%s)",str1);
      ChangeForName1(var,str);
      break;
    case DOUBLE:
      sprintf(str,"stk(lr%s)",str1);
      ChangeForName1(var,str);
      break;
    default: 
      printf("incompatibility between Scilab and Fortran type for variable \"%s\"\n",
	     var->name);
      exit(1);
    }
}

/***********************************************
 * Scalar 
 ***********************************************/

void GetSCALAR(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  GetCom(f,var,flag);
  /* Check(f,var,0); */
  if (var->list_el ==0 )
    {
      Fprintf(f,indent,"CheckScalar(%d,m%d,n%d);\n",i1,i1,i1);
    }
  else 
    {
      sprintf(str1,"%de%d",i1,var->list_el);
      Fprintf(f,indent,"CheckListScalar(%d,%d,m%s,n%s);\n",i1,var->list_el,str1,str1);
    }
}

/***********************************************
 * Pointers 
 ***********************************************/

void GetPOINTER(f,var,flag) 
     FILE *f;   VARPTR var ;int flag;
{
  static char C[]="GetRhsOPointer(%s,&lr%d);\n";
  int i1= var->stack_position;
  if ( flag == 1 ) 
    sprintf(str2,"k");
  else
    sprintf(str2,"%d",i1);
  sprintf(str1,"%d",i1);
  if (var->list_el ==0 )
    {
      /** A scilab matrix argument **/
      sprintf(str1,"%d",i1);
      Fprintf(f,indent,C,str2,i1);
      /* Adding the calling sequence in the for_names  */ 
      ChangeForName2(var,"stk(lr%s)",str1);
    }
  else
    {
      fprintf(stderr,"Wrong type opointer inside a list \n");
      exit(1);
    }
  AddDeclare1(DEC_INT,"lr%s",str1);
  
}


void GetANY(f,var,flag) FILE *f;   VARPTR var ;int flag;{
  fprintf(stderr,"Wrong type in Get function \n");
  exit(1);
}

void GetLIST(f,var,flag) FILE *f;   VARPTR var ;int flag;{
  fprintf(stderr,"Wrong type in Get function \n");
  exit(1);
}

void GetTLIST(f,var,flag) FILE *f;   VARPTR var ;int flag;{
  fprintf(stderr,"Wrong type in Get function \n");
  exit(1);
}

void GetSEQUENCE(f,var,flag) FILE *f;   VARPTR var ;int flag;
{  
  fprintf(stderr,"Wrong type in Get function \n");
  exit(1);
};

void GetEMPTY(f,var,flag)  FILE *f;   VARPTR var ;int flag;
{  
  fprintf(stderr,"Wrong type in Get function \n");
  exit(1);
};

void GetWORK(f,var,flag)  FILE *f;   VARPTR var ;int flag;
{  
  fprintf(stderr,"Wrong type in Get function \n");
  exit(1);
};


void GetDIMFOREXT(f,var,flag)  FILE *f;   VARPTR var ;int flag;
{  
  fprintf(stderr,"Wrong type in Get function \n");
  exit(1);
};

/***************************************
 * Utility function for the Getfunctions
 * Check for fixed sized dimensions 
 ***************************************/

void Check(f,var,nel)
     FILE *f;
     int nel;
     VARPTR var;
{
  VARPTR var1 = variables[var->el[nel]-1];
  if ( var1->nfor_name == 0) 
    {
      fprintf(stderr,"Pb with element number %d [%s] of variable %s\n",
	      nel+1, var1->name, var->name);
      return;
    }
  if (isdigit(var1->name[0]) != 0) 
    {
      /* the dimension of the variable is a constant integer */
      if ( strcmp(var1->for_name[0],var1->name) != 0) 
	{
	  if (var->list_el ==0 )
	    {
	      Fprintf(f,indent,"CheckOneDim(%d,%d,%s,%s);\n",
		      var->stack_position,
		      nel+1,
		      var1->for_name[0],var1->name);
	    }
	  else
	    {
	      Fprintf(f,indent,"CheckListOneDim(%s,%d,%d,%s,%s);\n",
		      str2,
		      var->list_el,
		      nel+1,
		      var1->for_name[0],var1->name);
	    }
	}
    }
}




void CheckSquare(f,var,str1,str2)
     FILE *f;
     VARPTR var;
     char *str1;
     char *str2;
{
  if (var->el[0] != var->el[1]) return ;
  if (var->list_el ==0 )
    {
      Fprintf(f,indent,"CheckSquare(%d,m%s,n%s);\n", var->stack_position,str1,str1);
    }
  else 
    {
      Fprintf(f,indent,"CheckListSquare(%s,%d,m%s,n%s);\n",str2, var->list_el,str1,str1);
    }  
}







