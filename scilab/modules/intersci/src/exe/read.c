#include <stdlib.h>

#include "intersci-n.h"

/**********************************************************
 *Reading the intersci description file 
 **********************************************************/
  
int ReadFunction(f)
     FILE *f;
{
  int i, j, l, type, ftype;
  char s[MAXLINE];
  char *words[MAXLINE];
  char *optwords[MAXLINE];
  IVAR ivar;
  int nwords, line1, inbas, fline1, infor, nopt, out1;
  nVariable = 0;
  icre=1;
  basfun->maxOpt = 0;
  basfun->NewMaxOpt = 0;
  line1 = 1;
  inbas = 0;
  fline1 = 0;
  infor = 0;
  out1 = 0;
  while (fgets(s,MAXLINE,f)) 
    {
      /* ignoring comments */
      if (s[0] == '/' && s[1] == '/' ) continue;
      
      /* analysis of one line */
      if (line1 != 1) 
	nwords = ParseLine(s,words);
      else 
	nwords = ParseScilabLine(s,words);
      /* empty definition at end of file */
      if (line1 == 1 && nwords == 0) 
	{
	  return 0;
	}
      /* end of description */
      if (words[0][0] == '*') return(1);
      if (line1 == 1) 
	{
	  /* SCILAB function description */
	  if ((int)strlen(words[0]) > 24) 
	    {
	      printf("SCILAB function name too long: \"%s\"\n",words[0]);
	      exit(1);
	    }
	  basfun->name = (char *)malloc((unsigned)(strlen(words[0])+1));
	  strcpy(basfun->name,words[0]);
	  printf("**************************\n");
	  printf("processing SCILAB function \"%s\"\n",words[0]);
	  funNames[nFun] = basfun->name;
	  i = nwords - 1;
	  if (i > MAXARG) 
	    {
	      printf("too may input arguments for SCILAB function\"%s\"\n",
		     words[0]);
	      printf("  augment constant \"MAXARG\" and recompile intersci\n");
	      exit(1);
	    }
	  basfun->nin = i;
	  for (i = 0; i < basfun->nin ; i++) 
	    {
	      if (words[i+1][0] == '{') 
		{
		  basfun->maxOpt++;
		  nopt = ParseLine(words[i+1]+1,optwords);
		  if (nopt != 2) {
		    printf("Bad syntax for optional argument. Two variables needed\n");
		    exit(1);
		  }
		  ivar = GetVar(optwords[0],1);
		  basfun->in[i] = ivar;
		  variables[ivar-1]->opt_type = NAME;
		  variables[ivar-1]->opt_name =
		    (char *)malloc((unsigned)(strlen(optwords[1])+1));
		  variables[ivar-1]->stack_position = icre++;
		  strcpy(variables[ivar-1]->opt_name,optwords[1]);
		  variables[ivar-1]->is_sciarg = 1;
		}
	      else if (words[i+1][0] == '[') 
		{
		  basfun->maxOpt++;
		  nopt = ParseLine(words[i+1]+1,optwords);
		  if (nopt != 2) 
		    {
		      printf("Bad syntax for optional argument. Two variables needed\n");
		      exit(1);
		    }
		  ivar = GetVar(optwords[0],1);
		  basfun->in[i] = ivar;
		  variables[ivar-1]->opt_type = VALUE;
		  variables[ivar-1]->opt_name =
		    (char *)malloc((unsigned)(strlen(optwords[1])+1));
		  strcpy(variables[ivar-1]->opt_name,optwords[1]);
		  variables[ivar-1]->stack_position = icre++;
		  variables[ivar-1]->is_sciarg = 1;
		}
	      else 
		{
		  basfun->in[i] = GetVar(words[i+1],1);
		  variables[basfun->in[i]-1]->stack_position = icre++;
		  variables[basfun->in[i]-1]->is_sciarg = 1;
		}
	    }
	  line1 = 0;
	  inbas = 1;
	} 
      else if (inbas == 1) 
	{
	  if (nwords == 0) 
	    {
	      /* end of SCILAB variable description */
	      inbas = 0;
	      fline1 = 1;
	    } 
	  else 
	    {
	      /* SCILAB variable description */
	      ivar = GetVar(words[0],1);
	      i = ivar - 1;
	      if ( variables[i]->is_sciarg == 0) 
		{
		  /** we only fix stack_position for remaining arguments**/
		  variables[i]->stack_position = icre++;
		}
	      if (nwords == 1) 
		{
		  printf("type missing for variable \"%s\"\n",words[0]);
		  exit(1);
		}
	      type = GetBasType(words[1]);
	      variables[i]->type = type;
	      switch (type) 
		{
		case SCALAR:
		case ANY:
		case SCIMPOINTER:
		case SCISMPOINTER:
		case SCILPOINTER:
		case SCIBPOINTER:
		case SCIOPOINTER:
		  break;
		case COLUMN:
		case ROW:
		case STRING:
		case WORK:
		case VECTOR:
		  if (nwords != 3) 
		    {
		      printf("bad type specification for variable \"%s\" \n", words[0]);
		      printf("only %d argument given and %d are expected\n", nwords,3);
		      exit(1);
		    }
		  variables[i]->el[0] = GetVar(words[2],1);
		  variables[i]->length++;
		  break;
		case LIST:
		case TLIST:
		  if (nwords != 3) 
		    {
		      printf("bad type specification for variable \"%s\"\n", words[0]);
		      printf("only %d argument given and %d are expected\n", nwords,3);
		      exit(1);
		    }
		  ReadListFile(words[2],words[0],i,
			       variables[i]->stack_position);
		  break;
		case POLYNOM:
		case MATRIX:
		case BMATRIX:
		case STRINGMAT:
		  if (nwords != 4) 
		    {
		      printf("bad type specification for variable \"%s\"\n",words[0]);
		      printf("%d argument given and %d are expected\n", nwords,4);
		      exit(1);
		    }	  
		  variables[i]->el[0] = GetVar(words[2],1);
		  variables[i]->el[1] = GetVar(words[3],1);
		  variables[i]->length = 2;
		  break;
		case IMATRIX:
		  if (nwords != 5) 
		    {
		      printf("bad type specification for variable \"%s\"\n",words[0]);
		      printf("%d argument given and %d are expected\n", nwords,4);
		      exit(1);
		    }	  
		  variables[i]->el[0] = GetVar(words[2],1);
		  variables[i]->el[1] = GetVar(words[3],1);
		  variables[i]->el[2] = GetVar(words[4],1);
		  variables[i]->length = 3;
		  break;
		case SPARSE:
		  if (nwords != 6)
		    {
		      printf("bad type specification for variable \"%s\"\n",words[0]);
		      printf("%d argument given and %d are expected\n", nwords,6);
		      printf("name sparse m n nel it\n");
		      exit(1);
		    }	  
		  variables[i]->el[0] = GetVar(words[2],1);
		  variables[i]->el[1] = GetVar(words[3],1);
		  variables[i]->el[2] = GetVar(words[4],1);
		  variables[i]->el[3] = GetVar(words[5],1);
		  variables[i]->length = 4;
		  break;
		case SEQUENCE:
		  printf("variable \"%s\" cannot have type \"SEQUENCE\"\n",
			 words[0]);
		  exit(1);
		  break;
		case EMPTY:
		  printf("variable \"%s\" cannot have type \"EMPTY\"\n",
			 words[0]);
		  exit(1);
		  break;
		}
	    }
	} 
      else if (fline1 == 1) 
	{
	  /* FORTRAN subroutine description */
	  forsub->name = (char *)malloc((unsigned)(strlen(words[0])+1));
	  strcpy(forsub->name,words[0]);
	  i = nwords - 1;
	  if (i > MAXARG) 
	    {
	      printf("too many argument for FORTRAN subroutine \"%s\"\n",
		     words[0]);
	      printf("  augment constant \"MAXARG\" and recompile intersci\n");
	      exit(1);
	    }
	  forsub->narg = i;
	  for (i = 0; i < nwords - 1; i++) 
	    {
	      forsub->arg[i] = GetExistVar(words[i+1]);
	    }
	  fline1 = 0;
	  infor = 1;
	} 
      else if (infor == 1) 
	{
	  if (nwords == 0) 
	    {
	      /* end of FORTRAN subroutine description */
	      infor = 0;
	      out1 = 1;
	    }
	  else 
	    {
	      /* FORTRAN variable description */
	      if (nwords == 1) 
		{
		  printf("type missing for FORTRAN argument \"%s\"\n",
			 words[0]);
		  exit(1);
		}
	      ivar = GetExistVar(words[0]);
	      ftype = GetForType(words[1]);
	      variables[ivar-1]->for_type = ftype;
	      if (ftype == EXTERNAL) 
		{
		  strcpy((char *)(variables[ivar-1]->fexternal),words[1]);
		  switch (variables[ivar-1]->type) 
		    {
		    case LIST :
		    case TLIST :
		    case SCALAR :
		    case SEQUENCE :
		    case WORK:
		    case EMPTY :
		    case ANY:
		    case SCIMPOINTER :
		    case SCISMPOINTER :
		    case SCILPOINTER :
		    case SCIBPOINTER :
		    case SCIOPOINTER :
		      printf("FORTRAN argument \"%s\" with external type \"%s\"\n",
			     variables[ivar-1]->name,words[1]);
		      printf("  cannot have a variable type of \"%s\" \n",SGetSciType(variables[ivar-1]->type));
		      exit(1);
		      break;
		    }
		}
	    }
	} 
      else if (out1 == 1) 
	{
	  /* output variable description */
	  i = ivar - 1;
	  if (nwords == 1) 
	    {
	      printf("type missing for output variable \"out\"\n");
	      exit(1);
	    }
	  ivar = GetOutVar(words[0]);
	  basfun->out = ivar;
	  i = ivar - 1;
	  type = GetBasType(words[1]);
	  variables[i]->type = type;
	  switch (type) 
	    {
	    case LIST:
	    case TLIST:
	    case SEQUENCE:
	      l = nwords - 2;
	      if (l > MAXEL) 
		{
		  printf("list or sequence too long for output variable \"out\"\n");
		  printf("  augment constant \"MAXEL\" and recompile intersci\n");
		  exit(1);
		}
	      for (j = 0; j < l; j++) 
		{
		  int k = GetExistVar(words[j+2]);
		  variables[i]->el[j] = k;
		  variables[k-1]->out_position = j+1;
		}
	      variables[i]->length = l;
	      break;
	    case EMPTY:
	      break;
	    default:
	      printf("output variable \"out\" of SCILAB function\n");
	      printf("  must have type \"LIST\", \"TLIST\", \"SEQUENCE\" or\n");
	      printf("  \"EMPTY\"\n");
	      exit(1);
	      break;
	    }
	  out1 = 0;
	}
      else 
	{
	  /* possibly equal variables */
	  ivar = GetExistVar(words[0]);
	  i = ivar -1 ;
	  variables[i]->equal = GetExistVar(words[1]);
	}
    }
  /* end of description file */
  return(0);
}

/***********************************************************************
 *  put the words of SCILAB function description line "s" in "words" and 
 * return the number of words with checking syntax of optional variables:
 * "{g  the_g }" => 1 word "{g  the_g \n"
 * "[f v]" => 1 word "[f v\n" 
 **************************************************************************/

int ParseScilabLine(s,words)
     char *s, *words[];
{
  char w[MAXNAM];
  int nwords = 0;
  int inword = 1;
  int inopt1 = 0; /* {  } */
  int inopt2 = 0; /* [  ] */
  int i = 0;
  if (*s == ' ' || *s == '\t') inword = 0;
  if (*s == '{') inopt1 = 1;
  if (*s == '[') inopt2 = 1;
  while (*s) {
    if (inopt1) {
      w[i++] = *s++;
      if (*s == '{' || *s == '[' || *s == ']' || *s == '\n') {
	printf("Bad syntax for optional argument. No matching \"}\"\n");
	exit(1);
      }
      else if (*s == '}') {
	w[i++] = '\n';
	w[i] = '\0';
	words[nwords] = (char *)malloc((unsigned)(i+1));
	strcpy(words[nwords],w);
	nwords++;
	inopt1 = 0;
	inword = 0;
      }
    }
    else if (inopt2) {
      w[i++] = *s++;
      if (*s == '[' || *s == '{' || *s == '}' || *s == '\n') {
	printf("Bad syntax for optional argument. No matching \"]\"\n");
	exit(1);
      }
      else if (*s == ']') {
	w[i++] = '\n';
	w[i] = '\0';
	words[nwords] = (char *)malloc((unsigned)(i+1));
	strcpy(words[nwords],w);
	nwords++;
	inopt2 = 0;
	inword = 0;
      }
    }
    else if (inword) {
      w[i++] = *s++;
      if (*s == ' ' || *s == '\t' || *s == '\n') {
	w[i] = '\0';
	words[nwords] = (char *)malloc((unsigned)(i+1));
	strcpy(words[nwords],w);
	nwords++;
	inword = 0;
      }
    }
    else {
      s++; /* *s++; */
      if (*s != ' ' && *s != '\t') {
	/* beginning of a word */
	i = 0;
	inword = 1;
	if (*s == '{') inopt1 = 1;
	if (*s == '[') inopt2 = 1;
      }
    }
  }
  return(nwords);
}

/* put the words of line "s" in "words" and return the number of words */

int ParseLine(s,words)
     char *s, *words[];
{
  char w[MAXNAM];
  int nwords = 0;
  int inword = 1;
  int i = 0;
  if(*s == ' ' || *s == '\t') inword = 0;
  while (*s) {
    if (inword) {
      w[i++] = *s++;
      if (*s == ' ' || *s == '\t' || *s == '\n') {
	w[i] = '\0';
	words[nwords] = (char *)malloc((unsigned)(i+1));
	strcpy(words[nwords],w);
	nwords++;
	inword = 0;
      }
    }
    else {
      s++; /* *s++; */
      if (*s != ' ' && *s != '\t') {
	i = 0;
	inword = 1;
      }
    }
  }
  return(nwords);
}

/***********************************************************************
 * Read a List description file 
 **************************************************************************/


void ReadListFile(listname,varlistname,ivar,stack_position)
     char *listname;
     char *varlistname;
     IVAR ivar;
     int stack_position ;
{
  FILE *fin;
  char filin[MAXNAM];
  int nel;
  
  sprintf(filin,"%s.list",listname);
  fin = fopen(filin,"r");
  if (fin == 0) 
    {
      printf("description file for list or tlist \"%s\" does not exist\n",
	     filin);
      exit(1);
    }
  printf("reading description file for list or tlist \"%s\"\n", listname);
  
  nel = 0;
  while(ReadListElement(fin,varlistname,ivar,nel,stack_position)) 
    {
      nel++;
    }
  
  fclose(fin);
}

int ReadListElement(f,varlistname,iivar,nel,stack_position)
     FILE *f;
     char *varlistname;
     int nel;
     IVAR iivar;
     int stack_position;
{
  char s[MAXLINE];
  char *words[MAXLINE];
  int i, nline, nwords, type;
  IVAR ivar;
  char str[MAXNAM];
  nline = 0;
  while (fgets(s,MAXLINE,f) != NULL) 
    {
      /* analyse of one line */
      nline++;
      switch (nline) 
	{
	case 1:
	  break;
	case 2:
	  /* SCILAB variable description */
	  nwords = ParseLine(s,words);
	  sprintf(str,"%s(%s)",words[0],varlistname);
	  ivar = GetVar(str,0);
	  i = ivar - 1;
	  variables[ivar-1]->stack_position =stack_position;
	  if (nwords == 1) 
	    {
	      printf("type missing for variable \"%s\"\n",words[0]);
	      exit(1);
	    }
	  type = GetBasType(words[1]);
	  variables[i]->type = type;
	  variables[i]->list_name = (char *)malloc((unsigned)(strlen(varlistname)+1));
	  strcpy(variables[i]->list_name,varlistname);
	  variables[i]->list_el = nel+1;
	  switch (type) 
	    {
	    case SCALAR:
	    case ANY:
	      break;
	    case COLUMN:
	    case ROW:
	    case STRING:
	    case VECTOR:
	      if (nwords != 3) 
		{
		  printf("bad type for variable \"%s\"\n",
			 words[0]);
		  exit(1);
		}
	      if (isdigit(words[2][0]))
		{
		  variables[i]->el[0] = GetVar(words[2],0);
		  variables[i]->length = 1;
		}
	      else 
		{
		  sprintf(str,"%s(%s)",words[2],varlistname);
		  variables[i]->el[0] = GetVar(str,0);
		  variables[i]->length = 1;
		}
	      break;
	    case POLYNOM:
	    case MATRIX:
	    case BMATRIX:
	    case STRINGMAT:
	      if (nwords != 4) 
		{
		  printf("bad type for variable \"%s\"\n",
			 words[0]);
		  exit(1);
		}	  
	      if (isdigit(words[2][0]))
		{
		  variables[i]->el[0] = GetVar(words[2],0);
		  variables[i]->length = 1;
		} 
	      else
		{
		  sprintf(str,"%s(%s)",words[2],varlistname);	
		  variables[i]->el[0] = GetVar(str,0);
		  variables[i]->length = 1;
		}
	      if (isdigit(words[3][0]))
		{
		  variables[i]->el[1] = GetVar(words[3],0); 
		  variables[i]->length = 2;
		}
	      else 
		{
		  sprintf(str,"%s(%s)",words[3],varlistname);	
		  variables[i]->el[1] = GetVar(str,0);
		  variables[i]->length = 2;
		}
	      break;
	    case IMATRIX:
	      if (nwords != 5) 
		{
		  printf("bad type for variable \"%s\"\n",
			 words[0]);
		  exit(1);
		}	  
	      if (isdigit(words[2][0]))
		{
		  variables[i]->el[0] = GetVar(words[2],0);
		  variables[i]->length = 1;
		} 
	      else
		{
		  sprintf(str,"%s(%s)",words[2],varlistname);	
		  variables[i]->el[0] = GetVar(str,0);
		  variables[i]->length = 1;
		}
	      if (isdigit(words[3][0]))
		{
		  variables[i]->el[1] = GetVar(words[3],0); 
		  variables[i]->length = 2;
		}
	      else 
		{
		  sprintf(str,"%s(%s)",words[3],varlistname);	
		  variables[i]->el[1] = GetVar(str,0);
		  variables[i]->length = 2;
		}
	      sprintf(str,"%s(%s)",words[4],varlistname);	
	      variables[i]->el[2] = GetVar(str,0);
	      variables[i]->length = 3;
	      break;
	    case SPARSE:
	      if (nwords != 6) 
		{
		  printf("bad type for variable \"%s\"\n",
			 words[0]);
		  exit(1);
		}	  
	      if (isdigit(words[2][0]))
		{
		  variables[i]->el[0] = GetVar(words[2],0);
		  variables[i]->length = 1;
		} 
	      else
		{
		  sprintf(str,"%s(%s)",words[2],varlistname);	
		  variables[i]->el[0] = GetVar(str,0);
		  variables[i]->length = 1;
		}
	      if (isdigit(words[3][0]))
		{
		  variables[i]->el[1] = GetVar(words[3],0); 
		  variables[i]->length = 2;
		}
	      else 
		{
		  sprintf(str,"%s(%s)",words[3],varlistname);	
		  variables[i]->el[1] = GetVar(str,0);
		  variables[i]->length = 2;
		}
	      if (isdigit(words[4][0]))
		{
		  variables[i]->el[2] = GetVar(words[4],0); 
		  variables[i]->length = 3;
		}
	      else 
		{
		  sprintf(str,"%s(%s)",words[4],varlistname);	
		  variables[i]->el[2] = GetVar(str,0);
		  variables[i]->length = 3;
		}
	      sprintf(str,"%s(%s)",words[5],varlistname);	
	      variables[i]->el[3] = GetVar(str,0);
	      variables[i]->length = 4;
	      break;
	    case WORK:
	    case SEQUENCE:
	    case EMPTY:
	    case LIST:
	    case TLIST:
	      printf("variable \"%s\" cannot have type \"%s\"\n",
		     words[0],SGetSciType(type));
	      exit(1);
	    default:
	      printf("variable \"%s\" has unknown type \"%s\"\n",
		     words[0],SGetSciType(type));
	    }
	  break;
	default:
	  /* end of description */
	  if (s[0] == '*') 
	    {
	      return(1);
	    }
	  else
	    {
	      printf("bad description file for list or tlist \"%s\"\n",
		     varlistname);
	      exit(1);
	    }
	  break;
	}
    }
  return(0);
}

