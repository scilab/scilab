/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/**************************************************/
/* intmacr2tree.c                                 */
/* Functions used for macr2tree() Scilab function */
/**************************************************/
#include "MALLOC.h"

#include "intmacr2tree.h"
#include "sciprint.h"
#include "cvstr.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "hashtable_core.h"

/* Table to store variable names */
static char varnames[isizt][nlgh+1];

/* Number of variables */
static int nbvars = 0;

/* Store stack index for last EOL */
static int last_eol_pos = 0;

/* The position of the first item which can be used as input argument or operand */
static int beginStorage = -10;

/****************************************************************/
static int CreateRecursiveIndex2List(int *data,int *index2);
/****************************************************************
 Function name: macr2tree
****************************************************************/
int C2F(macr2tree) (char *fname,unsigned long fname_len)
{
  /* Returned value parameters */
  int m_pgrm_tlist = 1,n_pgrm_tlist = 6;
  char *pgrm_tlist[] = {"program","name","outputs","inputs","statements","nblines"};

  int *stkdata = NULL; /* Pointeur to rhs arguments */
  int *data = NULL; /* Macro int vector (pointer to copy of rhs argument) */

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

  int deleted = 0;

  /* Loop index */
  int k = 0;

  /* Used for statements list creation */
  int sz = 0; /* Size */
  int newinstr = 0; /* flag used to know if a new instruction has been created (1->TRUE) */

  /* Verify number of RHS arguments */
  CheckRhs(1,1);

  /* Verify number of LHS arguments */
  CheckLhs(1,1);

  /* Read all data */
  stkdata = (int *) stk(*Lstk(Top));

  if (stkdata[0] > 0) /* Not a reference to variable */
    {
		Scierror(999,_("%s: Wrong type for input argument #%d: Named variable expected.\n"),"macr2tree",1);
      return 0;
    }
  else
    {
      stkdata = (int *) stk(stkdata[1]);
    }

  /* Verify good type for input: must be a compiled macro (type 13) */
  if(stkdata[0] != 13)
    {
		Scierror(999,_("%s: Wrong type for input argument #%d: Compiled macro expected.\n"),"macr2tree",1);
      return 0;
    }

  /* Memory allocation */
  if((name=CALLOC(1,sizeof(char*)))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"macr2tree");
      return 0;
    }
  if((name[0]=(char *)CALLOC(1,sizeof(char)*(nlgh+1)))==NULL)
    {
		Scierror(999,_("%s: Out of code.\n"),"macr2tree");
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
	  Scierror(999,_("%s: No more memory.\n"),"macr2tree");
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

  /* Error handling (S. Steer) */
  if (*Lstk(Top+1) >= *Lstk(Bot))
  {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), "macr2tree");
    
    /* Free memory */
    freeArrayOfString(name,1);
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

          if (cod_sav==15)
            {
              if (beginStorage>0) /* Inside a list of inputs or operands */
                {
                  newinstr=0; /* No new instruction created */
                }
            }
          else if (cod_sav==31)
            {
              if (beginStorage>0) /* Inside a list of inputs or operands */
                {
                  newinstr=0; /* No new instruction created */
                }
            }
          else
            {
              if (newinstr==1)
                {
                  /* Reinit */
                  beginStorage = -10;
                  last_eol_pos=-10;
                }
              else if (beginStorage <= 0 && cod_sav!=16 && cod_sav!=21 && cod_sav!=0)
                {
                  /* Store position */
                  beginStorage = Top - 1;
                }
            }

	  /* Error handling (S. Steer) */
	  if (Err>0 || C2F(errgst).err1>0)
	    {
	      /* Free memory */
              freeArrayOfString(name,1);
	      FREE(data);

	      return 0;
	    }

 	  if(cod_sav==15 && (data[cod_ind+1]==29 || (data[cod_ind+1]==2 && data[cod_ind+3+nsiz]!=0))) /* EOL as the last component of a column concatenation */
	    {
	      /* a = ['a'
		 'b'
		 ] */
	      Top--; /* EOL is erased */
              newinstr=0; /* No new instruction created */
              deleted ++;
	    }

	  cod_ind++;
	  if(cod_ind>codelength+ilt+1)
	  {
            Scierror(999,_("%s: Out of code.\n"),"macr2tree");
            
            /* Free memory */
            freeArrayOfString(name,1);
            FREE(data);
            return 0;
	  }

	}

      /* Test to be sure an instruction was created */
      if(TopSave!=Top-1)
        {
          Scierror(999,_("%s: Wrong %s value %d instead of %d.\n"),"macr2tree","Top",Top,TopSave+1);
          
          /* Free memory */
          freeArrayOfString(name,1);
          FREE(data);
          
          return 0;
        }
      

      /* Size of the element created in the list */
      sz = *Lstk(Top+1) - *Lstk(Top);
      
      /* Update the "pointer" in the list header */
      *istk(il+2+k) = *istk(il+1+k) + sz ;
      
      /* Now element is added to the list, Scilab can ignore it */
      Top--;
      
      /* Update address to write next value */
      *Lstk(Top+1) = *Lstk(Top+2);

      /* If a EOL was ignored (before affectation) it is added just after */
      if (deleted)
        {
          k++;

          CreateEOLList();

          sz = *Lstk(Top+1) - *Lstk(Top);
          *istk(il+2+k) = *istk(il+1+k) + sz ;
          Top--;
          *Lstk(Top+1) = *Lstk(Top+2);
         
          deleted = 0;
        }

    }

  /* Number of lines */
  C2F(itosci)(&nblines,&one,&one);

  C2F(mktlist)(&n_pgrm_tlist);

  /* Free memory */
  freeArrayOfString(name,1);
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
  if((eol=CALLOC(1,sizeof(char*)))==NULL)
    {
	  Scierror(999,_("%s: No more memory.\n"),"CreateEOLList");
      return 0;
    }
  if((eol[0]=(char *)CALLOC(1,sizeof(char)*(strlen("EOL")+1)))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"CreateEOLList");
      return 0;
    }
  (eol[0])[3]='\0';
  strncpy(eol[0],"EOL",3);

  /* Add eol to stack */
  str2sci(eol,one,one);

  /* Create list */
  C2F(mklist)(&one);

  /* Free memory */
  freeArrayOfString(eol, 1);

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
  int index2 = -1;
  int k;
  int maxlgth;

  for(k=0;k<isizt;k++)
    {
      if(strlen(name)>=strlen(varnames[k]))
	{
	  maxlgth=(int)strlen(name);
	}
      else
	{
	  maxlgth=(int)strlen(varnames[k]);
	}
      if(varnames[k][0]=='\0')
	{
	  index2 = -1;
	  break;
	}
      else if(!strncmp(name,varnames[k],maxlgth))
	{
	  index2 = k;
	  break;
	}
    }
  return index2;
}

/****************************************************************
 Function name: GetInstruction
****************************************************************/
static int GetInstruction(int *data,int *index2,int *nblines,int *addinstr)
{
  int job1 = 1;

  char **name;
  int namelgth;

  *addinstr=0;

  /* Memory allocation */
  if((name=CALLOC(1,sizeof(char*)))==NULL)
    {
		Scierror(999,_("%s: No more memory.\n"),"GetInstruction");
        return 0;
    }
  if((name[0]=(char *)CALLOC(1,sizeof(char)*(nlgh+1)))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"GetInstruction");
      return 0;
    }
  (name[0])[nlgh]='\0';

  switch(data[*index2]) {
  case 0: /* Deleted operation */
    /* This code is ignored */
    *index2 += data[*index2+1]-1;;
    break;
  case 1: /* Stack put (Obsolete) */
    CreateEqualTList("code1",data,index2);
    *addinstr=1;
    break;
  case 2: /* Stack get */
    /* Read name */
    CvNameL(&data[*index2+1],name[0],&job1,&namelgth);
    (name[0])[namelgth]='\0';
    *index2 += nsiz;
    if(data[*index2+2]==0) /* stack get (rhs=0) */
      {
	CreateVariableTList(name);
	*index2 += 2;
     }
    else
      {
	if( (IsDefinedVar(name[0])>=0) || ( (data[*index2+1]==-3) && (data[*index2+2]!=0) ) )
	  {
	    /* Stack get for extraction from variable */
	    CreateVariableTList(name);
	    *index2 += 2;
	  }
	else
	  {
	    /* Macro call */
	    data[*index2+1] = data[*index2+2];
	    if(data[*index2+3]==5 && data[*index2+4]==3) /* 3=code for extraction */
	      /* If next instruction is an extraction, it is ignored */
	      {
		data[*index2+2] = data[*index2+6]; /* Replace number of lhs for macro call by the one of extraction */
		CreateFuncallTList("macro",data,index2);
		*index2 += 4;
	      }
	    else
	      {
		data[*index2+2] = 1;
		CreateFuncallTList("macro",data,index2);
		*index2 += 4;
	      }
	  }
      }
    break;
  case 3: /* String */
    if(data[*index2 + data[*index2+1] + 2] == 26)
      {
        CreateInlineTList(data, index2, nblines, addinstr);
        *addinstr=1;
      }
    else
      {
        CreateCsteTList("string",data,index2);
      }
    break;
  case 4: /* Empty matrix */
    CreateCsteTList("emptymatrix",data,index2);
    break;
  case 5: /* Operations */
    if(data[*index2+2]==0)
      {
        *index2 +=3;
        break;
      }
    CreateOperationTList(data,index2);
    break;
  case 6: /* Number */
    CreateCsteTList("number",data,index2);
    break;
  case 7: /* 'for' control instruction */
    GetControlInstruction(data,index2,nblines);
    *addinstr=1;
    break;
  case 8: /* 'if-then-else' control instruction */
    GetControlInstruction(data,index2,nblines);
    *addinstr=1;
    break;
  case 9: /* 'while' control instruction */
    GetControlInstruction(data,index2,nblines);
    *addinstr=1;
   break;
  case 10: /* 'select-case' control instruction */
    GetControlInstruction(data,index2,nblines);
    *addinstr=1;
    break;
  case 11: /* 'try-catch' control instruction */
    GetControlInstruction(data,index2,nblines);
    *addinstr=1;
    break;
  case 12: /* pause */
  case 13: /* break */
  case 14: /* abort */
    CreateFuncallTList("datacode",data,index2);
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
    (*index2)++;
    break;
  case 17: /* quit (Should never append) */
    CreateFuncallTList("datacode",data,index2);
    *addinstr=1;
    break;
  case 18: /* Mark named variable */
    CreateEqualTList("code18",data,index2);
    break;
  case 19: /* Form recursive index2 list */
    CreateRecursiveIndex2List(data,index2);
    break;
  case 20: /* exit */
    CreateFuncallTList("datacode",data,index2);
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
    CreateCsteTList("code23",data,index2);
    break;
  case 24: /* Create an object with type 0 */
    Scierror(999,_("%s: code %d not yet implemented.\n"),"GetInstruction",data[*index2]);
    break;
  case 25: /* Compute profiling data */
 /* This code is ignored */
    *index2 += 2;
    break;
  case 26: /* Vector of strings */
    CreateInlineTList(data, index2, nblines, addinstr);
    *addinstr=1;
   break;
  case 27: /* varfunptr */
    Scierror(999,_("%s: code %d not yet implemented.\n"),"GetInstruction",data[*index2]);
    break;
  case 28: /* continue */
    CreateFuncallTList("datacode",data,index2);
    *addinstr=1;
    break;
  case 29: /* Affectation */
    CreateEqualTList("code29",data,index2);
    *addinstr=1;
    break;
  case 30: /* Expression evaluation short circuiting */
    /* This code is ignored */
    *index2 += 2;
    break;
  case 31: /* comment */
    CreateCommentTList(data,index2);
    *addinstr=1;
    break;

  case 99: /* return */
    CreateFuncallTList("datacode",data,index2);
    *addinstr=1;
    break;
  default:
    if(data[*index2]/100*100==data[*index2] && data[*index2]!=0)
      {
	/* funptr */
	CreateFuncallTList("funptr",data,index2);
      }
    else
      {
	Scierror(999,_("%s: Unknown code %d at index2 %d.\n"),"GetInstruction",data[*index2],*index2 );
	return 0;
      }
    break;
  }

  /* Free memory */
  freeArrayOfString(name, 1);

  return 0;
}

/****************************************************************
 Function name: GetControlInstruction
****************************************************************/
static int GetControlInstruction(int *data,int *index2,int *nblines)
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
  int index20,endindex2;
  int codelgth;
  int ncase = 0,icase = 0;
  int TopSave = 0,TopSave_elseifsorcases=0;
  int nbinstr = 0;
  int nbelseifsorcases = 0;

  int newinstr=0; /* Used to call GetInstruction with enough parameters */

  /* FOR */
  if(data[*index2]==7)
    {
      /* Write list items */
      str2sci(for_tlist,m_for_tlist,n_for_tlist);
      (*index2)++;
      codelgth = data[*index2];
      endindex2 = *index2 + codelgth;
      (*index2)++;

      /* Get expression */
      while(*index2<=endindex2)
	{
	  GetInstruction(data,index2,nblines,&newinstr);
	  (*index2)++;
	}

      codelgth = data[*index2];
      (*index2)++;

      /* Get loop variable */
      /* Memory allocation */
      if((name=CALLOC(1,sizeof(char*)))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"GetControlInstruction");
	  return 0;
	}
      if((name[0]=(char *)CALLOC(1,sizeof(char)*(nlgh+1)))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"GetControlInstruction");
	  return 0;
	}
      (name[0])[nlgh]='\0';

      CvNameL(&data[*index2],name[0],&job1,&namelgth);
      (name[0])[namelgth]='\0';
      *index2 += nsiz;
      /* Create a variable tlist with name */
      CreateVariableTList(name);

      /* variable = expression */
      CreateEqualTList("forexpr",data,index2);
      endindex2 = *index2 + codelgth;

      /* Get all instructions */
      TopSave = Top;
      while(*index2<=endindex2)
	{
	  /* Get all instructions */
	  GetInstruction(data,index2,nblines,&newinstr);
	  (*index2)++;
	}
      (*index2)--;
      /* Make list of instructions */
      nbinstr = Top - TopSave;
      C2F(mklist)(&nbinstr);

      /* Create FOR tlist */
      C2F(mktlist)(&n_for_tlist);

      /* Free memory */
      freeArrayOfString(name, 1);
    }
    /* TRYCATCH */
  else if(data[*index2]==11)
    {
      index20 = *index2;

      str2sci(trycatch_tlist,m_trycatch_tlist,n_trycatch_tlist);

      /* index2 now point to first code to use as an instruction code */
      *index2 += 3;

      codelgth = data[index20+1];
      endindex2 = *index2 + codelgth - 1;

      TopSave = Top;
      /* Get try instructions */
      while(*index2<=endindex2)
	{
	  GetInstruction(data,index2,nblines,&newinstr);
	  (*index2)++;
	}

      nbinstr = Top - TopSave;

      /* Create list of try instructions */
      C2F(mklist)(&nbinstr);
      last_eol_pos = -10;

      codelgth = data[index20+2];
      endindex2 = *index2 + codelgth - 1;

      TopSave = Top;
      /* Get catch instructions */
      while(*index2<=endindex2)
	{
	  GetInstruction(data,index2,nblines,&newinstr);
	  (*index2)++;
	}

      nbinstr = Top - TopSave;

      /* Create list of catch instructions */
      C2F(mklist)(&nbinstr);

     (*index2)--;

      /* Create trycatch tlist */
      C2F(mktlist)(&n_trycatch_tlist);
    }
  /* IF - WHILE - SELECT */
  else
    {
      index20 = *index2;

      /* if or while of Scilab version < 3 */
      if( (data[*index2]==8 || data[*index2]==9) && data[*index2+1]>=0 )
	{
	  /* This part will not be written */
	  /* No more used */
	  Scierror(999,_("%s: Old version of if and while not implemented.\n"),"GetControlInstruction");
	  return 0;
	}
      else
	{
	  ncase = data[index20+2]; /* Number of elseif + number of else = number of elseif + 1 */

	  /* Write first tlist item (tlist fields) */
	  if(data[index20]==8)
	    {
	       str2sci(if_tlist,m_if_tlist,n_if_tlist);
	    }
	  else if(data[index20]==9)
	    {
	      str2sci(while_tlist,m_while_tlist,n_while_tlist);
	    }
	  else if(data[index20]==10)
	    {
	      str2sci(select_tlist,m_select_tlist,n_select_tlist);
	    }

	  /* index2 now point to first code to use as an instruction code */
	  *index2 += 4;

	  codelgth = data[index20+3];
	  endindex2 = *index2 + codelgth - 1;

	  icase = ncase + 1;

	  /* If control instruction is a select, I get expression */
	  if(data[index20]==10)
	    {
	      TopSave = Top;
	      while(*index2<=endindex2)
		{
		  GetInstruction(data,index2,nblines,&newinstr);
		  (*index2)++;
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
	      if(icase==ncase && data[index20]==10)
      		TopSave_elseifsorcases = Top; /* Saved to know how many cases have been written */

	      if(icase<ncase && data[index20]==8) /* For a if (if loop already executed one time): found a elseif */
		{
		  /* Write tlist items */
		  str2sci(elseif_tlist,m_elseif_tlist,n_elseif_tlist);
		}

	      if(data[index20]==10) /* For a select: found a case */
		{
		  /* Write tlist items */
		  str2sci(case_tlist,m_case_tlist,n_case_tlist);
		}

	      codelgth = data[*index2];
	      (*index2)++;
	      endindex2 = *index2 + codelgth - 1;

	      /* Get expression */
              TopSave = Top;
	      while(*index2<=endindex2)
		{
		  GetInstruction(data,index2,nblines,&newinstr);
		  (*index2)++;
		}
	      last_eol_pos = -10;
	      codelgth = data[*index2];
	      (*index2)++;
	      endindex2 = *index2 + codelgth - 1;

	      /* Get then instructions */
	      TopSave = TopSave + 1; /* Position on stack saved to get the number of instructions */
	      while(*index2<=endindex2)
		{
		  GetInstruction(data,index2,nblines,&newinstr);
		  (*index2)++;
		}
	      nbinstr = Top - TopSave;
	      /* Create list of then instructions */
	      C2F(mklist)(&nbinstr);

	      if(icase<ncase && data[index20]==8) /* IF: create elseif tlist */
		{
		  /* Create elseif tlist */
		  C2F(mktlist)(&n_elseif_tlist);
		}

	      if(data[index20]==10) /* SELECT: create case tlist */
		{
		  /* Create case tlist */
		  C2F(mktlist)(&n_case_tlist);
		}

	      /* IF: after first pass, save position on stack to get the number of elseifs */
	      if(icase==ncase && data[index20]==8)
		TopSave_elseifsorcases = Top; /* Saved to know how many elseifs have been written */
	    }

	  nbelseifsorcases = Top - TopSave_elseifsorcases;

	  /* IF: create list of elseifs */
	  /* SELECT: create list of cases */
	  if(data[index20]==8 || data[index20]==10)
	    C2F(mklist)(&nbelseifsorcases);

	  /* else (if there is one) (not used for WHILE) */
	  (*index2)++;
	  codelgth = data[*index2];
	  (*index2)++;

	  if(codelgth==0) /* When no else in IF or SELECT and when a WHILE */
	    (*index2)++;

	  endindex2 = *index2 + codelgth - 1;

	  /* Get else instructions */
	  TopSave = Top;
	  while(*index2<=endindex2)
	    {
	      GetInstruction(data,index2,nblines,&newinstr);
	      (*index2)++;
	    }
	  nbinstr = Top - TopSave;
	  /* Create list of else instructions */
	  if(data[index20]==8 || data[index20]==10)
	    C2F(mklist)(&nbinstr);

	  (*index2)--; /* Index2 is decremented because is incremented when going back to intmacr2tree() */

	  if(data[index20]==8)
	    {
	      /* Create if tlist */
	      C2F(mktlist)(&n_if_tlist);
	    }
	  else if(data[index20]==9)
	    {
	      /* Create while tlist */
	      C2F(mktlist)(&n_while_tlist);
	    }
	  else if(data[index20]==10)
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
static int CreateCsteTList(char *type,int *data,int *index2)
{
  char *cste_tlist[] = {"cste","value"};
  int m_cste_tlist = 1;
  int n_cste_tlist = 2;

  /* Used to get endian */
  int littlendian = 1;
  char *endptr = NULL;

  /* Used when type=="emptymatrix" */
  double l_mat = 0;
  int m_mat = 0;
  int n_mat = 0;

  /* Used when type=="string" */
  char **str = NULL;
  int *int_str = NULL;
  int strlgth = 0;
  int job1 = 1;
  int one = 1;

  /* Used when type=="number" */
  double *value;
  int *ivalue;
  int i = 0; /* Loop index2 */

  /* First item of returned list */
  str2sci(cste_tlist,m_cste_tlist,n_cste_tlist);

  /* Create data to write in field 'value' */
  if(!strncmp(type,"emptymatrix",11))
    {
      C2F(dtosci)(&l_mat,&m_mat,&n_mat);
    }
  else if(!strncmp(type,"string",6))
    {
      (*index2)++;
      strlgth = data[*index2];

      /* Memory allocation */
      if((str=CALLOC(1,sizeof(char*)))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"CreateCsteTList");
	  return 0;
	}
      if((str[0]=(char *)CALLOC(1,sizeof(char)*(strlgth+1)))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"CreateCsteTList");
	  return 0;
	}
      if((int_str=(int *)CALLOC(1,sizeof(int)*(strlgth+1)))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"CreateCsteTList");
	  return 0;
	}
      /* Fill int_str */
      for(i=0;i<strlgth;i++)
	{
	  *index2=*index2 + 1;
	  int_str[i]=data[*index2];
	}
      CvStr(&strlgth,int_str,str[0],&job1,strlgth);
      (str[0])[strlgth]='\0';
      str2sci(str,one,one);

      /* Free memory */
      freeArrayOfString(str, 1);
      FREE(int_str);
      int_str=NULL;
    }

  else if(!strncmp(type,"code23",5))
    {
      strlgth=nlgh;
      /* Memory allocation */
      if((str=CALLOC(1,sizeof(char*)))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"CreateCsteTList");
	  return 0;
	}
      if((str[0]=(char *)CALLOC(1,sizeof(char)*(strlgth+1)))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"CreateCsteTList");
	  return 0;
	}

      /* Read name */
      CvNameL(&data[*index2+1],str[0],&job1,&strlgth);
      (str[0])[strlgth]='\0';
      *index2 += nsiz;
      /* Write on stack */
      str2sci(str,one,one);

      /* Free memory */
      freeArrayOfString(str, 1);
    }
  else if(!strncmp(type,"number",6))
    {
      /* Memory allocation */
      if((value=(double *)CALLOC(1,sizeof(double)))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"CreateCsteTList");
	  return 0;
	}
      ivalue = (int*) value;

      /* Get endian */
      endptr = (char *) &littlendian;
      littlendian = (int) *endptr;

      /* Read values in data */
      if(littlendian==1)
	{
	  *index2 = *index2 +1;
	  *ivalue = data[*index2];
	  *index2 = *index2 +1;
	  *(ivalue+1) = data[*index2];
	}
      else
	{
	  *index2 = *index2 + 1;
	  *(ivalue+1) = data[*index2];
	  *index2 = *index2 + 1;
	  *ivalue = data[*index2];
	}
      C2F(dtosci)(value,&one,&one);

      /* Free memory */
      FREE(value);
      value=NULL;
    }
  else /* Should never happen */
    {
	  Scierror(999,_("%s: Wrong type value.\n"),"CreateCsteTList");
      return 0;
    }

  /* Create returned list */
  C2F(mktlist)(&n_cste_tlist);

  return 0;
}

/****************************************************************
 Function name: CreateInlineTList
****************************************************************/
static int CreateInlineTList(int *data,int *index2, int *nblines, int *addinstr)
{
  char *inline_tlist[] = {"inline","prototype","definition"};
  int m_inline_tlist = 1;
  int n_inline_tlist = 3;

  int k = 0, i = 0; // Loop indices
  int nCols = 0; // Number of columns
  int nRows = 0; // Number of rows

  int *lengths = NULL; // Elements lengths
  int maxlength = 0;

  char **str = NULL;
  int *int_str = NULL;

  int job1 = 1;
  
  int strlgth = 0;
  int one = 1;
  
  /* First item of returned list */
  str2sci(inline_tlist,m_inline_tlist,n_inline_tlist);

  /* Get function prototype */
  (*index2)++;
  strlgth = data[*index2];
  
  /* Memory allocation */
  if((str=CALLOC(1,sizeof(char*)))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"CreateInlineTList");
      return 0;
    }
  if((str[0]=(char *)CALLOC(1,sizeof(char)*(strlgth+1)))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"CreateInlineTList");
      return 0;
    }
  if((int_str=(int *)CALLOC(1,sizeof(int)*(strlgth+1)))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"CreateInlineTList");
      return 0;
    }
  /* Fill int_str */
  for(i=0;i<strlgth;i++)
    {
      *index2=*index2 + 1;
      int_str[i]=data[*index2];
    }
  CvStr(&strlgth,int_str,str[0],&job1,strlgth);
  (str[0])[strlgth]='\0';
  str2sci(str,one,one);
  
  /* Free memory */
  freeArrayOfString(str, 1);
  FREE(int_str);
  int_str=NULL;
  
  /* Read function definition */
  
  (*index2)++; // Go to code 26
  (*index2)++; // Ignore code 26
  
  nCols = data[*index2];
  (*index2)++;
    
  nRows = data[*index2];
  (*index2)++;
  
  // Next zero is ignored
  (*index2)++; 
  
  /* Memory allocation */
  if((lengths=(int *)CALLOC(1,sizeof(int)*(nCols*nRows)))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"CreateInlineTList");
      return 0;
    }
  
  /* Read character strings lengths */
  k = 0;
  for (k=0; k<nCols*nRows; k++)
    {
      lengths[k] =  data[*index2 + 1] - data[*index2];
      if (lengths[k] > maxlength)
        {
          maxlength = lengths[k];
        }
      (*index2)++;
    }

  /* Memory allocation */
  if((str=CALLOC(1,sizeof(char*)*nCols*nRows))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"CreateInlineTList");
      return 0;
    }
  for (k=0; k< nCols*nRows; k++)
    {
      if((str[k]=(char *)CALLOC(1,sizeof(char)*(lengths[k]+1)))==NULL)
        {
          Scierror(999,_("%s: No more memory.\n"),"CreateInlineTList");
          return 0;
        }
    }
  if((int_str=(int *)CALLOC(1,sizeof(int)*(maxlength+1)))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"CreateInlineTList");
      return 0;
    }
  
  /* Read all strings and write to Scilab stack */
  for(k=0;k<nCols*nRows;k++)
    {
      for(i=0;i<lengths[k];i++)
        {
          *index2=*index2 + 1;
          int_str[i]=data[*index2];
        }
      CvStr(&lengths[k],int_str,str[k],&job1,lengths[k]);
      (str[k])[lengths[k]]='\0';
    }
  (*index2)++;
              
  str2sci(str, nCols, nRows);

  (*index2)++; // Unsused code
  (*index2)++; // Go to code 6
  
  /* Ignore next constant creation (not used) */
  if (data[*index2]==6)
    {
      GetInstruction(data,index2,nblines,addinstr);
    }

  /* Ignore next funcall: a call to deff function */
  (*index2)++;
  (*index2)++;
  data[*index2+2] = 2; // Change number of inputs because of constant ignore above */
  if (data[*index2]==3)
    {
      GetInstruction(data,index2,nblines,addinstr);
      Top --;
    }
  *index2=*index2 + 1;

  /* Ignore next affectation: ans used to store output of deff */
  if (data[*index2]==29)
    {
      GetInstruction(data,index2,nblines,addinstr);
      Top --;
    }

  /* Create returned list */
  C2F(mktlist)(&n_inline_tlist);

  // Free memory
  freeArrayOfString(str, nCols*nRows);
  FREE(int_str);
  return 0;
}

/****************************************************************
 Function name: CreateOperationTList
****************************************************************/
static int CreateOperationTList(int *data,int *index2)
{
  char *op_tlist[] = {"operation","operands","operator"};
  int m_op_tlist = 1;
  int n_op_tlist = 3;

  /* Operators table */
  char *operators[]={"+","-","*",".*","*.",".*.","/","./","/.","./.",
	       "\\",".\\","\\.",".\\.","^","==","<",">","<=",">=","~=",
	       ":","rc","ins","ext","'","cc","|","&","~",".^",".'","cceol"};
  /* cceol: special operator for column concatenation followed by EOL (initialisation made on more than one line... */

  char **operator = NULL;
  int max_op_lgth = 5; /* strlen("cceol") */

  int operators_num[32]={45,46,47,98,200,149,48,99,201,150,
			 49,100,202,151,62,50,59,60,109,110,119,
			 44,1,2,3,53,4,57,58,61,113,104};

  int operator_num,operator_index2=-1,nb_operands,nb_lhs;

  int k; /* Loop index2 */

  int orig,dest; /* Used when copy objects */

  int one = 1;

  int stkPos = Top;
  int nbOps = 0;

  /* Memory allocation */
  if((operator=CALLOC(1,sizeof(char*)))==NULL)
    {
	  Scierror(999,_("%s: No more memory.\n"),"CreateOperationTList");
      return 0;
    }
  if((operator[0]=(char *)CALLOC(1,sizeof(char)*max_op_lgth+1))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"CreateOperationTList");
      return 0;
    }
  (operator[0])[max_op_lgth] = '\0';

  /* Read useful data */
  (*index2)++; /* Pass index2 corresponding to 5 */
  operator_num = data[*index2];
  (*index2)++;
  nb_operands = data[*index2]; /* One or two */
  (*index2)++;
  nb_lhs = data[*index2]; /* Always one */

  
  /* Search operator index2 */
  for(k=0;k<32;k++)
    {
    if(operators_num[k]==operator_num)
	{
	  operator_index2=k;
	  break;
	}
    }

  if(operator_index2<0)
    {
      Scierror(999,_("%s: Unknown operator %d.\n"),"CreateOperationTList",operator_num);
      return 0;
    }

  /* In case a EOL is inserted in a row/column catenation */
  if(operator_index2!=24)
    {
      while (nbOps!=nb_operands && stkPos>0)
        {
          if (*istk(iadr(*Lstk(stkPos))) == 16)
            {
              if (!isAComment(stkPos))
                {
                  nbOps++;
                }
            }
          stkPos --;
        }
      if(stkPos!=0)
        {
          nb_operands = Top - stkPos;
        }
    }

  /* Write tlist items names */
  str2sci(op_tlist,m_op_tlist,n_op_tlist);


  /* Move all operands to next place in stack */
  if(operator_index2==24) /* For extraction: variable is moved to be the first operand */
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
  strcpy(operator[0],operators[operator_index2]);
  (operator[0])[strlen(operators[operator_index2])]='\0';
  str2sci(operator,one,one);

  /* Create operation tlist */
  C2F(mktlist)(&n_op_tlist);

  /* Move resulting list to first free place in stack */
  orig = Top;
  dest = Top - nb_operands;
  VCopyObj("CreateOperationTList",&orig,&dest,20L);

  return 0;
}

/****************************************************************
 Function name: CreateFuncallTList
****************************************************************/
static int CreateFuncallTList(char *fromwhat,int *data,int *index2)
{
  char *fun_tlist[] = {"funcall","rhs","name","lhsnb"};
  int m_fun_tlist = 1;
  int n_fun_tlist = 4;

  /* Used when fromwhat=="funptr" */
  int interf_num,interf_index2,funptr;
  int job1 = 1,job2 = (int)SCI_HFUNCTIONS_BACKSEARCH;
  int id[nsiz];

  double nblhs = 0;
  int nbrhs = 0;

  char **funname = NULL;
  int funnamelgth = 0;

  int one = 1;

  int orig,dest; /* Used when copy objects */

  /* Used for empty matrix creation when rhsnb==0 (function called as a command) */
  double l_mat = 0;
  int m_mat = 0;
  int n_mat = 0;

  int stkPos = Top;
  int nbOps = 0;

  /* Memory allocation */
  if((funname=CALLOC(1,sizeof(char*)))==NULL)
    {
	  Scierror(999,_("%s: No more memory.\n"),"CreateFuncallTList");
      return 0;
    }
  if((funname[0]=(char *)CALLOC(1,sizeof(char)*(nlgh+1)))==NULL)
    {
	  Scierror(999,_("%s: No more memory.\n"),"CreateFuncallTList");
      return 0;
    }
  (funname[0])[nlgh]='\0';

  if(!strncmp(fromwhat,"funptr",6))
    {
      interf_num = data[*index2];
      (*index2)++;
      nbrhs = data[*index2];
      (*index2)++;
      nblhs = data[*index2];
      (*index2)++;
      interf_index2 = data[*index2];

      funptr = interf_num * 10 + interf_index2;

      C2F(funtab)(id,&funptr,&job2,"NULL_NAME",0);

      CvNameL(id,funname[0],&job1,&funnamelgth);
      (funname[0])[funnamelgth]='\0';
    }
  else if(!strncmp(fromwhat,"datacode",8))
    {
      if(data[*index2]==12)
	{
	  strncpy(funname[0],"pause",5);
	  funnamelgth = 5;
	}
      else if(data[*index2]==13)
	{
	  strncpy(funname[0],"break",5);
	  funnamelgth = 5;
	}
      else if(data[*index2]==14)
	{
	  strncpy(funname[0],"abort",5);
	  funnamelgth = 5;
	}
      else if(data[*index2]==17)
	{
	  strncpy(funname[0],"quit",4);
	  funnamelgth = 4;
	}
      else if(data[*index2]==20)
	{
	  strncpy(funname[0],"exit",4);
	  funnamelgth = 4;
	}
      else if(data[*index2]==28)
	{
	  strncpy(funname[0],"continue",8);
	  funnamelgth = 8;
	}
      else if(data[*index2]==99)
	{
	  strncpy(funname[0],"return",6);
	  funnamelgth = 6;
	}
    }
  else if(!strncmp(fromwhat,"macro",5))
    {
      CvNameL(&data[*index2-nsiz+1],funname[0],&job1,&funnamelgth);
      (funname[0])[funnamelgth]='\0';

      (*index2)++;
      nbrhs = data[*index2];
      (*index2)++;
      nblhs = data[*index2];
    }
  else /* Should never happen */
    {
      Scierror(999,_("%s: Wrong fromwhat value %s.\n"),"CreateEqualTList",fromwhat);
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

      /* In case a EOL is inserted in a row/column catenation */
      while (nbOps!=nbrhs && stkPos>0)
        {
          if (*istk(iadr(*Lstk(stkPos))) == 16)
            {
              if (!isAComment(stkPos))
                {
                  nbOps++;
                }
            }
          stkPos --;
        }
      if(stkPos!=0)
        {
          nbrhs = Top - stkPos;
        }

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
  freeArrayOfString(funname, 1);

  return 0;
}

/****************************************************************
 Function name: CreateEqualTList
****************************************************************/
static int CreateEqualTList(char *fromwhat,int *data,int *index2)
{
  char *eq_tlist[] = {"equal","expression","lhs","endsymbol"};
  int m_eq_tlist = 1;
  int n_eq_tlist = 4;

  int nblhs = 0,nbrhs = 0;

  int k = 0,l = 0; /* Loop index2es */

  int job1 = 1;

  int orig,dest; /* Used when copy objects */

  char **name = NULL;
  int namelgth = 0;

  /* Used for lhs which are insertion operations */
  int index2es_pos;
  int nb_index2es = 0;
  char *op_tlist[] = {"operation","operands","operator"};
  int m_op_tlist = 1;
  int n_op_tlist = 3;

  char **operator = NULL;

  int one = 1;

  char **endsymbol = NULL;
  int symbol = 0;

  /* Memory allocation */
  if((name=CALLOC(1,sizeof(char*)))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
      return 0;
    }
  if((name[0]=(char *)CALLOC(1,sizeof(char)*(nlgh+1)))==NULL)
    {
   	  Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
      return 0;
    }
  (name[0])[nlgh] = '\0';

  if((operator=CALLOC(1,sizeof(char*)))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
      return 0;
    }
  if((operator[0]=(char *)CALLOC(1,sizeof(char)*4))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
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

      index2es_pos = Top - 3;

      /* Create list of lhs */
      (*index2)++; /* Code 29 is passed */
      nblhs = data[*index2];
      (*index2)++;

      /* Symbol which ends the line: ; , or nothing */
      symbol=data[*index2];
      if(symbol==43) /* ; */
	{
	  if((endsymbol=CALLOC(1,sizeof(char*)))==NULL)
	    {
	      Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
	      return 0;
	    }
	  if((endsymbol[0]=(char *)CALLOC(1,sizeof(char)*2))==NULL)
	    {
   	      Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
	      return 0;
	    }
	  strcpy(endsymbol[0],";");
	  (endsymbol[0])[1] = '\0';
	}
      else if(symbol==52) /* , */
	{
	  if((endsymbol=CALLOC(1,sizeof(char*)))==NULL)
	    {
	      Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
	      return 0;
	    }
	  if((endsymbol[0]=(char *)CALLOC(1,sizeof(char)*2))==NULL)
	    {
   	      Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
	      return 0;
	    }
	  strcpy(endsymbol[0],",");
	  (endsymbol[0])[1] = '\0';
	}
      else /* Nothing */
	{
	  if((endsymbol=CALLOC(1,sizeof(char*)))==NULL)
	    {
   	      Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
	      return 0;
	    }
	  if((endsymbol[0]=(char *)CALLOC(1,sizeof(char)*1))==NULL)
	    {
	      Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
	      return 0;
	    }
	  (endsymbol[0])[0] = '\0';
	}

      for(k=0;k<nblhs;k++)
	{
	  (*index2)++;
	  CvNameL(&data[*index2],name[0],&job1,&namelgth);
	  (name[0])[namelgth] = '\0';
	  *index2 = *index2 + nsiz;
	  nbrhs = data[*index2];
 	  nb_index2es = nbrhs + nb_index2es;

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

	      /* Index2es for insertion */
	      for(l=0;l<nbrhs;l++)
		{
		  orig = index2es_pos - nbrhs + l + 1;
		  dest = Top + 1;
		  VCopyObj("CreateEqualTList",&orig,&dest,16L);
		}
	      index2es_pos = index2es_pos - nbrhs;

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
      dest = Top - nb_index2es - 1;
      VCopyObj("CreateEqualTList",&orig,&dest,16L);
    }
  else if(!strncmp(fromwhat,"code18",6)) /* A code 18 was found in data */
    {
      /* Copy expression */
      orig = Top - 1;
      dest = Top + 1;
      VCopyObj("CreateEqualTList",&orig,&dest,16L);

      (*index2)++;
      nblhs++;
      CvNameL(&data[*index2],name[0],&job1,&namelgth);
      (name[0])[namelgth]='\0';
      CreateVariableTList(name);
      *index2 = *index2 + nsiz;
      *index2 = *index2 - 1;

      /* Create list of lhs */
      C2F(mklist)(&nblhs);

      /* Symbol */
      if((endsymbol=CALLOC(1,sizeof(char*)))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
	  return 0;
	}
      if((endsymbol[0]=(char *)CALLOC(1,sizeof(char)*1))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
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

      while(data[*index2]==1)
	{
	  (*index2)++;
	  nblhs++;
	  CvNameL(&data[*index2],name[0],&job1,&namelgth);
	  (name[0])[namelgth]='\0';
	  CreateVariableTList(name);
	  *index2 = *index2 + nsiz;
	  (*index2)++; /* Code 22 (print mode) is ignored */
	  (*index2)++; /* Code 99 is ignored */
	}
      /* Create list of lhs */
      C2F(mklist)(&nblhs);

      /* Symbol */
      if((endsymbol=CALLOC(1,sizeof(char*)))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
	  return 0;
	}
      if((endsymbol[0]=(char *)CALLOC(1,sizeof(char)*1))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
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
      if((endsymbol=CALLOC(1,sizeof(char*)))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
	  return 0;
	}
      if((endsymbol[0]=(char *)CALLOC(1,sizeof(char)*1))==NULL)
	{
	  Scierror(999,_("%s: No more memory.\n"),"CreateEqualTList");
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
      Scierror(999,_("%s: Wrong fromwhat value %s.\n"),"CreateEqualTList",fromwhat);
      return 0;
    }

  /* Free memory */
  freeArrayOfString(name, 1);
  freeArrayOfString(operator, 1);
  freeArrayOfString(endsymbol, 1);

  return 0;
}
/****************************************************************
 Function name: CreateCommentTList
****************************************************************/
static int CreateCommentTList(int *data,int *index2)
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
  (*index2)++;
  strlgth = data[*index2];
  (*index2)++;
  /* Memory allocation */
  if((text=(char *)CALLOC(1,sizeof(char)*(strlgth+1)))==NULL)
    {
      Scierror(999,_("%s: No more memory.\n"),"CreateCsteTList");
      return 0;
    }
  CvStr(&strlgth,&(data[*index2]),text,&job1,strlgth);
  text[strlgth]='\0';
  str2sci(&text,one,one);
  *index2 = *index2 + strlgth-1;
  /* Free memory */
  FREE(text);
  text=NULL;

  C2F(mktlist)(&n_fun_tlist);
  return 0;
}



/*
 * Function name: CreateRecursiveIndexList
 * Decription:
 *  Create on Scilab stack a list for recursive insertion or extraction
 *  First list item is a matrix which contains number of row/column indexes
 * @param data pointer to compiled macro code
 * @param index index of current int in data
 *
 * @return 0 if execution succeeds
 * @return not null if execution fails
 */
static int CreateRecursiveIndex2List(int *data,int *index2)
{
  int m,n;

  /* Get infos in data */
  (*index2)++;
  n = data[*index2];
  (*index2)++;
  m = data[*index2];

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
int complexity(int *data,int *index2,int *lgth)
{
  int count = 0;
  int countSave = 0;
  int codeSave = 0;
  int begin = -10;

  int cur_ind = *index2+1;

  int last_eol=0;

  int nbop = 0; /* Number of value stored on stack */

  /* nbop equivalent to Top */
  /* count equivalent to nbstat */

  while(cur_ind<=*lgth+*index2)
    {
      countSave = count;
      codeSave = data[cur_ind];
      switch(data[cur_ind])
	{
        case 0: /* Deleted operation */
	  cur_ind = cur_ind + data[cur_ind+1];
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
          /* EOL inserted inside catenation */
          if (begin>=0 && nbop - begin > data[cur_ind+2] && last_eol == nbop-2)
            {
              nbop = begin;
            }
          else
            {
              nbop = nbop - data[cur_ind+2];
            }
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
	case 19: /* Form recursive index2 list */
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
	      sciprint(_("%s: wrong code %d.\n"),"Complexity",data[cur_ind]);
	      return -1;
	    }
	  break;
	}

      if (codeSave==15)
        {
          if ((begin>0) & (data[cur_ind]!=29)) /* Inside a list of inputs or operands */
            {
              count--; /* No new instruction created */
            }
        }
      else if (codeSave==31)
        {
          if (begin>0) /* Inside a list of inputs or operands */
            {
              count--; /* No new instruction created */
            }
        }
      else
        {
          if (countSave!=count)
            {
              /* Reinit */
              begin = -10;
              last_eol = -10;
            }
          else if (begin <= 0 && codeSave!=16 && codeSave!=21 && codeSave!=0)
            {
              /* Store position */
              begin = nbop - 1;
            }
        }
    }
  return count;

}

int isAComment(int stkPos)
{
	int nbElements = 0;
	int firstElementAdr = 0;
	int firstChar = 0;
	int secondChar = 0;
	int thirdChar = 0;
	int fourthChar = 0;
	int il = iadr(*Lstk(stkPos));

	/* If not a tlist then cannot be a comment */
	if (*istk(il) != 16)
	{
		return 0;
	}

	/* If tlist size not equal to two then cannot be a comment */
	if (*istk(il + 1) != 2)
	{
		return 0;
	}

	/* Now the tlist can be a comment or a cste */
	nbElements = *istk(il + 1);
	firstElementAdr = iadr(sadr(il + 3 + nbElements));
	firstChar = *istk(firstElementAdr + 7);
	secondChar = *istk(firstElementAdr + 8);
	thirdChar = *istk(firstElementAdr + 9);
	fourthChar = *istk(firstElementAdr + 10);

	/* 12 = Scilab code for 'c' */
	/* 24 = Scilab code for 'o' */
	/* 22 = Scilab code for 'm' */
	return firstChar==12 && secondChar==24 && thirdChar==22 && fourthChar==22;

}
