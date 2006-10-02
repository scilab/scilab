/**************************************************/
/* intmacr2tree.c                                 */
/* Functions used for macr2tree() Scilab function */
/* Copyright INRIA                                */
/* V.C. - 2004                                    */
/**************************************************/

#include "intmacr2tree.h"

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h"  /* malloc */
#else
#include "../os_specific/sci_mem_alloc.h"  /* malloc */
#endif
/* Table to store variable names */
static char varnames[isizt][nlgh+1];

/* Number of variables */
static int nbvars = 0;

/* Store stack index for last EOL */
static int last_eol_pos = 0;

/****************************************************************
 Function name: macr2tree
****************************************************************/
int C2F(macr2tree) _PARAMS((char *fname))
{
  /* Returned value parameters */
  int m_pgrm_tlist = 1,n_pgrm_tlist = 6;
  char *pgrm_tlist[] = {"program","name","outputs","inputs","statements","nblines"};
  
  int *stkdata = NULL; /* Pointeur to rhs arguments */
  int *data = NULL; /* Macro integer vector (pointer to copy of rhs argument) */
 
  int minRhs = 1; /* Minimum number of RHS arguments */
  int maxRhs = 1; /* Maximum number of RHS arguments */
  int minLhs = 1; /* Minimum number of LHS arguments */
  int maxLhs = 1; /* Maximum number of LHS arguments */
  
  int il = 0,ils = 0,ile = 0,ilt = 0,codelength = 0;
  
  int i = 0,cod_ind = 0; /* Loop index */
  int job1 = 1; /* Used when job passed as a pointer to a function */
  
  /* Number of lines */
  int nblines = 1;

  /* Name (character string used to store function/lhs/rhs names */
  char **name = NULL;
  int namelgth = 0;

  /* Number of statements in macro */
  int nbstat = 0;

  /* Generic variables */
  int one = 1;

  /* Save Top */
  int TopSave = 0;
  
  /* Save last code interpreted */
  int cod_sav = 0;

  /* Loop index */
  int k = 0;

  /* Used for statements list creation */
  int sz = 0; /* Size */
  int newinstr = 0; /* flag used to know if a new instruction has been created (1->TRUE) */

  /* Verify number of RHS arguments */
  CheckRhs(minRhs,maxRhs);
  
  /* Verify number of LHS arguments */
  CheckLhs(minLhs,maxLhs);

  /* Read all data */
  stkdata = (int *) stk(*Lstk(Top));

  if (stkdata[0] > 0) /* Not a reference to variable */
    {
      Scierror(999,"macr2tree: input argument must be a named variable\r\n");
      return 0;
    }
  else
    {
      stkdata = (int *) stk(stkdata[1]);
    }

  /* Verify good type for input: must be a compiled macro (type 13) */
  if(stkdata[0] != 13)
    {
      Scierror(999,"macr2tree: Wrong input type (must be a compiled macro)!\r\n");
      return 0;
    }

  /* Memory allocation */
  if((name=CALLOC(1,sizeof(char)))==NULL)
    {
      Scierror(999,"macr2tree: No more memory available\r\n");
      return 0;
    }
  if((name[0]=(char *)CALLOC(1,sizeof(char)*(nlgh+1)))==NULL)
    {
      Scierror(999,"macr2tree: No more memory available\r\n");
      return 0;
    }
  (name[0])[nlgh]='\0';
  
  /* Get function name: variable name on top of idstk */
  /* Must be done before writing anything on stack or else we have to save Top when entering this program */

  CvNameL(idstk(1,Top),name[0],&job1,&namelgth);
  (name[0])[namelgth]='\0';

  /* Input variable is no more useful */
  Top--;

  /* Write 'program' tlist first element on stack */
  str2sci(pgrm_tlist,m_pgrm_tlist,n_pgrm_tlist);
 
  /* Write function name on stack */
  str2sci(name,one,one);
 
  ils=il+1; /* stkdata[ils] = number of outputs */

  /* Read output parameters names */
  for(i=0;i<stkdata[ils];i++)
    {
      CvNameL(&stkdata[ils+1+i*nsiz],name[0],&job1,&namelgth);
      (name[0])[namelgth]='\0';
      CreateVariableTList(name);
    }
  C2F(mklist)(&stkdata[ils]);

  ile = ils+nsiz*stkdata[ils]+1; /* stkdata[ile] = number of outputs */

  /* Read input parameters names */
  for(i=0;i<stkdata[ile];i++)
    {
      CvNameL(&stkdata[ile+1+i*nsiz],name[0],&job1,&namelgth);
      (name[0])[namelgth]='\0';
      CreateVariableTList(name);
    }
  C2F(mklist)(&stkdata[ile]);

  ilt=ile+nsiz*stkdata[ile]+1;

  codelength=stkdata[ilt];

  /* Make a copy variable passed as reference */
  /* Memory allocation */
  if((data=(int *)CALLOC(1,sizeof(int)*(codelength+ilt+1)))==NULL)
    {
      Scierror(999,"macr2tree: No more memory available\r\n");
      return 0;
    }
  /* Copy */
  for(k=0;k<(codelength+ilt+1);k++)
    data[k]=stkdata[k];

  /* List header */
  /* Considering Top is pointing last occupied place */

  /* Number of elements in list */
  cod_ind = ilt + 1;
  nbstat = complexity(data,&ilt,&codelength);

  Top++; /* First free place */
   
  il = iadr(*Lstk(Top));
  *istk(il) = 15;
  *istk(il+1) = nbstat;
  *istk(il+2) = 1;

  *Lstk(Top+1) = sadr(il+3+nbstat);
  
  /* Error handling (S. Steer */
  if (*Lstk(Top+1) >= *Lstk(Bot)) {
    Scierror(17,"macr2tree: stack size exceeded (Use stacksize function to increase it)\r\n");

    /* Free memory */
    FREE(name[0]);
    name[0]=NULL;
    FREE(name);
    name=NULL;
    FREE(data);
    
    return 0;
  }

  /* Fill list */
  for(k=1;k<=nbstat;k++)
    {
      newinstr = 0;
      TopSave=Top;
      while(newinstr==0)
  	{
	  cod_sav=data[cod_ind];
	  GetInstruction(data,&cod_ind,&nblines,&newinstr);

	  /* Error handling (S. Steer) */
	  if (Err>0 || C2F(errgst).err1>0)
	    {
	      /* Free memory */
	      FREE(name[0]);
	      name[0]=NULL;
	      FREE(name);
	      name=NULL;
	      FREE(data);

	      return 0;
	    }
 	  if(cod_sav==15 && data[cod_ind+1]==29) /* EOL as the last component of a column concatenation */
	    {
	      /* a = ['a'
		 'b'
		 ] */
	      Top--; /* EOL is erased */
	      last_eol_pos=-10; /* EOL position is erased */
	      newinstr=0; /* No new instruction created */
	      nbstat--; /* One statement deleted */
	    }
	  else if(cod_sav==15 && Top!=TopSave+1) /* Column catenation with EOL after semi-colon */
	    newinstr=0;
	  else if(cod_sav==15) /* If EOL is not after semi-colon in catenation, it is ignored */
	    last_eol_pos=-10;

	  cod_ind++;
	  if(cod_ind>codelength+ilt+1)
	    {
	      Scierror(999,"macr2tree: Out of code\r\n");
 
	      /* Free memory */
	      FREE(name[0]);
	      name[0]=NULL;
	      FREE(name);
	      name=NULL;
	      FREE(data);
	      
	      return 0;
	    }

	}
      if(TopSave!=Top-1) {
	Scierror(999,"macr2tree: wrong Top value %d instead of %d\r\n",Top,TopSave+1);

	/* Free memory */
	FREE(name[0]);
	name[0]=NULL;
	FREE(name);
	name=NULL;
	FREE(data);
	
	return 0;
      }

      sz = *Lstk(Top+1) - *Lstk(Top);

      *istk(il+2+k) = *istk(il+1+k) + sz ;

      Top--;
      
      *Lstk(Top+1) = *Lstk(Top+2);
    }

  /* Number of lines */
  C2F(itosci)(&nblines,&one,&one);

  C2F(mktlist)(&n_pgrm_tlist);

  /* Free memory */
  FREE(name[0]);
  name[0]=NULL;
  FREE(name);
  name=NULL;
  FREE(data);
  
  return 0;
}

/****************************************************************
 Function name: CreateVariableTList
****************************************************************/
static int CreateVariableTList(char **varname)
{
  char *variable_tlist[] = {"variable","name"};
  int m_variable_tlist = 1;
  int n_variable_tlist = 2;

  int one = 1;

  /* Add 'variable' tlist items to stack */
  str2sci(variable_tlist,m_variable_tlist,n_variable_tlist);

  /* Add variable name to stack */
  str2sci(varname,one,one);

  /* Create tlist */
  C2F(mktlist)(&n_variable_tlist);

  /* Add variable to known variables table */
  AddVar(varname[0]);

  return 0;
}

/****************************************************************
 Function name: CreateEOLList
****************************************************************/
static int CreateEOLList(void)
{
  char **eol;

  int one = 1;

  /* Memory allocation */
  if((eol=CALLOC(1,sizeof(char)))==NULL)
    {
      Scierror(999,"CreateEOLList: No more memory available\r\n");
      return 0;
    }
  if((eol[0]=(char *)CALLOC(1,sizeof(char)*(strlen("EOL")+1)))==NULL)
    {
      Scierror(999,"CreateEOLList: No more memory available\r\n");
      return 0;
    }
  (eol[0])[3]='\0';
  strncpy(eol[0],"EOL",3);

  /* Add eol to stack */
  str2sci(eol,one,one);

  /* Create list */
  C2F(mklist)(&one);

  /* Free memory */
  FREE(eol[0]);
  eol[0]=NULL;
  FREE(eol);
  eol=NULL;

  return 0;
}

/****************************************************************
 Function name: AddVar
****************************************************************/
static int AddVar(char *name)
{
  if(IsDefinedVar(name)==-1)
      {
	strcpy(varnames[nbvars],name);
	nbvars++;
       }
  return 0;
}

/****************************************************************
 Function name: IsDefinedVar
****************************************************************/
static int IsDefinedVar(char *name)
{
  int index = -1;
  int k;
  int maxlgth;

  for(k=0;k<isizt;k++)
    {
      if(strlen(name)>=strlen(varnames[k]))
	{
	  maxlgth=strlen(name);
	}
      else
	{
	  maxlgth=strlen(varnames[k]);
	}
      if(varnames[k][0]=='\0')
	{
	  index = -1;
	  break;
	} 
      else if(!strncmp(name,varnames[k],maxlgth))
	{
	  index = k;
	  break;
	}
    }
  return index;
}

/****************************************************************
 Function name: GetInstruction
****************************************************************/
static int GetInstruction(int *data,int *index,int *nblines,int *addinstr)
{
  int job1 = 1;

  char **name;
  int namelgth;
  
  *addinstr=0;

  /* Memory allocation */
  if((name=CALLOC(1,sizeof(char)))==NULL)
    {
      Scierror(999,"GetInstruction: No more memory available\r\n");
      return 0;
    }
  if((name[0]=(char *)CALLOC(1,sizeof(char)*(nlgh+1)))==NULL)
    {
      Scierror(999,"GetInstruction: No more memory available\r\n");
      return 0;
    }
  (name[0])[nlgh]='\0';

  switch(data[*index]) {
  case 0: /* Deleted operation */
    /* This code is ignored */
    *index += data[*index+1];
    break;
  case 1: /* Stack put (Obsolete) */
    CreateEqualTList("code1",data,index);
    *addinstr=1;
    break;
  case 2: /* Stack get */
    /* Read name */
    CvNameL(&data[*index+1],name[0],&job1,&namelgth);
    (name[0])[namelgth]='\0';
    *index += nsiz;

    if(data[*index+2]==0) /* stack get (rhs=0) */
      {
	CreateVariableTList(name);
	*index += 2;
     }
    else
      {
	if( (IsDefinedVar(name[0])>=0) || ( (data[*index+1]==-3) && (data[*index+2]!=0) ) )
	  {
	    /* Stack get for extraction from variable */
	    CreateVariableTList(name);
	    *index += 2;
	  }
	else
	  {
	    /* Macro call */
	    data[*index+1] = data[*index+2];
	    if(data[*index+3]==5 && data[*index+4]==3) /* 3=code for extraction */
	      /* If next instruction is an extraction, it is ignored */
	      {
		data[*index+2] = data[*index+6]; /* Replace number of lhs for macro call by the one of extraction */
		CreateFuncallTList("macro",data,index);
		*index += 4;
	      }
	    else
	      {
		data[*index+2] = 1;
		CreateFuncallTList("macro",data,index);
		*index += 4;
	      }
	  }
      }
    break;
  case 3: /* String */
    CreateCsteTList("string",data,index);
    break;
  case 4: /* Empty matrix */
    CreateCsteTList("emptymatrix",data,index);
    break;
  case 5: /* Operations */
    if(data[*index+2]==0) 
      {
        *index +=3;
        break;
      }
    CreateOperationTList(data,index);
    break;
  case 6: /* Number */
    CreateCsteTList("number",data,index);
    break;
  case 7: /* 'for' control instruction */
    GetControlInstruction(data,index,nblines);
    *addinstr=1;
    break;
  case 8: /* 'if-then-else' control instruction */
    GetControlInstruction(data,index,nblines);
    *addinstr=1;
    break;
  case 9: /* 'while' control instruction */
    GetControlInstruction(data,index,nblines);
    *addinstr=1;
    break;
  case 10: /* 'select-case' control instruction */
    GetControlInstruction(data,index,nblines);
    *addinstr=1;
    break;
  case 11: /* 'try-catch' control instruction */
    GetControlInstruction(data,index,nblines);
    *addinstr=1;
    break;
  case 12: /* pause */
  case 13: /* break */
  case 14: /* abort */
    CreateFuncallTList("datacode",data,index);
    *addinstr=1;
    break;
  case 15: /* EOL */
    (*nblines)++;
    CreateEOLList();
    last_eol_pos = Top;
    *addinstr=1;
    break;
  case 16: /* Set line number */
    /* This code is ignored */
    (*index)++;
    break;
  case 17: /* quit (Should never append) */
    CreateFuncallTList("datacode",data,index);
    *addinstr=1;
    break;
  case 18: /* Mark named variable */
    CreateEqualTList("code18",data,index);
    break;
  case 19: /* Form recursive index list */
    CreateRecursiveIndexList(data,index);
    break;
  case 20: /* exit */
    CreateFuncallTList("datacode",data,index);
    *addinstr=1;
    break;
  case 21: /* Beginning of rhs */
    /* This code is ignored */
    /* Code also ignored in CreateEqualTList with fromwhat=="code1" */
    break;
  case 22: /* Set print mode (ignored ?) */
    /* This code is ignored */
    break;
  case 23: /* Create variable from name */
    CreateCsteTList("code23",data,index);
    break;
  case 24: /* Create an object with type 0 */
    Scierror(999,"GetInstruction: code %d not yet implemented\r\n",data[*index]);
    break;
  case 25: /* Compute profiling data */
 /* This code is ignored */
    *index += 2;
    break;
  case 26: /* Vector of strings */
    Scierror(999,"GetInstruction: code %d not yet implemented\r\n",data[*index]);
    break;
  case 27: /* varfunptr */
    Scierror(999,"GetInstruction: code %d not yet implemented\r\n",data[*index]);
    break;
  case 28: /* continue */
    CreateFuncallTList("datacode",data,index);
    *addinstr=1;
    break;
  case 29: /* Affectation */
    CreateEqualTList("code29",data,index);
    *addinstr=1;
    break;
  case 30: /* Expression evaluation short circuiting */
    /* This code is ignored */
    *index += 2;
    break;
  case 31: /* comment */
     CreateCommentTList(data,index);
    *addinstr=1;
    break;

  case 99: /* return */
    CreateFuncallTList("datacode",data,index);
    *addinstr=1;
    break;
  default:
    if(data[*index]/100*100==data[*index] && data[*index]!=0)
      {
	/* funptr */
	CreateFuncallTList("funptr",data,index);
      }
    else
      {
	Scierror(999,"GetInstruction: unknown code %d at index %d \r\n",data[*index],*index );
	return 0;
      }
    break;
  }
  
  /* Free memory */
  FREE(name[0]);
  name[0]=NULL;
  FREE(name);
  name=NULL;

  return 0;
}

/****************************************************************
 Function name: GetControlInstruction
****************************************************************/
static int GetControlInstruction(int *data,int *index,int *nblines)
{
  /* try-catch */
  char *trycatch_tlist[] = {"trycatch","trystat","catchstat"};
  int m_trycatch_tlist = 1;
  int n_trycatch_tlist = 3;
  
  /* if */
  char *if_tlist[] = {"ifthenelse","expression","then","elseifs","else"};
  int m_if_tlist = 1;
  int n_if_tlist = 5;

  char *elseif_tlist[] = {"elseif","expression","then"};
  int m_elseif_tlist = 1;
  int n_elseif_tlist = 3;

  /* while */
  char *while_tlist[] = {"while","expression","statements"};
  int m_while_tlist = 1;
  int n_while_tlist = 3;

  /* select */
  char *select_tlist[] = {"selectcase","expression","cases","else"};
  int m_select_tlist = 1;
  int n_select_tlist = 4;

  char *case_tlist[] = {"case","expression","then"};
  int m_case_tlist = 1;
  int n_case_tlist = 3;

  /* for */
  char *for_tlist[] = {"for","expression","statements"};
  int m_for_tlist = 1;
  int n_for_tlist = 3;
  char **name;
  int namelgth = 0;

  int job1=1;
  int index0,endindex;
  int codelgth;
  int ncase = 0,icase = 0;
  int TopSave = 0,TopSave_elseifsorcases=0;
  int nbinstr = 0;
  int nbelseifsorcases = 0;

  int newinstr=0; /* Used to call GetInstruction with enough parameters */
  
  /* FOR */
  if(data[*index]==7)
    {
      /* Write list items */
      str2sci(for_tlist,m_for_tlist,n_for_tlist);
      (*index)++;
      codelgth = data[*index];
      endindex = *index + codelgth;
      (*index)++;

      /* Get expression */
      while(*index<=endindex)
	{
	  GetInstruction(data,index,nblines,&newinstr);
	  (*index)++;
	}
     
      codelgth = data[*index];
      (*index)++;

      /* Get loop variable */
      /* Memory allocation */
      if((name=CALLOC(1,sizeof(char)))==NULL)
	{
	  Scierror(999,"GetControlInstruction: No more memory available\r\n");
	  return 0;
	}
      if((name[0]=(char *)CALLOC(1,sizeof(char)*(nlgh+1)))==NULL)
	{
	  Scierror(999,"GetControlInstruction: No more memory available\r\n");
	  return 0;
	}
      (name[0])[nlgh]='\0';
      
      CvNameL(&data[*index],name[0],&job1,&namelgth);
      (name[0])[namelgth]='\0';
      *index += nsiz;
      /* Create a variable tlist with name */
      CreateVariableTList(name);
      
      /* variable = expression */
      CreateEqualTList("forexpr",data,index);
      endindex = *index + codelgth;
    
      /* Get all instructions */
      TopSave = Top;
      while(*index<=endindex)
	{
	  /* Get all instructions */
	  GetInstruction(data,index,nblines,&newinstr);
	  (*index)++;
	}
      (*index)--;
      /* Make list of instructions */
      nbinstr = Top - TopSave;
      C2F(mklist)(&nbinstr);

      /* Create FOR tlist */
      C2F(mktlist)(&n_for_tlist);

      /* Free memory */
      FREE(name[0]);
      name[0]=NULL;
      FREE(name);
      name=NULL;
    }
    /* TRYCATCH */
  else if(data[*index]==11)
    {  
      index0 = *index;
      
      str2sci(trycatch_tlist,m_trycatch_tlist,n_trycatch_tlist);
      
      /* index now point to first code to use as an instruction code */
      *index += 3;
      
      codelgth = data[index0+1];
      endindex = *index + codelgth - 1;
      
      TopSave = Top;
      /* Get try instructions */
      while(*index<=endindex)
	{
	  GetInstruction(data,index,nblines,&newinstr);
	  (*index)++;
	}
	
      nbinstr = Top - TopSave;
  
      /* Create list of try instructions */
      C2F(mklist)(&nbinstr);
      last_eol_pos = -10; 
      
      codelgth = data[index0+2];
      endindex = *index + codelgth - 1;
      
      TopSave = Top;
      /* Get catch instructions */
      while(*index<=endindex)
	{
	  GetInstruction(data,index,nblines,&newinstr);
	  (*index)++;
	}  
      
      nbinstr = Top - TopSave;
  
      /* Create list of catch instructions */
      C2F(mklist)(&nbinstr);
      
     (*index)--;
      
      /* Create trycatch tlist */
      C2F(mktlist)(&n_trycatch_tlist);
    }
  /* IF - WHILE - SELECT */
  else
    {
      index0 = *index;
      
      /* if or while of Scilab version < 3 */
      if( (data[*index]==8 || data[*index]==9) && data[*index+1]>=0 )
	{
	  /* This part will not be written */
	  /* No more used */
	  Scierror(999,"GetControlInstruction: old version of if and while not yet implemented\r\n");
	  return 0;
	}
      else
	{
	  ncase = data[index0+2]; /* Number of elseif + number of else = number of elseif + 1 */
	  
	  /* Write first tlist item (tlist fields) */
	  if(data[index0]==8)
	    {
	       str2sci(if_tlist,m_if_tlist,n_if_tlist);
	    }
	  else if(data[index0]==9)
	    {
	      str2sci(while_tlist,m_while_tlist,n_while_tlist);
	    }
	  else if(data[index0]==10)
	    {
	      str2sci(select_tlist,m_select_tlist,n_select_tlist);
	    }
	  
	  /* index now point to first code to use as an instruction code */
	  *index += 4;
	  
	  codelgth = data[index0+3];
	  endindex = *index + codelgth - 1;
	  
	  icase = ncase + 1;
	  
	  /* If control instruction is a select, I get expression */
	  if(data[index0]==10)
	    {
	      TopSave = Top;
	      while(*index<=endindex)
		{
		  GetInstruction(data,index,nblines,&newinstr);
		  (*index)++;
		}
	      /* Create a list for expression because can be expression+EOL */
	      nbinstr = Top - TopSave;
	      /* Create list of then instructions */
	      C2F(mklist)(&nbinstr);
	      last_eol_pos = -10; 
	    }
	  
	  
	  /* select: for all cases */
	  /* if: 1rst pass gives expression and then instructions */
	  /*     others passes give expression and then instructions for elseifs */
	  while(icase > 1)
	    {
	      icase = icase - 1;
	      
	      /* For a select: on first pass save position on stack to get the number of cases */
	      if(icase==ncase && data[index0]==10)
      		TopSave_elseifsorcases = Top; /* Saved to know how many cases have been written */
	      
	      if(icase<ncase && data[index0]==8) /* For a if (if loop already executed one time): found a elseif */
		{
		  /* Write tlist items */
		  str2sci(elseif_tlist,m_elseif_tlist,n_elseif_tlist);
		}
	      
	      if(data[index0]==10) /* For a select: found a case */
		{
		  /* Write tlist items */
		  str2sci(case_tlist,m_case_tlist,n_case_tlist);
		}
	      
	      codelgth = data[*index];
	      (*index)++;
	      endindex = *index + codelgth - 1;
	      
	      /* Get expression */
	      while(*index<=endindex)
		{
		  GetInstruction(data,index,nblines,&newinstr);
		  (*index)++;
		}
	      last_eol_pos = -10;
	      codelgth = data[*index];
	      (*index)++;
	      endindex = *index + codelgth - 1;
	      
	      /* Get then instructions */
	      TopSave = Top; /* Position on stack saved to get the number of instructions */
	      while(*index<=endindex)
		{
		  GetInstruction(data,index,nblines,&newinstr);
		  (*index)++;
		}
	      nbinstr = Top - TopSave;
	      /* Create list of then instructions */
	      C2F(mklist)(&nbinstr);
	      
	      if(icase<ncase && data[index0]==8) /* IF: create elseif tlist */
		{
		  /* Create elseif tlist */
		  C2F(mktlist)(&n_elseif_tlist);
		}
	      
	      if(data[index0]==10) /* SELECT: create case tlist */
		{
		  /* Create case tlist */
		  C2F(mktlist)(&n_case_tlist);
		}
	      
	      /* IF: after first pass, save position on stack to get the number of elseifs */
	      if(icase==ncase && data[index0]==8)
		TopSave_elseifsorcases = Top; /* Saved to know how many elseifs have been written */
	    }
	  
	  nbelseifsorcases = Top - TopSave_elseifsorcases;
	  
	  /* IF: create list of elseifs */
	  /* SELECT: create list of cases */
	  if(data[index0]==8 || data[index0]==10)
	    C2F(mklist)(&nbelseifsorcases);
	  
	  /* else (if there is one) (not used for WHILE) */
	  (*index)++;
	  codelgth = data[*index];
	  (*index)++;
	  
	  if(codelgth==0) /* When no else in IF or SELECT and when a WHILE */
	    (*index)++;
	  
	  endindex = *index + codelgth - 1;
	  
	  /* Get else instructions */
	  TopSave = Top;
	  while(*index<=endindex)
	    {
	      GetInstruction(data,index,nblines,&newinstr);
	      (*index)++;
	    }
	  nbinstr = Top - TopSave;
	  /* Create list of else instructions */
	  if(data[index0]==8 || data[index0]==10)
	    C2F(mklist)(&nbinstr);
	  
	  (*index)--; /* Index is decremented because is incremented when going back to intmacr2tree() */
	  
	  if(data[index0]==8)
	    {
	      /* Create if tlist */
	      C2F(mktlist)(&n_if_tlist);
	    }
	  else if(data[index0]==9)
	    {
	      /* Create while tlist */
	      C2F(mktlist)(&n_while_tlist);
	    }
	  else if(data[index0]==10)
	    {
	      /* Create select tlist */
	      C2F(mktlist)(&n_select_tlist);
	    }
	}
    }
  return 0;
}

/****************************************************************
 Function name: CreateCsteTList
****************************************************************/
static int CreateCsteTList(char *type,int *data,int *index)
{
  char *cste_tlist[] = {"cste","value"};
  int m_cste_tlist = 1;
  int n_cste_tlist = 2;

  /* Used to get endian */
  int littlendian = 1;
  char *endptr;

  /* Used when type=="emptymatrix" */
  double l_mat = 0;
  int m_mat = 0;
  int n_mat = 0;

  /* Used when type=="string" */
  char **str;
  int *int_str;
  int strlgth = 0;
  int job1 = 1;
  int one = 1;

  /* Used when type=="number" */
  double *value;
  int *ivalue;
  int i = 0; /* Loop index */

  /* First item of returned list */
  str2sci(cste_tlist,m_cste_tlist,n_cste_tlist);

  /* Create data to write in field 'value' */
  if(!strncmp(type,"emptymatrix",11))
    {
      C2F(dtosci)(&l_mat,&m_mat,&n_mat);
    }
  else if(!strncmp(type,"string",6))
    {
      (*index)++;
      strlgth = data[*index];
      
      /* Memory allocation */
      if((str=CALLOC(1,sizeof(char)))==NULL)
	{
	  Scierror(999,"CreateCsteTList: No more memory available\r\n");
	  return 0;
	}
      if((str[0]=(char *)CALLOC(1,sizeof(char)*(strlgth+1)))==NULL)
	{
	  Scierror(999,"CreateCsteTList: No more memory available\r\n");
	  return 0;
	}
      if((int_str=(int *)CALLOC(1,sizeof(int)*(strlgth+1)))==NULL)
	{
	  Scierror(999,"CreateCsteTList: No more memory available\r\n");
	  return 0;
	}
      /* Fill int_str */
      for(i=0;i<strlgth;i++)
	{
	  *index=*index + 1;
	  int_str[i]=data[*index];
	}
      CvStr(&strlgth,int_str,str[0],&job1,strlgth);
      (str[0])[strlgth]='\0';
      str2sci(str,one,one);

      /* Free memory */
      FREE(str[0]);
      str[0]=NULL;
      FREE(str);
      str=NULL;
      FREE(int_str);
      int_str=NULL;
    }

  else if(!strncmp(type,"code23",5))
    {
      strlgth=nlgh;
      /* Memory allocation */
      if((str=CALLOC(1,sizeof(char)))==NULL)
	{
	  Scierror(999,"CreateCsteTList: No more memory available\r\n");
	  return 0;
	}
      if((str[0]=(char *)CALLOC(1,sizeof(char)*(strlgth+1)))==NULL)
	{
	  Scierror(999,"CreateCsteTList: No more memory available\r\n");
	  return 0;
	}

      /* Read name */
      CvNameL(&data[*index+1],str[0],&job1,&strlgth);
      (str[0])[strlgth]='\0';
      *index += nsiz;

      /* Write on stack */
      str2sci(str,one,one);

      /* Free memory */
      FREE(str[0]);
      str[0]=NULL;
      FREE(str);
      str=NULL;
    }
  else if(!strncmp(type,"number",6))
    {
      /* Memory allocation */
      if((value=(double *)CALLOC(1,sizeof(double)))==NULL)
	{
	  Scierror(999,"CreateCsteTList: No more memory available\r\n");
	  return 0;
	}
      ivalue = (int*) value;
      
      /* Get endian */
      endptr = (char *) &littlendian;
      littlendian = (int) *endptr;

      /* Read values in data */
      if(littlendian==1)
	{
	  *index = *index +1;
	  *ivalue = data[*index];
	  *index = *index +1;
	  *(ivalue+1) = data[*index];
	}
      else
	{
	  *index = *index + 1;
	  *(ivalue+1) = data[*index];
	  *index = *index + 1;
	  *ivalue = data[*index];
	}

      C2F(dtosci)(value,&one,&one);
      
      /* Free memory */
      FREE(value);
      value=NULL;
    }
  else /* Should never happen */
    {
      Scierror(999,"CreateCsteTList: wrong type value %s\r\n",type);
      return 0;
    }
  
  /* Create returned list */
  C2F(mktlist)(&n_cste_tlist);

  return 0;
}

/****************************************************************
 Function name: CreateOperationTList
****************************************************************/
static int CreateOperationTList(int *data,int *index)
{
  char *op_tlist[] = {"operation","operands","operator"};
  int m_op_tlist = 1;
  int n_op_tlist = 3;

  /* Operators table */
  char *operators[]={"+","-","*",".*","*.",".*.","/","./","/.","./.",
	       "\\",".\\","\\.",".\\.","^","==","<",">","<=",">=","~=",
	       ":","rc","ins","ext","'","cc","|","&","~",".^",".'","cceol"};
  /* cceol: special operator for column concatenation followed by EOL (initialisation made on more than one line... */

  char **operator;
  int max_op_lgth = 5; /* strlen("cceol") */
  
  int operators_num[32]={45,46,47,98,200,149,48,99,201,150,
			 49,100,202,151,62,50,59,60,109,110,119,
			 44,1,2,3,53,4,57,58,61,113,104};

  int operator_num,operator_index=-1,nb_operands,nb_lhs;

  int k; /* Loop index */

  int orig,dest; /* Used when copy objects */

  int offset = 0;

  int one = 1;

  /* Memory allocation */
  if((operator=CALLOC(1,sizeof(char)))==NULL)
    {
      Scierror(999,"CreateOperationTList: No more memory available\r\n");
      return 0;
    }
  if((operator[0]=(char *)CALLOC(1,sizeof(char)*max_op_lgth+1))==NULL)
    {
      Scierror(999,"CreateOperationTList: No more memory available\r\n");
      return 0;
    }
  (operator[0])[max_op_lgth] = '\0';

  /* Read useful data */
  (*index)++; /* Pass index corresponding to 5 */
  operator_num = data[*index];
  (*index)++;
  nb_operands = data[*index]; /* One or two */
  (*index)++;
  nb_lhs = data[*index]; /* Always one */

  /* Write tlist items names */
  str2sci(op_tlist,m_op_tlist,n_op_tlist);

  /* Search operator index */
  for(k=0;k<32;k++)
    {
    if(operators_num[k]==operator_num)                
	{
	  operator_index=k;
	  break;
	}
    }
  if(operator_index<0) {
    Scierror(999,"CreateOperationTList: unknown operator %d\r\n",operator_num);
    return 0;
  }

  /* Move all operands to next place in stack */
  /* Special case for column concatenation followed by a EOL */
  /*  Example: a=[1,2;
                  3,4] */
  if( (operator_index==26) && (last_eol_pos==Top-2) )
    {
      /* Change operator */
      operator_index = 32;
      
      /* First operand is placed before EOL */
      orig = last_eol_pos - 1;
      dest = Top + 1;
      VCopyObj("CreateOperationTList",&orig,&dest,20L);

      /* Second operand is placed after EOL */
      orig = last_eol_pos + 1;
      dest = Top + 1;
      VCopyObj("CreateOperationTList",&orig,&dest,20L);
      offset = 1;
    }
  else if(operator_index==24) /* For extraction: variable is moved to be the first operand */
    {
      /* Move variable */
      orig = Top - 1;
      dest = Top + 1;
      VCopyObj("CreateOperationTList",&orig,&dest,20L);

      /* Move all indices */
      for(k=nb_operands;k>1;k--)
	{
	  orig = Top - nb_operands - 1;
	  dest = Top + 1;
	  VCopyObj("CreateOperationTList",&orig,&dest,20L);
	}

    }
  else
    {
      for(k=nb_operands;k>0;k--)
	{
	  orig = Top - nb_operands;
	  dest = Top + 1;
	  VCopyObj("CreateOperationTList",&orig,&dest,20L);
	}
    }

  /* Create list of operands */
  C2F(mklist)(&nb_operands);
  
  /* Add operator to stack */
  strcpy(operator[0],operators[operator_index]);
  (operator[0])[strlen(operators[operator_index])]='\0';
  str2sci(operator,one,one);

  /* Create operation tlist */
  C2F(mktlist)(&n_op_tlist);

  /* Move resulting list to first free place in stack */
  orig = Top;
  dest = Top - nb_operands - offset;
  VCopyObj("CreateOperationTList",&orig,&dest,20L);

  return 0;
}

/****************************************************************
 Function name: CreateFuncallTList
****************************************************************/
static int CreateFuncallTList(char *fromwhat,int *data,int *index)
{
  char *fun_tlist[] = {"funcall","rhs","name","lhsnb"};
  int m_fun_tlist = 1;
  int n_fun_tlist = 4;

  /* Used when fromwhat=="funptr" */
  int interf_num,interf_index,funptr;
  int job1 = 1,job2 = 2;
  int id[nsiz];

  double nblhs = 0;
  int nbrhs = 0;

  char **funname;
  int funnamelgth = 0;

  int one = 1;

  int orig,dest; /* Used when copy objects */

  /* Used for empty matrix creation when rhsnb==0 (function called as a command) */
  double l_mat = 0;
  int m_mat = 0;
  int n_mat = 0;

  /* Memory allocation */
  if((funname=CALLOC(1,sizeof(char)))==NULL)
    {
      Scierror(999,"CreateFuncallTList: No more memory available\r\n");
      return 0;
    }
  if((funname[0]=(char *)CALLOC(1,sizeof(char)*(nlgh+1)))==NULL)
    {
      Scierror(999,"CreateFuncallTList: No more memory available\r\n");
      return 0;
    }
  (funname[0])[nlgh]='\0';

  if(!strncmp(fromwhat,"funptr",6))
    {
      interf_num = data[*index];
      (*index)++;
      nbrhs = data[*index];
      (*index)++;
      nblhs = data[*index];
      (*index)++;
      interf_index = data[*index];

      funptr = interf_num + interf_index;

      C2F(funtab)(id,&funptr,&job2);

      CvNameL(id,funname[0],&job1,&funnamelgth);
      (funname[0])[funnamelgth]='\0';
    }
  else if(!strncmp(fromwhat,"datacode",8))
    {
      if(data[*index]==12)
	{
	  strncpy(funname[0],"pause",5);
	  funnamelgth = 5;
	}
      else if(data[*index]==13)
	{
	  strncpy(funname[0],"break",5);
	  funnamelgth = 5;
	}
      else if(data[*index]==14)
	{
	  strncpy(funname[0],"abort",5);
	  funnamelgth = 5;
	}
      else if(data[*index]==17)
	{
	  strncpy(funname[0],"quit",4);
	  funnamelgth = 4;
	}
      else if(data[*index]==20)
	{
	  strncpy(funname[0],"exit",4);
	  funnamelgth = 4;
	}
      else if(data[*index]==28)
	{
	  strncpy(funname[0],"continue",8);
	  funnamelgth = 8;
	}
      else if(data[*index]==99)
	{
	  strncpy(funname[0],"return",6);
	  funnamelgth = 6;
	}
    }
  else if(!strncmp(fromwhat,"macro",5))
    {
      CvNameL(&data[*index-nsiz+1],funname[0],&job1,&funnamelgth);
      (funname[0])[funnamelgth]='\0';

      (*index)++;
      nbrhs = data[*index];
      (*index)++;
      nblhs = data[*index];
    }
  else /* Should never happen */
    {
      Scierror(999,"CreateFuncallTList: wrong fromwhat value %s\r\n",fromwhat);
      return 0;
    }

  /* rhs==0 then function called as a command */
  /* In funcall tree, rhs=[] */
  if(nbrhs==0)
    {
      /* Create an empty matrix on stack */
      C2F(dtosci)(&l_mat,&m_mat,&n_mat);
    }
  else
    {
      /* rhs==-1 then function called with no rhs */
      /* In funcall tree, rhs=list() */
      if(nbrhs<0)
	nbrhs=0;
      /* Create rhs list */
      C2F(mklist)(&nbrhs);
   }

  /* Add tlist items names to stack */
  str2sci(fun_tlist,m_fun_tlist,n_fun_tlist);

  /* Copy rhs list */
  orig = Top - 1;
  dest = Top + 1;
  VCopyObj("CreateFuncallTList",&orig,&dest,18L);

  /* Add funname to stack */
  str2sci(funname,one,one);

  /* Add nblhs to stack */
  C2F(dtosci)(&nblhs,&one,&one);

  /* Create 'funcall' tlist */
  C2F(mktlist)(&n_fun_tlist);

  /* Copy tlist to first free place in stack */
  orig = Top;
  dest = Top - 1;
  VCopyObj("CreateFuncallTList",&orig,&dest,18L);

  /* Free memory */
  FREE(funname[0]);
  funname[0]=NULL;
  FREE(funname);
  funname=NULL;

  return 0;
}

/****************************************************************
 Function name: CreateEqualTList
****************************************************************/
static int CreateEqualTList(char *fromwhat,int *data,int *index)
{
  char *eq_tlist[] = {"equal","expression","lhs","endsymbol"};
  int m_eq_tlist = 1;
  int n_eq_tlist = 4;

  int nblhs = 0,nbrhs = 0;

  int k = 0,l = 0; /* Loop indexes */

  int job1 = 1;

  int orig,dest; /* Used when copy objects */

  char **name;
  int namelgth = 0;

  /* Used for lhs which are insertion operations */
  int indexes_pos;
  int nb_indexes = 0;
  char *op_tlist[] = {"operation","operands","operator"};
  int m_op_tlist = 1;
  int n_op_tlist = 3;

  char **operator;

  int one = 1;

  char **endsymbol;
  int symbol = 0;

  /* Memory allocation */
  if((name=CALLOC(1,sizeof(char)))==NULL)
    {
      Scierror(999,"CreateEqualTList: No more memory available\r\n");
      return 0;
    }
  if((name[0]=(char *)CALLOC(1,sizeof(char)*(nlgh+1)))==NULL)
    {
      Scierror(999,"CreateEqualTList: No more memory available\r\n");
      return 0;
    }
  (name[0])[nlgh] = '\0';

  if((operator=CALLOC(1,sizeof(char)))==NULL)
    {
      Scierror(999,"CreateEqualTList: No more memory available\r\n");
      return 0;
    }
  if((operator[0]=(char *)CALLOC(1,sizeof(char)*4))==NULL)
    {
      Scierror(999,"CreateEqualTList: No more memory available\r\n");
      return 0;
    }
  strcpy(operator[0],"ins");
  (operator[0])[3] = '\0';

  /* Add tlist items names to stack */
  str2sci(eq_tlist,m_eq_tlist,n_eq_tlist);

  if(!strncmp(fromwhat,"code29",6))  /* A code 29 was found in data */
    {
      /* Copy expression */
      orig = Top - 1;
      dest = Top + 1;
      VCopyObj("CreateEqualTList",&orig,&dest,16L);

      indexes_pos = Top - 3;
      
      /* Create list of lhs */
      (*index)++; /* Code 29 is passed */
      nblhs = data[*index];
      (*index)++;
      
      /* Symbol which ends the line: ; , or nothing */
      symbol=data[*index];
      if(symbol==43) /* ; */
	{
	  if((endsymbol=CALLOC(1,sizeof(char)))==NULL)
	    {
	      Scierror(999,"CreateEqualTList: No more memory available\r\n");
	      return 0;
	    }
	  if((endsymbol[0]=(char *)CALLOC(1,sizeof(char)*2))==NULL)
	    {
	      Scierror(999,"CreateEqualTList: No more memory available\r\n");
	      return 0;
	    }
	  strcpy(endsymbol[0],";");
	  (endsymbol[0])[1] = '\0';
	}
      else if(symbol==52) /* , */
	{
	  if((endsymbol=CALLOC(1,sizeof(char)))==NULL)
	    {
	      Scierror(999,"CreateEqualTList: No more memory available\r\n");
	      return 0;
	    }
	  if((endsymbol[0]=(char *)CALLOC(1,sizeof(char)*2))==NULL)
	    {
	      Scierror(999,"CreateEqualTList: No more memory available\r\n");
	      return 0;
	    }
	  strcpy(endsymbol[0],",");
	  (endsymbol[0])[1] = '\0';
	}
      else /* Nothing */
	{
	  if((endsymbol=CALLOC(1,sizeof(char)))==NULL)
	    {
	      Scierror(999,"CreateEqualTList: No more memory available\r\n");
	      return 0;
	    }
	  if((endsymbol[0]=(char *)CALLOC(1,sizeof(char)*1))==NULL)
	    {
	      Scierror(999,"CreateEqualTList: No more memory available\r\n");
	      return 0;
	    }
	  (endsymbol[0])[0] = '\0';
	}
      for(k=0;k<nblhs;k++)
	{
	  (*index)++;
	  CvNameL(&data[*index],name[0],&job1,&namelgth);
	  (name[0])[namelgth] = '\0';
	  *index = *index + nsiz;
	  nbrhs = data[*index];
 	  nb_indexes = nbrhs + nb_indexes;

	  if(nbrhs==0) /* Variable affectation */
	    {
	      CreateVariableTList(name);
	    }
	  else /* Insertion */
	    {
	      /* Write tlist items names */
	      str2sci(op_tlist,m_op_tlist,n_op_tlist);
	      
	      /* Name of variable where data will be inserted */
	      CreateVariableTList(name);
	      
	      /* Indexes for insertion */
	      for(l=0;l<nbrhs;l++)
		{
		  orig = indexes_pos - nbrhs + l + 1;
		  dest = Top + 1;
		  VCopyObj("CreateEqualTList",&orig,&dest,16L);
		}
	      indexes_pos = indexes_pos - nbrhs;
	      
	      /* Create list of operands */
	      nbrhs = nbrhs + 1;
	      C2F(mklist)(&nbrhs);
	      
	      /* Add operator */
	      str2sci(operator,one,one);
	      
	      /* Create operation tlist */
	      C2F(mktlist)(&n_op_tlist);
	    }
	}
      /* Reverse order of lhs */
      for(k=0;k<nblhs;k++)
	{
	  orig = Top - 2 * k;
	  dest = Top + 1;
	  VCopyObj("CreateEqualTList",&orig,&dest,16L);
	}
      
      /* Create list of lhs */
      C2F(mklist)(&nblhs);
      
      /* Copy lhs list to first free place */
      orig = Top;
      dest = Top - nblhs;
      VCopyObj("CreateEqualTList",&orig,&dest,16L);

      /* Symbol */
      str2sci(endsymbol,one,one);
      
      /* Create equal tlist */
      C2F(mktlist)(&n_eq_tlist);
 
      /* Copy tlist to first free place */
      orig = Top;
      dest = Top - nb_indexes - 1;
      VCopyObj("CreateEqualTList",&orig,&dest,16L);
    }
  else if(!strncmp(fromwhat,"code18",6)) /* A code 18 was found in data */
    {
      /* Copy expression */
      orig = Top - 1;
      dest = Top + 1;
      VCopyObj("CreateEqualTList",&orig,&dest,16L);

      (*index)++;
      nblhs++;
      CvNameL(&data[*index],name[0],&job1,&namelgth);
      (name[0])[namelgth]='\0';
      CreateVariableTList(name);
      *index = *index + nsiz;
      *index = *index - 1;
      
      /* Create list of lhs */
      C2F(mklist)(&nblhs);

      /* Symbol */
      if((endsymbol=CALLOC(1,sizeof(char)))==NULL)
	{
	  Scierror(999,"CreateEqualTList: No more memory available\r\n");
	  return 0;
	}
      if((endsymbol[0]=(char *)CALLOC(1,sizeof(char)*1))==NULL)
	{
	  Scierror(999,"CreateEqualTList: No more memory available\r\n");
	  return 0;
	}
      (endsymbol[0])[0] = '\0';
      str2sci(endsymbol,one,one);
      
      /* Create equal tlist */
      C2F(mktlist)(&n_eq_tlist);

      /* Copy equal tlist */
      orig = Top;
      dest = Top - 1;
      VCopyObj("CreateEqualTList",&orig,&dest,16L);
   }
  else if(!strncmp(fromwhat,"code1",5)) /* A code 1 was found in data (should no more exist) */
    {
      /* Copy expression */
      orig = Top - 1;
      dest = Top + 1;
      VCopyObj("CreateEqualTList",&orig,&dest,16L);

      while(data[*index]==1)
	{
	  (*index)++;
	  nblhs++;
	  CvNameL(&data[*index],name[0],&job1,&namelgth);
	  (name[0])[namelgth]='\0';
	  CreateVariableTList(name);
	  *index = *index + nsiz;
	  (*index)++; /* Code 22 (print mode) is ignored */
	  (*index)++; /* Code 99 is ignored */
	}
      /* Create list of lhs */
      C2F(mklist)(&nblhs);

      /* Symbol */
      if((endsymbol=CALLOC(1,sizeof(char)))==NULL)
	{
	  Scierror(999,"CreateEqualTList: No more memory available\r\n");
	  return 0;
	}
      if((endsymbol[0]=(char *)CALLOC(1,sizeof(char)*1))==NULL)
	{
	  Scierror(999,"CreateEqualTList: No more memory available\r\n");
	  return 0;
	}
      (endsymbol[0])[0] = '\0';
      str2sci(endsymbol,one,one);
      
      /* Create equal tlist */
      C2F(mktlist)(&n_eq_tlist);
    }
  else if(!strncmp(fromwhat,"forexpr",7))
    {
      /* Copy expression */
      orig = Top - 2;
      dest = Top + 1;
      VCopyObj("CreateEqualTList",&orig,&dest,16L);

      /* Copy variable after tlist items */
      orig = Top - 2;
      dest = Top + 1;
      VCopyObj("CreateEqualTList",&orig,&dest,16L);

      nblhs = 1;
      /* Create list of lhs */
      C2F(mklist)(&nblhs);

      /* Symbol */
      if((endsymbol=CALLOC(1,sizeof(char)))==NULL)
	{
	  Scierror(999,"CreateEqualTList: No more memory available\r\n");
	  return 0;
	}
      if((endsymbol[0]=(char *)CALLOC(1,sizeof(char)*1))==NULL)
	{
	  Scierror(999,"CreateEqualTList: No more memory available\r\n");
	  return 0;
	}
      (endsymbol[0])[0] = '\0';
      str2sci(endsymbol,one,one);
      
      /* Create equal tlist */
      C2F(mktlist)(&n_eq_tlist);

      /* Copy tlist to first free place */
      dest = Top - 2;
      orig = Top;
      VCopyObj("CreateEqualTList",&orig,&dest,16L);
    }
  else /* Should not happen */
    {
      Scierror(999,"CreateEqualTList: wrong fromwhat value %s\r\n",fromwhat);
      return 0;
    }

  /* Free memory */
  FREE(name[0]);
  name[0]=NULL;
  FREE(name);
  name=NULL;
  FREE(operator[0]);
  operator[0]=NULL;
  FREE(operator);
  operator=NULL;
  FREE(endsymbol[0]);
  endsymbol[0]=NULL;
  FREE(endsymbol);
  endsymbol=0;

  return 0;
}
/****************************************************************
 Function name: CreateCommentTList
****************************************************************/
static int CreateCommentTList(int *data,int *index)
{
  char *fun_tlist[] = {"comment","text"};
  int m_fun_tlist = 1;
  int n_fun_tlist = 2;

  int strlgth;

  char *text=NULL;
  int job1 = 1;

  int one = 1;

   /* First item of returned list */
  str2sci(fun_tlist,m_fun_tlist,n_fun_tlist);

  /* Create data to write in field 'text' */
  (*index)++;
  strlgth = data[*index];
  (*index)++;   
  /* Memory allocation */
  if((text=(char *)CALLOC(1,sizeof(char)*(strlgth+1)))==NULL)
    {
      Scierror(999,"CreateCsteTList: No more memory available\r\n");
      return 0;
    }
  CvStr(&strlgth,&(data[*index]),text,&job1,strlgth);
  text[strlgth]='\0';
  str2sci(&text,one,one);
  *index = *index + strlgth-1;
  /* Free memory */
  FREE(text);
  text=NULL;

  C2F(mktlist)(&n_fun_tlist);
  return 0;
}

/****************************************************************
 Function name: CreateRecursiveIndexList
****************************************************************/
static int CreateRecursiveIndexList(int *data,int *index)
{
  int m,n;

  /* Get infos in data */
  (*index)++;
  n = data[*index];
  (*index)++;
  m = data[*index];

  if(m>1)
    {
      C2F(mklist)(&m);
    }
  if(n!=0)
    {
      C2F(mklist)(&n);
    }

  return 0;
}

/***********************************************************
 Copy an object in Scilab stack

 fname: name of function from where VCopyObj was called
 orig: position of object to copy
 dest: position where object has to be copied
 fname_length: length of character string fname
***********************************************************/
static int VCopyObj(char *fname,int *orig,int *dest,unsigned long fname_length)
{
  C2F(vcopyobj)(fname,orig,dest,fname_length);
  Top = *dest;
  return 0;
}

/****************************************************************
 Function name: complexity
****************************************************************/
int complexity(int *data,int *index,int *lgth)
{
  int count = 0;
  
  int cur_ind = *index+1;

  int last_eol;

  int nbop = 0; /* Number of value stored on stack */

  while(cur_ind<=*lgth+*index)
    {
      switch(data[cur_ind])
	{
	case 0: /* Deleted operation */
	  cur_ind = cur_ind + data[cur_ind+1] + 1;
	  break;
	case 1: /* Stack put (Obsolete) */
	  cur_ind = cur_ind + nsiz + 1;
	  count++;
	  break;
	case 2: /* Stack get */
	  cur_ind = cur_ind + nsiz + 3;
	  nbop++;
	  break;
	case 3: /* String */
	  cur_ind = cur_ind + 2 + data[cur_ind+1];
	  nbop++;
	  break;
	case 4: /* Empty matrix */
	  cur_ind++;
	  nbop++;
	  break;
	case 5: /* Operations */
	  if( (data[cur_ind+1]==4) && (last_eol==nbop-2) ) /* rc with a EOL */
	    {
	      nbop--;
	      count--;
	    }
	  nbop = nbop - data[cur_ind+2];
	  cur_ind = cur_ind + 4;
	  nbop++;
	  break;
	case 6: /* Number */
	  cur_ind = cur_ind + 3;
	  nbop++;
	  break;
	case 7: /* 'for' control instruction */
	  cur_ind = cur_ind + data[cur_ind+1] + 2;
	  cur_ind = cur_ind + 1 + nsiz + data[cur_ind];
	  count++;
	  break;
	case 8: /* 'if-then-else' control instruction */
	  if(data[cur_ind+1]>0)
	    {
	      cur_ind = cur_ind + 2;
	      cur_ind = cur_ind + 3 + data[cur_ind] + data[cur_ind+1] + data[cur_ind+2];
	    }
	  else
	    {
	      cur_ind = cur_ind - data[cur_ind+1];
	    }
	  count++;
	  break;
	case 9: /* 'while' control instruction */
	  if(data[cur_ind+1]>0)
	    {
	      cur_ind = cur_ind + 2;
	      cur_ind = cur_ind + 3 + data[cur_ind] + data[cur_ind+1] + data[cur_ind+2];
	    }
	  else
	    {
	      cur_ind = cur_ind - data[cur_ind+1];
	    }
	  count++;
	  break;
	case 10: /* 'select-case' control instruction */
	  cur_ind = cur_ind + data[cur_ind+1];
	  count++;
	  break;
	case 11: /* 'try-catch' control instruction */
	  cur_ind = cur_ind + data[cur_ind+1] + data[cur_ind+2] + 3;
	  count++;
	  break;
	case 12: /* pause */
	  cur_ind++;
	  count++;
	  break;
	case 13: /* break */
	  cur_ind++;
	  count++;
	  break;
	case 14: /* abort */
	  cur_ind++;
	  count++;
	  break;
	case 15: /* EOL */
	  cur_ind++;
	  last_eol = nbop;
	  nbop++;
	  count++;
	  break;
	case 16: /* Set line number */
	  cur_ind = cur_ind + 2;
	  break;
	case 17: /* quit (Should never append) */
	  cur_ind++;
	  count++;
	  break;
	case 18: /* Mark named variable */
	  cur_ind = cur_ind + 1 + nsiz;
	  break;
	case 19: /* Form recursive index list */
	  nbop = nbop - data[cur_ind+1] + 1;
	  cur_ind = cur_ind + 3;
	  break;
	case 20: /* exit */
	  cur_ind++;
	  count++;
	  break;
	case 21: /* Beginning of rhs */
	  cur_ind = cur_ind + 1;
	  break;
	case 22: /* Set print mode (ignored ?) */
	  cur_ind = cur_ind + 2;
	  break;
	case 23: /* Create variable from name */
	  cur_ind = cur_ind + 1 + nsiz;
	  nbop++;
	  break;
	case 24: /* Create an object with type 0 */
	  cur_ind = cur_ind + 1;
	  break;
	case 25: /* Compute profiling data */
	  cur_ind = cur_ind + 3;
	  break;
	case 26: /* Vector of strings */
	  cur_ind = cur_ind + 5 + data[cur_ind+1]*data[cur_ind+2] + data[cur_ind+4+data[cur_ind+1]*data[cur_ind+2]] - 1;
	  break;
	case 27: /* varfunptr ??? */
	  cur_ind = cur_ind + 3 + nsiz;
	  break;
	case 28: /* continue */
	  cur_ind++;
	  count++;
	  break;
	case 29: /* Affectation */
	  nbop = 0;
	  cur_ind = cur_ind + 2 + (data[cur_ind+1])*(nsiz+1) + 1;
	  count++;
	  break;
	case 30: /* Expression evaluation short circuiting */
	  /* This code is ignored */
	  cur_ind = cur_ind + 3;
	  break;
	case 31: /* comment */
	  cur_ind = cur_ind + 2 + data[cur_ind+1];
	  count++;
	  break;
	case 99: /* return */
	  cur_ind++;
	  count++;
	  break;
	default:
	  if(data[cur_ind]/100*100==data[cur_ind])
	    {
	      cur_ind = cur_ind + 4;
	    }
	  else
	    {
	      sciprint("complexity: wrong code %d\r\n",data[cur_ind]);
	      return -1;
	    }
	  break;
	}
    }
  return count;
  
}
