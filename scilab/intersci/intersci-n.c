#include <stdlib.h>

#include "intersci-n.h"
#include "getrhs.h" 
#include "crerhs.h" 

/* global variables */

int icre=1;     /* incremental counter for variable creation */
int indent = 0; /* incremental counter for code indentation */
int pass = 0 ;  /* flag for couting pass on code generation */
char target = 'C'; /* langage for generation */

VARPTR variables[MAXVAR]; /* array of VAR structures */
int nVariable; /* number of variables */
BASFUNPTR basfun; /* SCILAB function structure */
FORSUBPTR forsub; /* FORTRAN subroutine structure */
int nFun; /* total number of functions in "desc" file */
char *funNames[MAXFUN]; /* array of function names */
char str1[4*MAXNAM];
char str2[4*MAXNAM];

static void GenBuilder __PARAMS(( char *file,char *files,char *libs ));

/* local variables */

int main(argc,argv)
     unsigned int argc;
     char **argv;
{ 
  char *files,*libs;
  char *file;
  int interface = 0 ;
  switch (argc) 
    {
    case 2:
      file = argv[1];
      target =  'C';
      interface =  0; 
      files = NULL;
      libs = NULL;
      break;
    case 3:
      file = argv[1];
      target = 'C';
      interface = 0; 
      files = argv[2];
      libs = NULL;
      break;
    case 4:
      file = argv[1];
      target = 'C';
      interface = 0; 
      files = argv[2];
      libs = argv[3];
      break;
    default:
      printf("usage:  intersci <interface file> 'files' 'libs' \n");
      exit(1);
      break;
    }
  basfun = BasfunAlloc();
  if (basfun == 0) {
    printf("Running out of memory\n");
    exit(1);
  }
  forsub = ForsubAlloc();
  if (forsub == 0) {
    printf("Running out of memory\n");
    exit(1);
  }
  Generate(file);
  GenFundef(file,interface);
  GenBuilder(file,files,libs);
  exit(0);
}

/***************************************************************
 * Produce the interface 
 ***************************************************************/

void Generate(file)
     char *file;
{
  int icrekp;
  FILE *fin, *fout, *foutv;
  char filout[MAXNAM];
  char filin[MAXNAM];
  sprintf(filin,"%s.desc",file);
  fin = fopen(filin,"r");
  if (fin == 0) {
    printf("interface file \"%s\" does not exist\n",filin);
    exit(1);
  }
  Copyright();
  strcpy(filout,file);
  strcat(filout,(target == 'F' ) ? ".f" : ".c" );
  fout = fopen(filout,"w");
  strcpy(filout,file);
  strcat(filout,".tmp");
  foutv = fopen(filout,"w");
  InitDeclare();
  nFun = 0;
  Fprintf(fout,indent,"#include \"stack-c.h\"\n");
  while(ReadFunction(fin)) {
    nFun++;
    if (nFun > MAXFUN) {
      printf("Too many SCILAB functions. The maximum is %d\n",MAXFUN);
      exit(1);
    }
    pass=0;
    /** changing stack_positions (external variables are not in the stack)**/
    FixStackPositions();
    icrekp=icre;
    FixForNames();
    ResetDeclare();
    /** ShowVariables();**/
    /* first pass to collect declarations */
    WriteFunctionCode(foutv);
    /* cleaning added Fornames before pass 2 */
    ForNameClean();
    FixForNames();
    /* scond pass to produce code */
    pass=1;
    icre=icrekp;
    WriteFunctionCode(fout);
    /** WriteInfoCode(fout); **/
  }
  /* WriteMain(fout,file); */ 
  printf("C file \"%s.c\" has been created\n",file);
  /* WriteAddInter(file) ; */
  printf("Scilab file \"%s.sce\" has been created\n",file);
  fclose(fout);
  fclose(fin);
}

/***************************************************************
 * Interface function 
 ***************************************************************/

void WriteMain(f,file)
     FILE *f;
     char* file;
{
  int i;
  FCprintf(f,"\n/**********************\n");
  FCprintf(f," *  interface function \n");
  FCprintf(f," ********************/\n");
  Fprintf(f,indent++,"static TabF Tab[]={\n");
  for (i = 0; i < nFun; i++) {
    Fprintf(f,indent,"{ ints%s, \"%s\"},\n",funNames[i],funNames[i]);
  }
  Fprintf(f,--indent,"};\n\n");
  Fprintf(f,indent,"int C2F(%s)()\n",file);
  Fprintf(f,indent++,"{\n");
  Fprintf(f,indent,"Rhs=Max(0,Rhs);\n");
  Fprintf(f,indent,"(*(Tab[Fin-1].f))(Tab[Fin-1].name);\n");
  Fprintf(f,indent,"return 0;\n");
  Fprintf(f,--indent,"};\n");

}

/***************************************************************
 * Code for addinter 
 ***************************************************************/

void WriteAddInter(file) 
     char *file;
{
  FILE *fout;
  int i;
  char filout[MAXNAM];
  strcpy(filout,file);
  strcat(filout,".sce");
  fout = fopen(filout,"w");
  if ( fout != (FILE*) 0) 
    {
      fprintf(fout,"// Addinter for file %s\n",file);
      fprintf(fout,"// for hppa/sun-solaris/linux/dec\n");
      fprintf(fout,"//--------------------------------\n");
      fprintf(fout,"//Scilab functions \n");
      fprintf(fout,"%s_funs=[...\n",file);
      for (i = 0; i < nFun -1; i++) 
	fprintf(fout,"  '%s';\n",funNames[i]);
      fprintf(fout,"  '%s']\n",funNames[nFun-1]);
      fprintf(fout,"// interface file to link: ifile='%s.o' \n",file);
      fprintf(fout,"// user's files to link: ufiles=['file1.o','file2.o',....] \n");
      fprintf(fout,"addinter([files],'%s',%s_funs);\n",file,file);
      fclose(fout);
    }
  else
    fprintf(stderr,"Can't open file %s\n",file);
}
 

void Copyright()
{
  printf("\nINTERSCI Version %s (%s)\n",VERSION,DATE);
  printf("    Copyright (C) INRIA/ENPC All rights reserved\n\n");
}

/***************************************************************
  Code generation 
***************************************************************/

void WriteHeader(f,fname0,fname)
     FILE *f;
     char* fname,*fname0;
{
  Fprintf(f,indent,"\nint %s%s(fname)\n",fname0,fname);
  Fprintf(f,indent+3,"char *fname;\n");
  Fprintf(f,indent,"{\n");indent++;
  WriteDeclaration(f);
}

void WriteFunctionCode(f)
     FILE* f;
{
  int i;
  IVAR ivar;
  if ( pass == 1) 
    {
      printf("  generating C interface for function (%s) Scilab function\"%s\"\n",
	     forsub->name,
	     basfun->name);
    }
  FCprintf(f,"/******************************************\n");
  FCprintf(f," * SCILAB function : %s, fin = %d\n",basfun->name,nFun);
  FCprintf(f," ******************************************/\n");

  WriteHeader(f,"ints",basfun->name);

  /* optional arguments : new style */
  /** XXXXXX basfun->NewMaxOpt= basfun->maxOpt; */
  basfun->NewMaxOpt= basfun->maxOpt;
  if ( basfun->NewMaxOpt > 0 ) 
    {
      /** optional arguments **/ 
      AddDeclare(DEC_INT,"nopt");
      AddDeclare(DEC_INT,"iopos");
      Fprintf(f,indent,"nopt=NumOpt();\n");
    }

  /* rhs argument number checking */

  if ( basfun->NewMaxOpt > 0 ) 
    Fprintf(f,indent,"CheckRhs(%d,%d+nopt);\n",basfun->nin - basfun->maxOpt,
	    basfun->nin-basfun->maxOpt);
  else 
    Fprintf(f,indent,"CheckRhs(%d,%d);\n",basfun->nin - basfun->maxOpt,basfun->nin);

  
  /* lhs argument number checking */
  ivar = basfun->out;
  if ( ivar == 0) 
    {
      Fprintf(f,indent,"CheckLhs(0,1);\n");
    }
  else 
    {
      if ((variables[ivar-1]->length == 0) 
	  || (variables[ivar-1]->type == LIST)
	  || (variables[ivar-1]->type == TLIST))
	{
	  Fprintf(f,indent,"CheckLhs(1,1);\n");
	}
      else 
	{
	  Fprintf(f,indent,"CheckLhs(1,%d);\n",variables[ivar-1]->length);
	}
    }
  /* SCILAB argument checking */
  for (i = 0; i < basfun->nin - basfun->NewMaxOpt ; i++)
    {
      switch ( variables[i]->type ) 
	{
	case LIST : 
	  WriteListAnalysis(f,i,"l");
	  break;
	case TLIST:
	  WriteListAnalysis(f,i,"t");
	  break;
	case MLIST :
	  WriteListAnalysis(f,i,"m");
	  break;
        default:
	  WriteArgCheck(f,i);
	  break;
	}
    }

  if ( basfun->NewMaxOpt != 0) 
    {
      sprintf(str1,"rhs_opts opts[]={\n");
      for (i = basfun->nin -basfun->NewMaxOpt ; i < basfun->nin ; i++)
	{
	  sprintf(str2,"\t{-1,\"%s\",\"%s\",0,0,0},\n",variables[i]->name,
		  SGetForTypeAbrev(variables[i]));
	  strcat(str1,str2);
	}
      strcat(str1,"\t{-1,NULL,NULL,NULL,0,0}}");
      AddDeclare(DEC_DATA,str1); 
      Fprintf(f,indent,"iopos=Rhs;\n");
      Fprintf(f,indent,"if ( get_optionals(fname,opts) == 0) return 0;\n");
      for (i = basfun->nin -basfun->NewMaxOpt ; i < basfun->nin ; i++)
	{
	  WriteOptArgPhase2(f,i);
	}
    }
  
  /* SCILAB cross checking */
  WriteCrossCheck(f);

  /* SCILAB equal output variable checking */
  WriteEqualCheck(f);
  
  /* FORTRAN call */
  WriteFortranCall(f);
  
  /* FORTRAN output to SCILAB */
  WriteOutput(f);
}


void WriteInfoCode(f)
     FILE* f;
{
  int i,iout;
  IVAR ivar;
  VARPTR var,vout;

  iout = GetExistOutVar();
  vout = variables[iout -1];

  switch (vout->type) {
  case LIST:
  case TLIST:
    /* loop on output variables */
    printf("list(");
    for (i = 0; i < vout->length; i++) 
      {
	ivar = vout->el[i];
	var = variables[ivar-1];
	printf("%s",var->name);
	if ( i != vout->length -1 ) 
	  printf(",");
	else
	  printf(")");
      }
    break ;
  case SEQUENCE:
    /* loop on output variables */
    printf("[");
    for (i = 0; i < vout->length; i++) 
      {
	ivar = vout->el[i];
	var = variables[ivar-1];
	printf("%s",var->name);
	if ( i != vout->length -1 ) 
	  printf(",");
	else
	  printf("]");
      }
    break;
  case EMPTY:
    printf("[]\n");
    break;
  }

  printf("=%s(",basfun->name);
  for (i = 0; i < basfun->nin; i++)
    {
      printf("%s(%s)",variables[i]->name,SGetSciType(variables[i]->type));
      if ( i != basfun->nin -1 ) 
	printf(",");
    }
  printf(")\n");

}

/*************************************************************
 *  Ckecking and getting infos for datas coming from scilab calling 
 * sequence ( datas on the stack ) 
 ***********************************************************/

void WriteArgCheck(f,i)
     FILE *f;
     int i;
{
  int i1;
  VARPTR var = variables[basfun->in[i]-1];

  i1 = i + 1;
  
  Fprintf(f,indent,"/*  checking variable %s */\n",var->name);
  
  if (var->opt_type != 0) 
    {
      /* Optional Arguments */ 
      WriteOptArg(f,var);
    }
  else 
    {
      /** 
       * generate the code for getting a Scilab argument 
       * and check some dimensions property if necessary 
       **/
      if (RHSTAB[var->type].type != var->type ) 
	{
	  fprintf(stderr,"Bug in intersci : Something wrong in RHSTAB\n");
	}
      (*(RHSTAB[var->type].fonc))(f,var,0);
    }
}



/*************************************************************
 * cross checking dimensions 
 ***********************************************************/

void WriteCrossCheck(f)
     FILE *f;
{
  int i, j;
  VARPTR var;
  Fprintf(f,indent,"/* cross variable size checking */\n");
  for (i = 0; i < nVariable; i++) 
    {
      var = variables[i];
      if ( var->type == DIMFOREXT ) 
	{
	  if ( var->nfor_name > 1) 
	    {
	      for ( j = 1 ; j < var->nfor_name ; j++) 
		{
		  /** we do not check square variables : this is done elsewhere */
		  /* we do not check external values since they are not known here */
		  if ( (var->for_name_orig[j] != var->for_name_orig[j-1]) 
		       && ( var->for_name[j-1][1] != 'e' &&  var->for_name[j][1] != 'e' ))
		    {
		      Fprintf(f,indent,"CheckDimProp(%d,%d,%s != %s);\n",
			      var->for_name_orig[j-1], var->for_name_orig[j],
			      var->for_name[j-1],   var->for_name[j]);
		    }
		}
	    }
	}
      else if (var->type == SCALAR) 
	{
	  /** some dimensions are given by a scalar input argument **/
	  if ( var->nfor_name > 1) 
	    {
	      for ( j = 1 ; j < var->nfor_name ; j++) 
		{
		  int dim=2;
		  if ( var->for_name[j][0]=='m') dim=1;
		  if ( var->for_name[j][1] != 'e' )  /* do not check external variables */
		    {
		      if ( strncmp(var->for_name[0],"istk",4)==0) 
			Fprintf(f,indent,"CheckOneDim(%d,%d,%s,*%s);\n",
				var->for_name_orig[j], dim ,  var->for_name[j],var->for_name[0]);
		      else
			Fprintf(f,indent,"CheckOneDim(%d,%d,%s,%s);\n",
				var->for_name_orig[j], dim ,  var->for_name[j],var->for_name[0]);
		    }
		}
	    }
	}
    }
  /* 
     FCprintf(f,"/ *       cross formal parameter checking\n");
     FCprintf(f," *      not implemented yet * /\n");  */
}




void WriteEqualCheck(f)
     FILE *f;
{
  /*Fprintf(f,indent,"/ * cross equal output variable checking\n");
    Fprintf(f,indent,"   not implemented yet* /\n"); */
}

/***************************************************************
 * Scilab argument of type list 
 ***************************************************************/

void WriteListAnalysis(f,i,list_type)
     FILE *f;
     int i;
     char *list_type;
{
  int k,i1;
  VARPTR var;
  i1=i+1;

  AddDeclare1(DEC_INT,"m%d",i1);
  AddDeclare1(DEC_INT,"n%d",i1);
  AddDeclare1(DEC_INT,"l%d",i1);
  Fprintf(f,indent,"GetRhsVar(%d,\"%s\",&m%d,&n%d,&l%d);\n",
	  i1,list_type,i1,i1,i1);
  for (k = 0; k < nVariable ; k++) 
    {
      var = variables[k];
      if ((var->list_el != 0) &&
	  (strcmp(var->list_name,variables[i]->name) == 0) &&
	  var->present)
	{
	  Fprintf(f,indent,"/* list element %d %s */\n",var->list_el,var->name);
	  if (RHSTAB[var->type].type != var->type ) 
	    {
	      fprintf(stderr,"Bug in intersci : Something wrong in RHSTAB\n");
	    }
	  (*(RHSTAB[var->type].fonc))(f,var,0);
	}
    }
}

/***************************************************************
 * Create the code for stack creation of 
 * variables which are not Scilab argument 
 * and gather the code for C or Fortran call
 ***************************************************************/

void CheckCreateOrder()
{
  int ivar,min= 10000;
  int i,count=0;
  if ( forsub->narg == 0) return ;
  for (i = 0; i < forsub->narg; i++) 
    {
      ivar = forsub->arg[i];
      if (variables[ivar-1]->list_el == 0 
	  &&  variables[ivar-1]->is_sciarg == 0 
	  &&  variables[ivar-1]->for_type != EXTERNAL 
	  &&  variables[ivar-1]->for_type != CSTRINGV ) 
	{
	  count++;
	  if ( min != 10000 && variables[ivar-1]->stack_position !=0 
	       && variables[ivar-1]->stack_position < min) 
	    {
	      fprintf(stderr,"Error: declaration for local variables \n");
	      fprintf(stderr,"\t must respect the order given in the calling sequence\n");
	      fprintf(stderr,"\t declaration for %s must be moved downward\n",
		      variables[ivar-1]->name);
	      exit(1);
	    }
	  if (  variables[ivar-1]->stack_position !=0 )
	    min = variables[ivar-1]->stack_position;
	}
    }
}


void WriteFortranCall(f)
     FILE *f;
{
  int i, ind;
  IVAR ivar, iivar;
  char call[MAXCALL];

  sprintf(call,"C2F(%s)(",forsub->name);

  CheckCreateOrder();

  /* loop on FORTRAN arguments */

  for (i = 0; i < forsub->narg; i++) 
    {
      ivar = forsub->arg[i];
      ind = 0;
      if (variables[ivar-1]->list_el != 0) 
	{
	  /* FORTRAN argument is a list element */
	  iivar = GetExistVar(variables[ivar-1]->list_name);
	  if ( variables[iivar-1]->is_sciarg == 0) 
	    {
	      printf("list or tlist \"%s\" must be an argument of SCILAB function\n",
		     variables[ivar-1]->list_name);
	      exit(1);	    
	    }
	  strcat(call,variables[ivar-1]->for_name[0]);
	  strcat(call,",");
	}
      else 
	{
	  if ( variables[ivar-1]->is_sciarg == 1)
	    {
	      if (target == 'C' && variables[ivar-1]->C_name[0] != NULL) 
		{
		  strcat(call,"&");
		  strcat(call,variables[ivar-1]->C_name[0]);
		}
	      else 
		strcat(call,variables[ivar-1]->for_name[0]);
	      strcat(call,",");
	    }
	  else 
	    {
	      /* FORTRAN argument is not a SCILAB argument */
	      /* a new variable is created on the stack for each 
		 Fortran argument */
	      (*(CRERHSTAB[variables[ivar-1]->type].fonc))(f,variables[ivar-1]);
	      if (target == 'C' && variables[ivar-1]->C_name[0] != NULL) 
		{
		  strcat(call,"&");
		  strcat(call,variables[ivar-1]->C_name[0]);
		}
	      else 
		strcat(call,variables[ivar-1]->for_name[0]);
	      strcat(call,",");
	    }
	}
    }
  if  (forsub->narg == 0) 
    strcat(call,")");
  else 
    call[strlen(call)-1] = ')';

  if (target == 'C' ) strcat(call,";\n");
  Fprintf(f,indent,call);

  for ( i=0 ; i < nVariable ; i++) 
    {
      if ( strcmp(variables[i]->name,"err")==0)
	{
	  AddDeclare(DEC_INT,"err=0");
	  Fprintf(f,indent++,"if (err >  0) {\n");  
	  Fprintf(f,indent,"Scierror(999,\"%%s: Internal Error \\r\\n\",fname);\n");
	  Fprintf(f,indent,"return 0;\n");
	  Fprintf(f,--indent,"};\n");
	  break;
	}
    }
}

/***************************************************
 * Write the interface code 
 * for lhs variables creation 
 *****************************************************/

void WriteOutput(f)
     FILE *f;
{
  IVAR iout,ivar;
  VARPTR var,vout;
  int i;
  
  iout = CheckOutVar();
  
  if ( iout == 0) 
    {
      Fprintf(f,indent,"LhsVar(1)=0;\n;return 0;\n");
    }
  else 
    {
      vout = variables[iout-1];
      switch (vout->type) 
	{
	case LIST:
	case TLIST:
	case MLIST:
	  Fprintf(f,indent,"/* Creation of output %s of length %d*/\n",
		  SGetSciType(vout->type),vout->length);
	  vout->stack_position = icre;
	  icre++;
	  Fprintf(f,indent,"Create%s(%d,%d);\n",
		  SGetSciType(vout->type),
		  vout->stack_position,
		  vout->length);
	  /* loop on output variables */
	  for (i = 0; i < vout->length; i++) 
	    {
	      ivar = vout->el[i];
	      var = variables[ivar-1];
	      Fprintf(f,indent,"/* Element %d: %s*/\n",i+1,var->name);
	      WriteVariable(f,var,ivar,1,i+1);
	    }
	  Fprintf(f,indent,"LhsVar(1)= %d;\nreturn 0;",vout->stack_position);
	  break;
	case SEQUENCE:
	  /* loop on output variables */
	  for (i = 0; i < vout->length; i++) 
	    {
	      ivar = vout->el[i];
	      var = variables[ivar-1];
	      WriteVariable(f,var,ivar,0,0);
	    }
	  Fprintf(f,indent,"return 0;\n");
	  break;
	case EMPTY:
	  Fprintf(f,indent,"LhsVar(1)=0;\n;return 0;\n");
	  break;
	}
    }
  Fprintf(f,--indent,"}\n");
}

/***********************************************
 * Output of variable var 
 *   if variable is outputed inside a list 
 *   insidelist is set to true (1) and nel is the number 
 *   of the variable in the list 
 ***********************************************/

void WriteVariable(f,var,ivar,insidelist,nel)
     FILE *f;
     VARPTR var;
     IVAR ivar;
     int insidelist;
{
  IVAR ivar2, barg, farg;
  VARPTR var2;
  int j;

  /* get number of variable in SCILAB calling list */

  barg = GetNumberInScilabCall(ivar);

  /* get number of variable in FORTRAN calling list */

  farg = GetNumberInFortranCall(ivar);
  
  if (var->for_type == EXTERNAL) 
    {
      /* external type */
      if (barg != 0) 
	{
	  printf("output variable with external type \"%s\" \n",var->name);
	  printf("  cannot be an input argument of SCILAB function\n");
	  exit(1);
	}
      if (var->equal != 0) 
	{
	  printf("output variable with external type \"%s\"\n", var->name);
	  printf("  cannot have a convertion\n");
	  exit(1);
	}
      if (farg == 0) 
	{
	  printf("output variable with external type \"%s\" must be\n", var->name);
	  printf("  an argument of FORTRAN subroutine");
	  exit(1);
	}
      WriteExternalVariableOutput(f,var,insidelist,nel);
    }
  else 
    {
      if ( insidelist == 0 && var->list_el == 0 )
	{
	  if ( var->opt_type != 0) 
	    {
	      Fprintf(f,indent,"LhsVar(%d)= opts[%d].position /* %s */;\n",
		      var->out_position,
		      var->stack_position - basfun->NewMaxOpt+1,
		      var->name);
	    }
	  else
	    {

	      if ( var->for_type == CSTRINGV) 
		/* variable is recreated fro output */
		Fprintf(f,indent,"LhsVar(%d)= %d;\n",
			var->out_position,icre);
	      else 
		Fprintf(f,indent,"LhsVar(%d)= %d;\n",
			var->out_position,var->stack_position);
	    }
	}
      if (var->equal != 0) 
	{
	  /* SCILAB type convertion */
	  if (barg !=0 || farg!= 0) 
	    {
	      printf("output variable with convertion \"%s\" must not be\n",var->name);
	      printf("  an input variable of SCILAB function or an argument\n");
	      printf("  of FORTRAN subroutine\n");
	      exit(1);
	    }
	  ivar2 = var->equal;
	  var2 = variables[ivar2-1];
	  /* get number of equal variable in SCILAB calling list */
	  barg = 0;
	  for (j = 0; j < basfun->nin; j++) 
	    {
	      if (ivar2 == basfun->in[j]) 
		{
		  barg = j + 1;
		  break;
		}
	    }
	  if (barg == 0) 
	    {
	      printf("output variable with convertion \"%s\" must be\n",
		     var->name);
	      printf("  an input variable of SCILAB function\n");
	      exit(1);
	    }
	  /* get number of equal variable in FORTRAN calling list */
	  farg = 0;
	  for (j = 0; j < forsub->narg; j++) {
	    if (ivar2 == forsub->arg[j]) {
	      farg = j + 1;
	      break;
	    }
	  }
	  if (farg == 0) 
	    {
	      printf("output variable with convertion \"%s\" must be\n",
		     var->name);
	      printf("  an argument FORTRAN subroutine");
	      exit(1);
	    }
	  var->for_type = var2->for_type;
	  WriteVariableOutput(f,var,1,insidelist,nel);
	}
      else 
	{
	  /* no SCILAB type convertion */
	  if ( var->type == LIST || var->type == TLIST )
	    {
	      /** il faut alors verifier la condition pour 
		  tous les arguments de la liste **/
	      WriteVariableOutput(f,var,0,insidelist,nel);
	      return;
	    }
	  if (farg == 0 ) {
	    printf("variable without convertion \"%s\" must be an argument\n",
		   var->name);
	    printf("  of FORTRAN subroutine\n");
	    exit(1);
	  }
	  
	  WriteVariableOutput(f,var,0,insidelist,nel);
	}
    }
}


int GetNumberInScilabCall(ivar)
     int ivar;
{
  int j;
  for (j = 0; j < basfun->nin; j++) 
    {
      if (ivar == basfun->in[j]) {
	return(j+1);
	break;
      }
    }
  return(0);
}

int GetNumberInFortranCall(ivar)
     int ivar;
{
  int j;
  for (j = 0; j < forsub->narg; j++) 
    {
      if (ivar == forsub->arg[j]) 
	{
	  return( j + 1);
	  break;
	}
    }
  return(0);
}

/********************************************
 * changes string "str" to "int(str)" 
 * if str begins with stk or return str unchanged 
 ********************************************/
 
char unknown[]="ukn";

char *Forname2Int(var,i)
     VARPTR var;
     int i;
{
  int l;
  char *p;
  if ( var->for_name[i] == (char *) 0) 
    {
      printf("Error in Forname2Int for variable %s \n",var->name);
      printf("Maybe an internal variable has a dimension\n");
      printf("which can't be evaluated\n");
      abort();
      return(unknown);
    }
  if ( var->C_name[i] != (char *) 0) 
    return var->C_name[i];
  if (strncmp(var->for_name[i],"stk",3) == 0) {
    l = strlen(var->for_name[i]);
    p = (char *)malloc((unsigned)(l + 6));
    sprintf(p,"int(%s)",var->for_name[i]);
    return p;
  }
  else return var->for_name[i];
}

void GenFundef(file,interf)
     char *file;
     int interf;
{
  FILE *fout;
  char filout[MAXNAM];
  int i,j;
  if (interf != 0 ) 
    {
      strcpy(filout,file);
      strcat(filout,".fundef");
      fout = fopen(filout,"w");
      fprintf(fout,"#define IN_%s %.2d\n",file,interf);
      for (i = 0; i < nFun; i++) {
	fprintf(fout,"{\"%s\",",funNames[i]);
	for (j = 0; j < 25 - (int)strlen(funNames[i]); j++) fprintf(fout," ");
	fprintf(fout,"\t\tIN_%s,\t%d,\t3},\n",file,i+1);
      }
      printf("\nfile \"%s\" has been created\n",filout);
      fclose(fout);
    }
}

static void GenBuilder(file,files,libs)
     char *file;
     char *files,*libs;
{
  FILE *fout;
  char filout[MAXNAM];
  int i;
  strcpy(filout,file);
  strcat(filout,"_builder.sce");
  fout = fopen(filout,"w");
  fprintf(fout,"// generated with intersci \n");
  fprintf(fout,"ilib_name = 'lib%s'\t\t// interface library name\n",file);

  /* files = 'file1.o file2.o ....' delimiter = ' ' */
  while ( files != NULL)
    {
      static int first =1; 
      if ( first ==1 ) { fprintf(fout,"files =['%s.o';\n\t'",file);first ++;} 
      else { fprintf(fout,"\t'");}
      while ( *files != 0 && *files != ' ' ) { fprintf(fout,"%c",*files); files++;}
      while ( *files == ' ') files++;
      if ( *files == 0 ) { fprintf(fout,"'];\n"); break;}
      else {  fprintf(fout,"'\n");} ;
    }

  while ( libs != NULL)
    {
      static int first =1; 
      if ( first ==1 ) { fprintf(fout,"libs =['");first ++;} 
      else { fprintf(fout,"\t'");}
      while ( *libs != 0 && *libs != ' ' ) { fprintf(fout,"%c",*libs); libs++;}
      while ( *libs == ' ') libs++;
      if ( *libs == 0 ) { fprintf(fout,"'];\n"); break;}
      else {  fprintf(fout,"'\n");} ;
    }

  fprintf(fout,"\ntable =[");
  i=0;
  if ( nFun == 1) 
    fprintf(fout,"\"%s\",\"ints%s\"];\n",funNames[i],funNames[i]);
  else 
    {
      fprintf(fout,"\"%s\",\"ints%s\";\n",funNames[i],funNames[i]);
      for (i = 1; i < nFun-1; i++) {
	fprintf(fout,"\t\"%s\",\"ints%s\";\n",funNames[i],funNames[i]);
      }
      i=nFun-1;
      fprintf(fout,"\t\"%s\",\"ints%s\"];\n",funNames[i],funNames[i]);
    }
  fprintf(fout,"ilib_build(ilib_name,table,files,libs);\n");
  printf("\nfile \"%s\" has been created\n",filout);
  fclose(fout);
}




/**********************************************************
  Dealing With Fortran OutPut 
  taking into account indentation and line breaks after column 72
***********************************************************/


#define MAXBUF 4096 
char sbuf[MAXBUF];

#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif 

#ifdef __STDC__ 
void  Fprintf(FILE *f,int indent,char *format,...) 
#else 
     /*VARARGS0*/
     void Fprintf(va_alist) va_dcl
#endif
{
  int i;
  static int count=0;
  va_list ap;
#ifdef __STDC__
  va_start(ap,format);
#else
  int indent;
  char *format;
  FILE *f;
  va_start(ap);
  f = va_arg(ap, FILE *);
  indent= va_arg(ap, int );
  format = va_arg(ap, char *);
#endif
  vsprintf(sbuf,format,ap);

  for ( i = 0 ; i < (int) strlen(sbuf); i++) 
    {
      if ( count == 0)  
	{
	  white(f,indent);
	  count = indent;
	}
      if ( count >= 100 && sbuf[i] != '\n' && (sbuf[i] == ' ' || sbuf[i]== ',' || sbuf[i] == ';' || sbuf[i] == '(' ) ) 
	{ 
	  fprintf(f,"\n");
	  white(f,indent);count=indent;
	}
      if ( sbuf[i] == '\n') count = -1 ;
      fprintf(f,"%c",sbuf[i]);
      count++;
    }
  va_end(ap);
}

void white(f,ind)
     FILE *f;
     int ind;
{
  int i ;
  for (i =0 ; i < ind ; i++) fprintf(f," ");
}


#ifdef __STDC__ 
void  FCprintf(FILE *f,char *format,...) 
#else 
     /*VARARGS0*/
     void FCprintf(va_alist) va_dcl
#endif
{
  va_list ap;
#ifdef __STDC__
  va_start(ap,format);
#else 
  FILE *f;
  char *format;
  va_start(ap);
  f = va_arg(ap, FILE *);
  format = va_arg(ap, char *);
#endif
  vfprintf(f,format,ap);
  va_end(ap);
}

/******************************************
 *  memory allocators 
 ******************************************/

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
