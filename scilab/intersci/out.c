#include <stdlib.h>

#include "intersci-n.h"

/* global variables */

extern int icre ;    /* incremental counter for variable creation */
extern int indent ; /* incremental counter for code indentation */
extern int pass ; /* flag for couting pass on code generation */

static char str[MAXNAM];
static char str1[MAXNAM];
static char str2[MAXNAM];
static char str3[MAXNAM];
static char str4[MAXNAM];
static char str5[MAXNAM];
static char strR[MAXNAM];
static char strc[MAXNAM];
static char strit[MAXNAM];

void OutCOLUMN(f,var,insidelist,nel)
     FILE *f;VARPTR var;
{
  strcpy(str2,"un");
  AddDeclare1(DEC_INT,"un=1");
  strcpy(str1,Forname2Int(variables[var->el[0]-1],0));  
  OutCommon(f,var,insidelist,nel);
}

void OutROW(f,var,insidelist,nel)
     FILE *f;VARPTR var;
{
  strcpy(str1,"un");
  AddDeclare1(DEC_INT,"un=1");
  strcpy(str2,Forname2Int(variables[var->el[0]-1],0));
  OutCommon(f,var,insidelist,nel);
}

void OutVECTOR(f,var,insidelist,nel)
     FILE *f;VARPTR var;
{
  strcpy(str1,"un");
  AddDeclare1(DEC_INT,"un=1");
  strcpy(str2,Forname2Int(variables[var->el[0]-1],0));
  OutCommon(f,var,insidelist,nel);
}

void OutMATRIX(f,var,insidelist,nel)
     FILE *f;VARPTR var;
{
  strcpy(str1,Forname2Int(variables[var->el[0]-1],0));
  strcpy(str2,Forname2Int(variables[var->el[1]-1],0));
  OutCommon(f,var,insidelist,nel);
}

void OutSCALAR(f,var,insidelist,nel)
     FILE *f;VARPTR var;
{
  strcpy(str1,"un");
  strcpy(str2,"un");
  AddDeclare1(DEC_INT,"un=1");
  OutCommon(f,var,insidelist,nel);
}

void OutCommon(f,var,insidelist,nel)
     FILE *f;VARPTR var;
     int insidelist,nel;
{
  int i1= var->stack_position;
  if (var->list_el ==0 )
    sprintf(str3,"l%d",i1);
  else
    sprintf(str3,"l%de%d",i1,var->list_el);
  if ( insidelist != 0)
    {
      /** var is inside a list **/
      int iout = GetExistOutVar();
      VARPTR vout = variables[iout -1];
      AddDeclare(DEC_INT,"lrs");
      Fprintf(f,indent,"CreateListVarFrom(%d,%d,\"%s\",&%s,&%s,&lrs,&%s);\n",
	      vout->stack_position,nel,SGetForTypeAbrev(var),
	      (str1[0] == '&' ) ? str1+1: str1,
	      (str2[0] == '&' ) ? str2+1: str2,
	      str3);
    }
  else
    {
      /* var is not outputed inside a list but var can be a list 
	 element */
      if ( var->list_el != 0) 
	{
	  AddDeclare(DEC_INT,"lrs");
	  Fprintf(f,indent,"CreateVarFrom(%d,\"%s\",&%s,&%s,&lrs,&%s);\n",
		  icre,SGetForTypeAbrev(var),
		  (str1[0] == '&' ) ? str1+1: str1,
		  (str2[0] == '&' ) ? str2+1: str2,
		  str3);
	  Fprintf(f,indent,"LhsVar(%d)=%d;\n",var->out_position,icre);
	  icre++;
	}
    }
}



void OutBMATRIX(f,var,insidelist,nel)
     FILE *f;VARPTR var;
     int insidelist,nel;
{
  strcpy(str1,Forname2Int(variables[var->el[0]-1],0));
  strcpy(str2,Forname2Int(variables[var->el[1]-1],0));
  OutCommon(f,var,insidelist,nel);
}


void OutSTRING(f,var,insidelist,nel)
     FILE *f;VARPTR var;
{
  strcpy(str1,Forname2Int(variables[var->el[0]-1],0));
  AddDeclare1(DEC_INT,"n%d",var->stack_position);
  sprintf(str2,"&n%d",var->stack_position);
  OutCommon(f,var,insidelist,nel);
}


void OutLIST(f,var,insidelist,nel)
     FILE *f;VARPTR var;
     int insidelist,nel;
{
  if ( insidelist != 0)
    {
      printf("can't output an argument of type list if the global\n");
      printf("output is a list\n");
      exit(1);
    }
  else 
    {
      int k;
      VARPTR var1;
      /** var is a list output ==> backconversion of its arguments **/
      for (k = 0; k < nVariable ; k++) 
	{
	  var1 = variables[k];
	  if ((var1->list_el != 0) &&
	      (strcmp(var1->list_name,var->name) == 0) &&
	      var1->present)
	    {
	      OutLISTarg(f,var,var1,insidelist,nel);
	    }
	}
    }
}

/* performs back conversion of list arguments */

void OutLISTarg(f,var,var1,insidelist,nel)
     FILE *f;VARPTR var;VARPTR var1;
     int insidelist,nel;
{
  switch ( var1->type) 
    {
    case BMATRIX :
      /* nothing to do */
      break;
    case ROW :
    case VECTOR:
      if ( var1->for_type != DOUBLE )
	{
	  strcpy(str1,"un");
	  AddDeclare1(DEC_INT,"un=1");
	  strcpy(str2,Forname2Int(variables[var1->el[0]-1],0));
	  Fprintf(f,indent,"ConvertData(\"%s\",%s*%s,l%de%d);\n",
		  SGetForTypeAbrev(var1),str1,str2,
		  var1->stack_position,  var1->list_el);
	}
      break;
    case COLUMN:
      strcpy(str2,"un");
      AddDeclare1(DEC_INT,"un=1");
      strcpy(str1,Forname2Int(variables[var1->el[0]-1],0));
      Fprintf(f,indent,"ConvertData(\"%s\",%s*%s,l%de%d);\n",
	      SGetForTypeAbrev(var1),str1,str2,
	      var1->stack_position,  var1->list_el);
      break;
    case MATRIX :

      if ( var1->for_type != DOUBLE )
	{
	  strcpy(str1,Forname2Int(variables[var1->el[0]-1],0));
	  strcpy(str2,Forname2Int(variables[var1->el[1]-1],0));
	  Fprintf(f,indent,"ConvertData(\"%s\",%s*%s,l%de%d);\n",
		  SGetForTypeAbrev(var1),str1,str2,
		  var1->stack_position,  var1->list_el);
	}
      break;
    case SCALAR : 
      if ( var1->for_type != DOUBLE )
	{
	  strcpy(str2,"un");
	  strcpy(str1,"un");
	  AddDeclare1(DEC_INT,"un=1");
	  Fprintf(f,indent,"ConvertData(\"%s\",%s*%s,l%de%d);\n",
		  SGetForTypeAbrev(var1),str1,str2,
		  var1->stack_position,  var1->list_el);
	}
      break;
    case STRING:
      strcpy(str1,Forname2Int(variables[var1->el[0]-1],0));
      Fprintf(f,indent,"ConvertData(\"c\",%s,l%de%d);\n",
	      str1,var1->stack_position,var1->list_el);
      break;
    default:
      printf("OutLIST : List output : unfinished \n");
      break;
    }
}





/***********************************************
 * Sparse Matrix 
 ***********************************************/


void OutSPARSE(f,var,insidelist,nel)
     FILE *f;VARPTR var;
{
  int i1= var->stack_position;
  strcpy(str1,Forname2Int(variables[var->el[0]-1],0));
  strcpy(str2,Forname2Int(variables[var->el[1]-1],0));
  if (var->list_el ==0 )
    {
      sprintf(str5,"%d",i1);
    }
  else
    {
      sprintf(str5,"%de%d",i1,var->list_el);
    }
  if ( insidelist != 0)
    {
      /** var is inside a list **/
      int iout = GetExistOutVar();
      VARPTR vout = variables[iout -1];
      Fprintf(f,indent,"CreateListVarFromPtr(%d,%d,\"s\",&%s,&%s,&S%s);\n",
	      vout->stack_position,nel,str1,str2,str5);
    }
  else
    {
      /* var is not outputed inside a list but var can be a list 
	 element */
      if ( var->list_el != 0) 
	{
	  Fprintf(f,indent,"CreateVarFromPtr(%d,\"s\",&%s,&%s,&S%s);\n",
		  icre,str1,str2,str5);
	  Fprintf(f,indent,"LhsVar(%d)=%d;\n",var->out_position,icre);
	  icre++;
	}
    }
}

/***********************************************
 * Complex Matrix OK
 ***********************************************/

void OutIMATRIX(f,var,insidelist,nel)
     FILE *f;VARPTR var;
{
  int i1= var->stack_position;
  strcpy(str1,Forname2Int(variables[var->el[0]-1],0));
  strcpy(str2,Forname2Int(variables[var->el[1]-1],0));
  strcpy(str3,Forname2Int(variables[var->el[2]-1],0));
  if (var->list_el ==0 )
    {
      sprintf(str4,"lr%d",i1);
      sprintf(str5,"lc%d",i1);
    }
  else
    {
      sprintf(str4,"lr%de%d",i1,var->list_el);
      sprintf(str5,"lc%de%d",i1,var->list_el);
    }
  if ( insidelist != 0)
    {
      /** var is inside a list **/
      int iout = GetExistOutVar();
      VARPTR vout = variables[iout -1];
      AddDeclare(DEC_INT,"lrs");
      Fprintf(f,indent,"CreateListCVar(%d,%d,\"%s\",%s,%s,%s,&lrs,&lcs,%s,%s);\n",
	      vout->stack_position,nel,SGetForTypeAbrev(var),
	      str3,str1,str2,str4,str5);
    }
  else
    {
      /* var is not outputed inside a list but var can be a list 
	 element */
      if ( var->list_el != 0) 
	{
	  AddDeclare(DEC_INT,"lrs");
	  Fprintf(f,indent,"CreateCVarFrom(%d,\"%s\",%s,%s,%s,&lrs,&lcs,%s,%s);\n",
		  icre,SGetForTypeAbrev(var),
		  str3,str1,str2,str4,str5);
	  Fprintf(f,indent,"LhsVar(%d)=%d;\n",var->out_position,icre);
	  icre++;
	}
    }
}



void OutPOLYNOM(f,var,insidelist,nel)
     FILE *f;VARPTR var;
     int insidelist,nel;
{
  strcpy(str1,Forname2Int(variables[var->el[0]-1],0));
  if ( insidelist != 0)
    {
      AddDeclare(DEC_LOGICAL,"listcreopoly");
      Fprintf(f,indent,"if(.not.listcreopoly(fname,top,%d,lw,0,%s,name%s,namel%s,lrs,lcs)) return\n",nel,str1,str,str);
    }
  else 
    {
      AddDeclare(DEC_LOGICAL,"creonepoly");
      Fprintf(f,indent,"if(.not.creonepoly(fname,top,0,%s,name%s,namel%s,lrs,lcs)) return\n",str1,str,str);
    }
  switch (var->for_type) {
  case INT:
    Fprintf(f,indent,"call int2db(%s,stk(%s),-1,stk(lrs),-1)\n",
	    str1,str);
    break;
  case DOUBLE:
    Fprintf(f,indent,"call dcopy(%s,stk(%s),1,stk(lrs),1)\n",
	    str1,str);
    break;
  case REAL:
    Fprintf(f,indent,"call rea2db(%s,stk(%s),-1,stk(lrs),-1)\n",
	    str1,str);
    break;
  }
}


void OutPOINTER(f,var,insidelist,nel)
     FILE *f;VARPTR var;
     int insidelist,nel;
{
  if ( insidelist != 0)
    {
      printf(" %s in output list : not implemented ;",SGetSciType(var->type));
    }
  else 
    {

    }
}


void OutSTRINGMAT(f,var,insidelist,nel)
     FILE *f;VARPTR var;
     int insidelist,nel;
{
  strcpy(str1,Forname2Int(variables[var->el[0]-1],0));
  strcpy(str2,Forname2Int(variables[var->el[1]-1],0));
  sprintf(str,"lr%d",var->stack_position);
  if ( insidelist != 0)
    {
      int iout = GetExistOutVar();
      VARPTR vout = variables[iout -1];
      Fprintf(f,indent,"CreateListVarFromPtr(%d,%d,\"S\",&%s,&%s,Str%d);\n",
	      vout->stack_position,nel,
	      (str1[0] == '&' ) ? str1+1: str1,
	      (str2[0] == '&' ) ? str2+1: str2,
	      var->stack_position);
    }
  else
    {
      Fprintf(f,indent,"CreateVarFromPtr(%d, \"S\",&%s,&%s,Str%d);\n",
	      icre,
	      (str1[0] == '&' ) ? str1+1: str1,
	      (str2[0] == '&' ) ? str2+1: str2,
	      var->stack_position);
      Fprintf(f,indent,"FreeRhsSVar(Str%d);\n",var->stack_position);
      icre++;
    }
}



void OutANY(f,var,insidelist,nel)
     FILE *f;VARPTR var; 
     int insidelist,nel;
{
  printf("output variable \"%s\" cannot have type\n",
	 var->name);
  printf("  \"WORK\", \"LIST\", \"TLIST\", \"SEQUENCE\" or \"ANY\"\n");
  exit(1);
}

typedef  struct  {
  int type;
  void  (*fonc) __PARAMS((FILE *f,VARPTR var,int insidelist,int nel));} OutRhsTab ;




OutRhsTab OUTRHSTAB[] = {
  {DIMFOREXT,OutANY},
  {COLUMN,OutCOLUMN},
  {LIST,OutLIST},
  {TLIST,OutANY},
  {MATRIX,OutMATRIX},
  {POLYNOM,OutPOLYNOM},
  {ROW,OutROW},
  {SCALAR,OutSCALAR},
  {SEQUENCE,OutANY},
  {STRING,OutSTRING},
  {WORK,OutVECTOR},
  {EMPTY,OutANY},
  {ANY,OutANY},
  {VECTOR,OutVECTOR},
  {STRINGMAT,OutSTRINGMAT},
  {SCIMPOINTER,OutPOINTER},
  {IMATRIX,OutIMATRIX},
  {SCISMPOINTER,OutPOINTER},
  {SCILPOINTER,OutPOINTER},
  {BMATRIX,OutBMATRIX},
  {SCIBPOINTER,OutPOINTER},
  {SCIOPOINTER,OutPOINTER},
  {SPARSE,OutSPARSE}
};


/***********************************************
 * Output of variable var 
 *   if variable is outputed inside a list 
 *   insidelist is set to true and nel is the number 
 *   of the variable in the list 
 * Convert ==> 1 if the variable has an output convertion to be done
 ***********************************************/

void WriteVariableOutput(f,var,convert,insidelist,nel)
     FILE *f;
     VARPTR var;
     int  insidelist,convert,nel;
{
  if ( var->list_name != (char *) 0 ) 
    {
      /* variable is a list member */
      int nel1= var->list_el;
      sprintf(strR,"%de%d",var->stack_position,nel1);
      sprintf(str,"lr%de%d",var->stack_position,nel1);
      sprintf(strc,"lc%de%d",var->stack_position,nel1);
      sprintf(strit,"it%de%d",var->stack_position,nel1);
    }
  else 
    {
      sprintf(strR,"%d",var->stack_position);
      sprintf(str,"lr%d",var->stack_position);
      sprintf(strc,"lc%d",var->stack_position);
      sprintf(strit,"it%d",var->stack_position);
    }
  (*(OUTRHSTAB[var->type].fonc))(f,var,insidelist,nel);
}

