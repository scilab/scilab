#include <stdlib.h>

#include "intersci-n.h"
#include "check.h"



CheckRhsTab CHECKTAB[] = {
  {DIMFOREXT,CheckDIMFOREXT},
  {COLUMN,CheckCOLUMN},
  {LIST,CheckLIST},
  {TLIST,CheckTLIST},
  {MATRIX,CheckMATRIX},
  {POLYNOM,CheckPOLYNOM},
  {ROW,CheckROW},
  {SCALAR,CheckSCALAR},
  {SEQUENCE,CheckSEQUENCE},
  {STRING,CheckSTRING},
  {WORK,CheckWORK},
  {EMPTY,CheckEMPTY},
  {ANY,CheckANY},
  {VECTOR,CheckVECTOR},
  {STRINGMAT,CheckSTRINGMAT},
  {SCIMPOINTER,CheckPOINTER},
  {IMATRIX,CheckIMATRIX},
  {SCISMPOINTER,CheckPOINTER},
  {SCILPOINTER,CheckPOINTER},
  {BMATRIX,CheckBMATRIX},
  {SCIBPOINTER,CheckPOINTER},
  {SCIOPOINTER,CheckPOINTER},
  {SPARSE,CheckSPARSE}
};

extern  int indent ; /* incremental counter for code indentation */
extern  int pass;  /* flag for couting pass on code generation */

static  char str1[MAXNAM];
static  char str2[MAXNAM];

/***********************************************
 * Matrix OK
 *   flag is used for optional variables 
 *   f(..... x=val) 
 ***********************************************/

void CheckMATRIX(f,var,flag) 
     FILE *f;   VARPTR var ;int flag;
{
  CheckCom(f,var,flag);
  /** str1 was set by CheckCom */
  CheckOptSquare(f,var,str1); 
  CheckOptDim(f,var,0);
  CheckOptDim(f,var,1);
}

/** common function for different data types */

void CheckCom(f,var,flag) 
     FILE *f;   VARPTR var ;int flag;
{
  int i1 = var->stack_position - basfun->NewMaxOpt +1 ;
  if ( flag == 1 ) 
    sprintf(str2,"k");
  else
    sprintf(str2,"%d",i1);
  if (var->list_el ==0 )
    {
      /** A scilab matrix argument **/
      sprintf(str1,"%d",i1);
    }
  else
    {
      sprintf(str1,"%de%d",i1,var->list_el);
    }
}




/***********************************************
 * STRING : OK
 ***********************************************/
  
void CheckSTRING(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  if (var->for_type != CHAR) 
    {
      printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
	     SGetSciType(STRING),SGetForType(var->for_type),var->name);
      exit(1);
    }

  CheckCom(f,var,flag);
  CheckOptDim(f,var,0);
}

/***********************************************
 * Boolean matrix OK
 ***********************************************/


void CheckBMATRIX(f,var,flag) 
     FILE *f;   VARPTR var ;int flag;
{
  if (var->for_type != INT && var->for_type != BOOLEAN)
    {
      printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
	     SGetSciType(var->type),SGetForType(var->for_type),var->name);
      exit(1);
    }
  var->for_type = BOOLEAN;
  CheckCom(f,var,flag);
  /** str1 was set by CheckCom */
  CheckOptSquare(f,var,str1); 
  CheckOptDim(f,var,0);
  CheckOptDim(f,var,1);
}

/***********************************************
 * Complex Matrix 
 ***********************************************/

void CheckIMATRIX(f,var,flag) 
     FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  if ( flag == 1 ) 
    sprintf(str2,"k");
  else
    sprintf(str2,"%d",i1);
  if (var->list_el ==0 )
    sprintf(str1,"%d",i1);
  else
    sprintf(str1,"%de%d",i1,var->list_el);
  /** str1 was set by CheckCom */
  CheckOptSquare(f,var,str1); 
  CheckOptDim(f,var,0);
  CheckOptDim(f,var,1);
}


/***********************************************
 * Sparse Matrix 
 ***********************************************/

void CheckSPARSE(f,var,flag) 
     FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  if ( flag == 1 ) 
    sprintf(str2,"k");
  else
    sprintf(str2,"%d",i1);
  if (var->list_el ==0 )
    {
      sprintf(str1,"%d",i1);
    }
  else
    {
      sprintf(str1,"%de%d",i1,var->list_el);
    }
  /** str1 was set by CheckCom */
  CheckOptSquare(f,var,str1); 
  CheckOptDim(f,var,0);
  CheckOptDim(f,var,1);
}


/***********************************************
 * Stringmat 
 ***********************************************/

void CheckSTRINGMAT(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  if (var->list_el ==0 )
    {
      sprintf(str1,"%d",i1);
    }
  else
    {
      sprintf(str1,"%de%d",i1,var->list_el);
    }
  /* square matrix */
  CheckOptSquare(f,var,str1); 
  CheckOptDim(f,var,0);
  CheckOptDim(f,var,1);
}

/***********************************************
 * Row 
 ***********************************************/

void CheckROW(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  CheckCom(f,var,flag);
  CheckOptDim(f,var,0);
  Fprintf(f,indent,"CheckRow(%d,m%d,n%d);\n",i1,i1,i1);
  Fprintf(f,indent,"mn%d=m%d*n%d;\n",i1,i1,i1);
  AddDeclare1(DEC_INT,"mn%d",i1);
}


/***********************************************
 * Column 
 ***********************************************/

void CheckCOLUMN(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  CheckCom(f,var,flag);
  CheckOptDim(f,var,0);
  Fprintf(f,indent,"CheckColumn(%d,m%d,n%d);\n",i1,i1,i1);
  Fprintf(f,indent,"mn%d=m%d*n%d;\n",i1,i1,i1);
  AddDeclare1(DEC_INT,"mn%d",i1);
}

/***********************************************
 * Vector 
 ***********************************************/

void CheckVECTOR(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  CheckCom(f,var,flag);
  CheckOptDim(f,var,0);
  Fprintf(f,indent,"CheckVector(%d,m%d,n%d);\n",i1,i1,i1);
  Fprintf(f,indent,"mn%d=m%d*n%d;\n",i1,i1,i1);
  AddDeclare1(DEC_INT,"mn%d",i1);
}

/***********************************************
 * Polynom 
 ***********************************************/

void CheckPOLYNOM(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  if ( flag == 1 ) 
    sprintf(str2,"k");
  else
    sprintf(str2,"%d",i1);
  if (var->list_el ==0 )
    {
      sprintf(str1,"%d",i1);
    }
  else
    {
      sprintf(str1,"%de%d",i1,var->list_el);
    }
  CheckOptDim(f,var,0);
}

/***********************************************
 * Scalar 
 ***********************************************/

void CheckSCALAR(f,var,flag) FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  CheckCom(f,var,flag);
  CheckOptDim(f,var,0);
  Fprintf(f,indent,"CheckScalar(%d,m%d,n%d);\n",i1,i1,i1);
}

/***********************************************
 * Pointers 
 ***********************************************/

void CheckPOINTER(f,var,flag) 
     FILE *f;   VARPTR var ;int flag;
{
  int i1= var->stack_position;
  if ( flag == 1 ) 
    sprintf(str2,"k");
  else
    sprintf(str2,"%d",i1);
  sprintf(str1,"%d",i1);
  if (var->list_el ==0 )
    {
      sprintf(str1,"%d",i1);
    }
  else
    {
      fprintf(stderr,"Wrong type opointer inside a list \n");
      exit(1);
    }
  AddDeclare1(DEC_INT,"lr%s",str1);
}


void CheckANY(f,var,flag) FILE *f;   VARPTR var ;int flag;{
  fprintf(stderr,"Wrong type in Check function \n");
  exit(1);
}

void CheckLIST(f,var,flag) FILE *f;   VARPTR var ;int flag;{
  fprintf(stderr,"Wrong type in Check function \n");
  exit(1);
}

void CheckTLIST(f,var,flag) FILE *f;   VARPTR var ;int flag;{
  fprintf(stderr,"Wrong type in Check function \n");
  exit(1);
}

void CheckSEQUENCE(f,var,flag) FILE *f;   VARPTR var ;int flag;
{  
  fprintf(stderr,"Wrong type in Check function \n");
  exit(1);
};

void CheckEMPTY(f,var,flag)  FILE *f;   VARPTR var ;int flag;
{  
  fprintf(stderr,"Wrong type in Check function \n");
  exit(1);
};

void CheckWORK(f,var,flag)  FILE *f;   VARPTR var ;int flag;
{  
  fprintf(stderr,"Wrong type in Check function \n");
  exit(1);
};


void CheckDIMFOREXT(f,var,flag)  FILE *f;   VARPTR var ;int flag;
{  
  fprintf(stderr,"Wrong type in Check function \n");
  exit(1);
};


void CheckOptDim(f,var,nel)
     FILE *f;
     int nel;
     VARPTR var;
{
  if (var->el[nel]-1>=0) {
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
	  Fprintf(f,indent,"CheckOneDim(opts[%d].position,%d,%s,%s);\n",
		  var->stack_position - basfun->NewMaxOpt +1 ,
		  nel+1,
		  var1->for_name[0],var1->name);
	}
    }
  }
}



void CheckOptSquare(f,var,str1)
     FILE *f;
     VARPTR var;
     char *str1;
{
  /* XXXXX cas liste ? */
  if (var->el[0] == var->el[1]) 
    {
      Fprintf(f,indent,"CheckSquare(opts[%d].position,opts[%s].m,opts[%s].n);\n",
	      var->stack_position - basfun->NewMaxOpt +1 ,
	      str1,str1);
    }
}

