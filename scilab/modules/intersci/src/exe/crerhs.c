#include <stdlib.h>
#include "intersci-n.h"
#include "crerhs.h"

/*****************************************************************
 * For each possible data type we have here a function 
 * which generate the code for <<Creating>> a variable 
 * and checking some properties 
 *     if needed check if the variable is square 
 *     if needed check if some dimensions must be of fixed sizes
 * All the possible Getfunction are stored in a function table 
 ******************************************************************/

/**************************************************
 * A FINIR : 
 * le code qui est ici est utilise a deux fins 
 * Creer des objets ds le stack Scilab 
 * et construire en meme temps 
 * la sequence d'appel pour 
 * le Fortran 
 * Il faudrait sans doute couper cela en deux 
 * et surtout voir si l'on ne peux pas simplifier 
 * le tout : 
 *   i.e associer a une variable 
 *   des formats qui indiquent comment 
 *   on accede aux champs importants pour elle 
 *   + un numero unique de variable 
 *   un numero ds le stacl etc......
 **************************************************/

/******************************************************
 * the functions in the following table must follow  the 
 * order given by the type list defined in  intersci-n.h 
 * The correct ordering is checked when using this table.
 * (see intersci.c) 
 ********************************************************/

CreRhsTab CRERHSTAB[] = {
  {DIMFOREXT,CreDIMFOREXT},
  {COLUMN,CreCOLUMN},
  {LIST,CreANY},
  {TLIST,CreANY},
  {MATRIX,CreMATRIX},
  {POLYNOM,CreVECTOR},
  {ROW,CreVECTOR},
  {SCALAR,CreSCALAR},
  {SEQUENCE,CreANY},
  {STRING,CreSTRING},
  {WORK,CreVECTOR},
  {EMPTY,CreANY},
  {ANY,CreANY},
  {VECTOR,CreVECTOR},
  {STRINGMAT,CreSTRINGMAT},
  {SCIMPOINTER,CrePOINTER},
  {IMATRIX,CreIMATRIX},
  {SCISMPOINTER,CrePOINTER},
  {SCILPOINTER,CrePOINTER},
  {BMATRIX,CreBMATRIX},
  {SCIBPOINTER,CrePOINTER},
  {SCIOPOINTER,CrePOINTER},
  {SPARSE,CreSPARSE}
};



extern  int indent ; /* incremental counter for code indentation */
extern  int pass;  /* flag for couting pass on code generation */

static  char str[MAXNAM];
static char str1[MAXNAM];
static char str2[MAXNAM];
static char str3[MAXNAM];
static char str4[MAXNAM];

/***********************************************
 * Matrix  XXXXX OK 
 ***********************************************/

void CreMATRIX(f,var)
     FILE *f;
     VARPTR var;
{
  WriteCallRestCheck(f,var,"mm",1,0) ;
  GetDim(str3,var->el[1]);
  WriteCallRestCheck(f,var,"nn",0,0) ;
  GetDim(str4,var->el[0]);
  if ( str3[0] == '&' || str3[0] == '(') 
    sprintf(str2,"%s",str3);
  else 
    sprintf(str2,"&%s",str3);
  if ( str4[0] == '&' || str4[0] == '(') 
    sprintf(str1,"%s",str4);
  else 
    sprintf(str1,"&%s",str4);
  CreCommon(f,var);
}

void CreCommon(f,var)
     FILE *f;
     VARPTR var;
{
  if (var->for_type == EXTERNAL ) 
    {
      Fprintf(f,indent,"/* external variable named %s (xxe%d) */\n",var->name,var->stack_position);
      AddDeclare1(DEC_INT,"me%d",var->stack_position);
      AddDeclare1(DEC_INT,"ne%d",var->stack_position);
       if ( strncmp(var->fexternal,"cintf",4)==0 ) 
	 AddDeclare1(DEC_IPTR,"le%d",var->stack_position);
       else if ( strncmp(var->fexternal,"cboolf",5)==0 ) 
	 AddDeclare1(DEC_IPTR,"le%d",var->stack_position);
       else if ( strncmp(var->fexternal,"cdoublef",7)==0 ) 
	 AddDeclare1(DEC_DPTR,"le%d",var->stack_position);
       else if (strncmp(var->fexternal,"ccharf",5)==0) 
	 AddDeclare1(DEC_CPTR,"le%d",var->stack_position);
       else if (strncmp(var->fexternal,"cfloatf",6)==0 ) 
	 AddDeclare1(DEC_RPTR,"le%d",var->stack_position);
       else 
	 AddDeclare1(DEC_UL,"le%d",var->stack_position);
       ChangeForName2(var,"&le%d",var->stack_position);
       /** List case not considered here **/
    } 
   else 
     {
       char *lstr1,*lstr2;
       if ( strncmp(str1,"&istk(",6)==0 || strncmp(str1,"&cstk(",6)==0 || strncmp(str1,"&rstk(",6)==0 
	    || strncmp(str1,"&stk(",5)==0 ) lstr1 = str1+1;
       else
	 lstr1=str1;
       if ( strncmp(str2,"&istk(",6)==0 || strncmp(str2,"&cstk(",6)==0 || strncmp(str2,"&rstk(",6)==0 
	    || strncmp(str2,"&stk(",5)==0 ) lstr2 = str2+1;
       else
	 lstr2=str2;

       Fprintf(f,indent,"CreateVar(%d,\"%s\",%s,%s,&l%d);/* named: %s */\n",
	       var->stack_position,
	       SGetForTypeAbrev(var),
	       lstr1,lstr2,var->stack_position,
	       var->name);
       AddDeclare1(DEC_INT,"l%d",var->stack_position);
       ChangeForName2(var,"%s(l%d)",SGetForTypeStack(var),
		      var->stack_position);
     }
}

/***********************************************
 * String   XXXXX OK 
 ***********************************************/

void CreSTRING(f,var)
     FILE *f;
     VARPTR var;
{
  if (var->for_type != CHAR && var->for_type != EXTERNAL ) 
    {
      printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
	     SGetSciType(STRING),SGetForType(var->for_type),var->name);
      exit(1);
    }
  AddDeclare1(DEC_INT,"n%d",var->stack_position);
  AddDeclare1(DEC_INIT,"n%d=1",var->stack_position);
  sprintf(str2,"&n%d",var->stack_position);
  WriteCallRestCheck(f,var,"nn",0,0) ;
  GetDim(str1,var->el[0]);
  CreCommon(f,var);
}



/***********************************************
 * Boolean matrix  XXXXX OK 
 ***********************************************/

void CreBMATRIX(f,var)
     FILE *f;
     VARPTR var;
{
  if (var->for_type == INT ) var->for_type = BOOLEAN;
  if (var->for_type != BOOLEAN && var->for_type != EXTERNAL )
    {
      printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
	     SGetSciType(var->type),SGetForType(var->for_type),var->name);
      exit(1);
    }
  CreMATRIX(f,var);
}

/***********************************************
 * variable which are dimensions 
 * (of Scilab variables or external)
 ***********************************************/

void CreDIMFOREXT(f,var)
     FILE *f;
     VARPTR var;
{
  if (var->nfor_name == 0 && var->for_type != PREDEF) 
    {
      printf("dimension variable \"%s\" is not defined\n",var->name);
      exit(1);
    }
  switch (var->for_type) 
    {
    case PREDEF:
    case 'C':
      if ( strcmp(var->name,"rhs") == 0) 
	{
	  AddDeclare(DEC_INT,"rhs");
	  Fprintf(f,indent,"rhs=Rhs;\n");
	  sprintf(str,"&rhs");
	}
      else 
	{
	  sprintf(str,"&%s",var->name);
	}
      ChangeForName1(var,str);
      break;
    case 0:
    case INT:
      sprintf(str,"&%s",var->for_name[0]);
      if ( ~isdigit(str[1])) 
	{
	  ChangeForName1(var,str);
	}
      else 
	{
	  Fprintf(f,indent,"loc%s= (int) %s;\n",
		  var->for_name[0],var->for_name[0]);
	  sprintf(str,"loc%s",var->for_name[0]);
	  AddDeclare(DEC_INT,str);
	  sprintf(str,"&loc%s",var->for_name[0]);
	  ChangeForName1(var,str);
	}
      break;
    case DOUBLE:
      Fprintf(f,indent,"loc%s= (double) %s;\n",
	      var->for_name[0],var->for_name[0]);
      sprintf(str,"loc%s",var->for_name[0]);
      AddDeclare(DEC_DOUBLE,str);
      sprintf(str,"&loc%s",var->for_name[0]);
      ChangeForName1(var,str);
      break;
    case REAL:
      Fprintf(f,indent,"loc%s= (float) %s;\n",var->for_name[0],var->for_name[0]);
      sprintf(str,"loc%s",var->for_name[0]);
      AddDeclare(DEC_REAL,str);
      sprintf(str,"&loc%s",var->for_name[0]);
      ChangeForName1(var,str);
      break;
    case CHAR:
    case CSTRINGV:
      printf("a dimension variable cannot have FORTRAN type \"%s\"\n",
	     SGetForType(var->for_type));
      exit(1);
      break;
    }
}

/***********************************************
 * Common code for COLUMN, ROW, WORK,POLYNOM,VECTOR
 ***********************************************/

void CreVECTOR(f,var)
     FILE *f;
     VARPTR var;
{
  char *lstr1 ;
  WriteCallRestCheck(f,var,"nn",0,0) ;
  GetDim(str1,var->el[0]);
  AddDeclare1(DEC_INT,"un=1");
  AddDeclare1(DEC_INT,"mn%d",var->stack_position);
  lstr1 = ( str1[0]== '&' )? str1+1: str1;
  /** peut-etre un pb si str1 contient des stk istk .. XXXXXX **/
  if ( strncmp(lstr1,"stk",3) ==0 || strncmp(lstr1,"istk",4)==0 
       || strncmp(lstr1,"rstk",4)==0 || strncmp(lstr1,"cstk",4)==0 )
    sprintf(str2,"(mn%d=*%s,&mn%d)",var->stack_position,
	    lstr1,var->stack_position);
  else
    sprintf(str2,"(mn%d=%s,&mn%d)",var->stack_position,
	    lstr1,var->stack_position);
  strcpy(str1,"(un=1,&un)");
  CreCommon(f,var);
}

void CreCOLUMN(f,var)
     FILE *f;
     VARPTR var;
{
  char *lstr1 ;
  WriteCallRestCheck(f,var,"nn",0,0) ;
  GetDim(str1,var->el[0]);
  AddDeclare1(DEC_INT,"un=1");
  AddDeclare1(DEC_INT,"mn%d",var->stack_position);
  lstr1 = ( str1[0]== '&' )? str1+1: str1;
  /** peut-etre un pb si str1 contient des stk istk .. XXXXXX **/
  if ( strncmp(lstr1,"stk",3) ==0 || strncmp(lstr1,"istk",4)==0 
       || strncmp(lstr1,"rstk",4)==0 || strncmp(lstr1,"cstk",4)==0 )
    sprintf(str2,"(mn%d=*%s,&mn%d)",var->stack_position,
	    lstr1,var->stack_position);
  else
    sprintf(str2,"(mn%d=%s,&mn%d)",var->stack_position,
	    lstr1,var->stack_position);
  strcpy(str1,str2);
  strcpy(str2,"(un=1,&un)");
  CreCommon(f,var);
}


/***********************************************
 * Sparse OK
 ***********************************************/

void CreSPARSE(f,var)
     FILE *f;
     VARPTR var;
{
  WriteCallRestCheck(f,var,"mm",1,0) ;
  GetDim(str2,var->el[1]);
  WriteCallRestCheck(f,var,"nn",0,0) ;
  GetDim(str1,var->el[0]);
  if (var->for_type == EXTERNAL) 
    {
      AddDeclare1(DEC_SPARSEPTR,"S%d",var->stack_position);
      ChangeForName2(var,"&S%d",var->stack_position);
      /** List case not considered here **/
    } 
  else 
    {
      VARPTR m,n;
      int origm,orign;
      m= variables[var->el[0]-1];
      n= variables[var->el[1]-1];
      /* here we must create a sparse variable 
       * but str1 and str2 is not enough to create the matrix 
       * we copy the argument which gives the size 
       */
      origm = ( m->nfor_name == 0)? -1 : m->for_name_orig[0];
      orign = ( n->nfor_name == 0)? -1 : n->for_name_orig[0];
      if ( origm != orign ) 
	{
	  fprintf(stderr,"A local sparse matrix can only be built as a copy of a sparse entry\n");
	  exit(1);
	}
      AddDeclare1(DEC_SPARSE,"S%d",var->stack_position);
      Fprintf(f,indent,"CreateVarFromPtr(%d,\"s\",&%s,&%s,&S%d);\n",
	      var->stack_position,
	      str1,str2,origm);
      Fprintf(f,indent,"GetRhsVar(%d,\"s\",&%s,&%s,&S%d);\n",
	      var->stack_position,
	      str1,str2,var->stack_position);
      ChangeForName2(var,"&S%d",var->stack_position);
    }
}


/***********************************************
 * Complex Matrix OK 
 ***********************************************/

void CreIMATRIX(f,var)
     FILE *f;
     VARPTR var;
{
  WriteCallRestCheck(f,var,"it",2,0) ;
  GetDim(str3,var->el[2]);
  WriteCallRestCheck(f,var,"mm",1,0) ;
  GetDim(str2,var->el[1]);
  WriteCallRestCheck(f,var,"nn",0,0) ;
  GetDim(str1,var->el[0]);
  if (var->for_type == EXTERNAL) 
    {
      AddDeclare1(DEC_UL,"ler%d",var->stack_position);
      AddDeclare1(DEC_UL,"lec%d",var->stack_position);
      AddDeclare1(DEC_INT,"ite%d",var->stack_position);
      ChangeForName2(var,"&ler%d,&lec%d,&ite%d",var->stack_position,var->stack_position,var->stack_position);
      /** List case not considered here **/
    } 
  else 
    {
      Fprintf(f,indent,"CreateCVar(%d,\"%s\",&%s,&%s,&%s,&lr%d,&lc%d);\n",
	      var->stack_position,SGetForTypeAbrev(var),
	      str3,str1,str2,var->stack_position,var->stack_position);
      AddDeclare1(DEC_INT,"lr%d",var->stack_position);
      AddDeclare1(DEC_INT,"lc%d",var->stack_position);
      AddDeclare1(DEC_INT,"it%d",var->stack_position);
      ChangeForName2(var,"%s(lr%d),%s(lc%d),&it%d",
		     SGetForTypeStack(var),
		     var->stack_position, 
		     SGetForTypeStack(var),
		     var->stack_position,var->stack_position);
    }
}


/***********************************************
 * Pointers 
 ***********************************************/

void CrePOINTER(f,var)
     FILE *f;
     VARPTR var;
{
  if (var->for_type == EXTERNAL) 
    {
      AddDeclare1(DEC_UL,"le%d",var->stack_position);
      ChangeForName2(var,"&le%d",var->stack_position);
      /** List case not considered here **/
    } 
  else 
    {
      Fprintf(f,indent,"CreateOpointer(%d,&lr%d);\n",
	      var->stack_position,var->stack_position);
      AddDeclare1(DEC_INT,"lr%d",var->stack_position);
      ChangeForName2(var,"stk(lr%d)",
		     var->stack_position);
    }
}


/***********************************************
 * STRINGMAT
 ***********************************************/

void CreSTRINGMAT(f,var)
     FILE *f;
     VARPTR var;
{
  if (var->for_type == EXTERNAL || var->for_type == CSTRINGV )
    {
      /* for external or cstringv parameters, unknown formal dimensions 
	 can be used */
      WriteCallRestCheck(f,var,"nsmm",0,1) ;
      WriteCallRestCheck(f,var,"nsnn",1,1) ;
      sprintf(str,"nsmm%d",var->stack_position);
      AddForName1(var->el[0],str,NULL,var->stack_position);
      sprintf(str,"nsnn%d",var->stack_position);
      AddForName1(var->el[1],str,NULL,var->stack_position);
      AddDeclare1(DEC_SMAT,"Str%d",var->stack_position);
      AddDeclare1(DEC_INT,"nsmm%d",var->stack_position);
      AddDeclare1(DEC_INT,"nsnn%d",var->stack_position);
      sprintf(str,"&Str%d",var->stack_position);
      ChangeForName1(var,str);
    }
  else 
    {
      /** XXXX dimensions should be specified **/
      fprintf(stderr,"WARNING : your code contains a specification \n");
      fprintf(stderr," not fully implemented in intersci \n");
      WriteCallRestCheck(f,var,"mm",0,0) ;
      WriteCallRestCheck(f,var,"nn",1,0) ;
      AddDeclare(DEC_LOGICAL,"cresmatafaire");
      Fprintf(f,indent,"if(.not.cresmatafaire(fname,top-rhs+%d,lr%d)) return\n",var->stack_position,var->stack_position);
      sprintf(str,"stk(lr%d)",var->stack_position);
      ChangeForName1(var,str);
    }
}


/***********************************************
 * Scalar 
 ***********************************************/

void CreSCALAR(f,var)
     FILE *f;
     VARPTR var;
{
  strcpy(str1,"&un");
  strcpy(str2,"&un");
  AddDeclare(DEC_INT,"un=1");
  CreCommon(f,var);
}

/***********************************************
 ** Common code for LIST TLIST SEQUENCE ANY **
 ***********************************************/

void CreANY(f,var)
     FILE *f;
     VARPTR var;
{
  printf("work or output FORTRAN argument cannot have\n");
  printf("  type \"ANY\", \"LIST\", \"TLIST\" or \"SEQUENCE\"\n");
  exit(1);
}

/***********************************************
 * Utility function for WriteCallRest 
 * when flag==1 we acccept undefined dimensions 
 * this is used with stringmat/Cstringv 
 * where dimensions and space are allocated inside 
 * the interfaced function and returned 
 * to the interface 
 *******************************************/

void WriteCallRestCheck(f,var,name,iel,flag) 
     VARPTR var ;
     FILE *f;
     char *name;
     int iel,flag;
{
  char sdim[MAXNAM]; 
  char str[MAXNAM];
  if (variables[var->el[iel]-1]->nfor_name == 0) 
    {
      strcpy(str,variables[var->el[iel]-1]->name);
      if (isdigit(str[0]) == 0) 
	{
	  if ( variables[var->el[iel]-1]->is_sciarg == 1) 
	    {
	      /* dimension of FORTRAN argument is a SCILAB argument */
	      sprintf(sdim,"%s%d",name,var->stack_position);
	      Fprintf(f,indent,"%s= (int) *stk(lr%d));\n",sdim,
		      variables[var->el[iel]-1]->stack_position);
	      AddForName1(var->el[iel],sdim,NULL,var->stack_position);
	    }
	  else if ( flag != 1) 
	    {
	      printf("dimension variable \"%s\" is not defined\n",
		     variables[var->el[iel]-1]->name);
	      exit(1);
	    }
	} 
      else 
	{
	  sprintf(sdim,"%s%d",name,var->stack_position);
	  Fprintf(f,indent,"%s=%s;\n",sdim,str);
	  AddForName1(var->el[iel],sdim,NULL,var->stack_position);
	}
    }
}

/** to be finished : Forname2Int is not finished fo C  **/

void GetDim(str,ivar) 
     IVAR ivar;
     char *str;
{
  char *s;
  s=Forname2Int(variables[ivar-1],0);
  if ( strncmp(s,"stk",3)==0 || strncmp(s,"istk",4)==0
       || strncmp(s,"sstk",4)==0 || strncmp(s,"stk",3)==0 || 
       strncmp(s,"cstk",4)==0 )
    {
      sprintf(str,"%s",s);
      return ;
    }
  else 
    {
      AddDeclare1(DEC_INT,s);
      sprintf(str,"%s",s);
    }
}


