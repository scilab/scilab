
#ifdef WIN32 
#include <windows.h>
#include <stdio.h>
#endif 

#ifdef __STDC__
#include <stdlib.h>
#else 
extern  char  *getenv();
#endif


#include "intersci.h"

static char buf[1024];

static int icre=1;     /* incremental counter for variable creation */
static int indent = 0; /* incremental counter for code indentation */
static int pass = 0 ;  /* flag for couting pass on code generation */

#ifdef WIN32 
static void SciEnv ();
#endif 

int main(argc,argv)
     unsigned int argc;
     char **argv;
{ 
  int InterFace = 0 ;
#ifdef WIN32 
  SciEnv();
#endif 
  switch (argc) {
  case 2:
    InterFace = 0; break;
  case 3:
    InterFace = atoi(argv[2]);break;
  default:
    printf("usage:  intersci <interface file> <interface number>\n");
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
  ISCIReadFile(argv[1]);
  GenFundef(argv[1],InterFace);
  return 0;
}

void ISCIReadFile(file)
     char *file;
{
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
  strcat(filout,".f");
  fout = fopen(filout,"w");
  strcpy(filout,file);
  strcat(filout,".tmp");
  foutv = fopen(filout,"w");
  InitDeclare();
  nFun = 0;
  while(ReadFunction(fin)) {
    nFun++;
    if (nFun > MAXFUN) {
      printf("Too many SCILAB functions. The maximum is %d\n",MAXFUN);
      exit(1);
    }
    ResetDeclare();
    /* first pass to collect declarations */
    pass=0;
    WriteFunctionCode(foutv);
    /* cleaning added Fornames before pass 2 */
    ForNameClean();
    /* scond pass to produce code */
    pass=1;
    WriteFunctionCode(fout);
    /** WriteInfoCode(fout); **/
  }
  WriteMain(fout,file);
  printf("FORTRAN file \"%s.f\" has been created\n",file);
  WriteAddInter(file) ;
  printf("Scilab file \"%s.sce\" has been created\n",file);
  fclose(fout);
  fclose(fin);
}

void WriteMain(f,file)
     FILE *f;
     char* file;
{
  int i;
  FCprintf(f,"\nc  interface function \n");
  FCprintf(f,"c   ********************\n");
  WriteMainHeader(f,file);
  Fprintf(f,indent,"goto (");
  for (i = 1; i < nFun ; i++) {
    Fprintf(f,indent,"%d,",i);
  }
  Fprintf(f,indent,"%d) fin\nreturn\n",nFun);
  for (i = 0; i < nFun; i++) {
    FCprintf(f,"%d      call ints%s('%s')\n",i+1,funNames[i],funNames[i]);
    Fprintf(f,indent,"return\n");
  }
  Fprintf(f,indent,"end\n");
}

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
 fprintf(fout,"// files = [ifile,ufiles] \n");
 fprintf(fout,"addinter(files,'%s',%s_funs);\n",file,file);
 fclose(fout);
    }
  else
    fprintf(stderr,"Can't open file %s\n",file);
}
 
  


void Copyright()
{
  printf("\nINTERSCI Version %s (%s)\n",VERSION,DATE);
  printf("    Copyright (C) INRIA All rights reserved\n\n");
}

/**********************************************************
 *Reading the intersci description file 
 **********************************************************/
  
int ReadFunction(f)
     FILE *f;
{
  int i, j, l, type, ftype;
  char s[MAXLINE];
  char str[MAXNAM];
  char *words[MAXLINE];
  char *optwords[MAXLINE];
  IVAR ivar;
  int nwords, line1, inbas, fline1, infor, nopt, out1;
  
  nVariable = 0;
  maxOpt = 0;
  line1 = 1;
  inbas = 0;
  fline1 = 0;
  infor = 0;
  out1 = 0;
  while (fgets(s,MAXLINE,f)) 
    {
      /* analysis of one line */
      if (line1 != 1) nwords = ParseLine(s,words);
      else nwords = ParseScilabLine(s,words);
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
	  for (i = 0; i < nwords - 1; i++) 
	    {
	      if (words[i+1][0] == '{') 
		{
		  maxOpt++;
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
		  strcpy(variables[ivar-1]->opt_name,optwords[1]);
		}
	      else if (words[i+1][0] == '[') 
		{
		  maxOpt++;
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
		}
	      else basfun->in[i] = GetVar(words[i+1],1);
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
		  break;
		case LIST:
		case TLIST:
		  if (nwords != 3) 
		    {
		      printf("bad type specification for variable \"%s\"\n", words[0]);
		      printf("only %d argument given and %d are expected\n", nwords,3);
		      exit(1);
		    }
		  ReadListFile(words[2],words[0],i);
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
		  break;
		case IMATRIX:
		  if (nwords != 5) 
		    {
		      printf("bad type specification for variable \"%s\"\n",words[0]);
		      printf("%d argument given and %d are expected\n", nwords,5);
		      exit(1);
		    }	  
		  variables[i]->el[0] = GetVar(words[2],1);
		  variables[i]->el[1] = GetVar(words[3],1);
		  variables[i]->el[2] = GetVar(words[4],1);
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
		    case COLUMN:
		    case POLYNOM:
		    case ROW:
		    case STRING:
		    case VECTOR:
		      sprintf(str,"ne%d",ivar);
		      AddForName(variables[ivar-1]->el[0],str);
		      break;
		    case SPARSE:
		      sprintf(str,"me%d",ivar);
		      AddForName(variables[ivar-1]->el[0],str);
		      sprintf(str,"ne%d",ivar);
		      AddForName(variables[ivar-1]->el[1],str);
		      sprintf(str,"nel%d",ivar);
		      AddForName(variables[ivar-1]->el[2],str);
		      sprintf(str,"it%d",ivar);
		      AddForName(variables[ivar-1]->el[3],str);
		      break;
		    case IMATRIX:
		      sprintf(str,"me%d",ivar);
		      AddForName(variables[ivar-1]->el[0],str);
		      sprintf(str,"ne%d",ivar);
		      AddForName(variables[ivar-1]->el[1],str);
		      sprintf(str,"it%d",ivar);
		      AddForName(variables[ivar-1]->el[2],str);
		      break;
		    case MATRIX:
		    case BMATRIX:
		    case STRINGMAT:
		      sprintf(str,"me%d",ivar);
		      AddForName(variables[ivar-1]->el[0],str);
		      sprintf(str,"ne%d",ivar);
		      AddForName(variables[ivar-1]->el[1],str);
		      break;
		    default:
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
		variables[i]->el[j] = GetExistVar(words[j+2]);
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

/* 
  put the words of SCILAB function description line "s" in "words" and 
  return the number of words with checking syntax of optional variables:
  "{g  the_g }" => 1 word "{g  the_g \n"
  "[f v]" => 1 word "[f v\n" 
  */

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



void ReadListFile(listname,varlistname,ivar)
     char *listname;
     char *varlistname;
     IVAR ivar;
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
  while(ReadListElement(fin,varlistname,ivar,nel)) 
    {
      nel++;
    }
  
  fclose(fin);
}

int ReadListElement(f,varlistname,iivar,nel)
     FILE *f;
     char *varlistname;
     int nel;
     IVAR iivar;
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
	  sprintf(str,"stk(l%de%d)",iivar+1,nel+1);
	  AddForName(ivar,str);     
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
		}
	      else 
		{
		  sprintf(str,"%s(%s)",words[2],varlistname);
		  variables[i]->el[0] = GetVar(str,0);
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
		} 
	      else
		{
		  sprintf(str,"%s(%s)",words[2],varlistname);	
		  variables[i]->el[0] = GetVar(str,0);
		}
	      if (isdigit(words[3][0]))
		{
		  variables[i]->el[1] = GetVar(words[3],0); 
		}
	      else 
		{
		  sprintf(str,"%s(%s)",words[3],varlistname);	
		  variables[i]->el[1] = GetVar(str,0);
		}
	      break;
	    case IMATRIX:
	      if (nwords != 6) 
		{
		  printf("bad type for variable \"%s\"\n",
			 words[0]);
		  exit(1);
		}	  
	      if (isdigit(words[2][0]))
		{
		  variables[i]->el[0] = GetVar(words[2],0);
		} 
	      else
		{
		  sprintf(str,"%s(%s)",words[2],varlistname);	
		  variables[i]->el[0] = GetVar(str,0);
		}
	      if (isdigit(words[3][0]))
		{
		  variables[i]->el[1] = GetVar(words[3],0); 
		}
	      else 
		{
		  sprintf(str,"%s(%s)",words[3],varlistname);	
		  variables[i]->el[1] = GetVar(str,0);
		}
	      sprintf(str,"%s(%s)",words[4],varlistname);	
	      variables[i]->el[2] = GetVar(str,0);
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
		} 
	      else
		{
		  sprintf(str,"%s(%s)",words[2],varlistname);	
		  variables[i]->el[0] = GetVar(str,0);
		}
	      if (isdigit(words[3][0]))
		{
		  variables[i]->el[1] = GetVar(words[3],0); 
		}
	      else 
		{
		  sprintf(str,"%s(%s)",words[3],varlistname);	
		  variables[i]->el[1] = GetVar(str,0);
		}
	      if (isdigit(words[4][0]))
		{
		  variables[i]->el[2] = GetVar(words[4],0); 
		}
	      else 
		{
		  sprintf(str,"%s(%s)",words[4],varlistname);	
		  variables[i]->el[2] = GetVar(str,0);
		}
	      sprintf(str,"%s(%s)",words[5],varlistname);	
	      variables[i]->el[3] = GetVar(str,0);
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

/*********************************************************************
  Dealing with the set of variables 
*********************************************************************/


/* return the variable number of variable name. if it does not already exist,
   it is created and "nVariable" is incremented
   p corresponds to the present slot of var structure:
   - if the variable does not exist it is created with p value
   - if the variable exists it is created with (p or 0) value 
   */

IVAR GetVar(name,p)
     char *name;
     int p;
{
  int i;
  VARPTR var;
  if (strcmp(name,"out") == 0) {
    printf("the name of a variable which is not the output variable\n");
    printf("  of SCILAB function cannot be \"out\"\n");
    exit(1);
  }
  for (i = 0; i < nVariable; i++) {
    var = variables[i];
    if (strcmp(var->name,name) == 0) {
      var->present = var->present || p;
      return(i+1);
    }
  }
  if (nVariable == MAXVAR) {
    printf("too many variables\n");
    printf("  augment constant \"MAXVAR\" and recompile intersci\n");
    exit(1);
  }
  var = VarAlloc();
  if (var == 0) {
    printf("Running out of memory\n");
    exit(1);
  }
  var->name = (char *)malloc((unsigned)(strlen(name) + 1));
  strcpy(var->name,name);
  var->type = 0;
  var->length = 0;
  var->for_type = 0;
  var->equal = 0;
  var->nfor_name = 0;
  var->kp_state = -1;
  var->list_el = 0;
  var->opt_type = 0;
  var->present = p;
  variables[nVariable++] = var;
  return(nVariable);
}

/* return the variable number of variable name which must already  exist */

IVAR GetExistVar(name)
     char *name;
{
  int i;
  VARPTR var;
  if (strcmp(name,"out") == 0) {
    printf("the name of a variable which is not the output variable\n");
    printf("  of SCILAB function cannot be \"out\"\n");
    exit(1);
  }
  for (i = 0; i < nVariable; i++) {
    var = variables[i];
    if (strcmp(var->name,name) == 0) {
      /* always present */
      var->present = 1;
      return(i+1);
    }
  }
  i=CreatePredefVar(name);
  if ( i != -1) return(i);
  printf("variable \"%s\" must exist\n",name);
  exit(1);
}

/* fname,rhs,lhs,err are predefined variables 
   if someone want to add them in the Fortran or C Calling sequence 
   it's done without aby checks 
*/

int CreatePredefVar(name)
     char *name;
{
  VARPTR var;
  if (strcmp(name,"err")  == 0 
      || strcmp(name,"rhs") == 0 
      || strcmp(name,"lhs") == 0 
      || strcmp(name,"fname") == 0)
    {
      int num ;
      num=GetVar(name,1);
      var = variables[num-1];
      var->for_type = PREDEF;
      return(num);
    }
  return(-1);
}

/* return the variable number of variable "out"
   which is created and "nVariable" is incremented */

IVAR GetOutVar(name)
     char *name;
{
  VARPTR var;
  if (strcmp(name,"out") != 0) {
    printf("the name of output variable of SCILAB function\n");
    printf("  must be \"out\"\n");
    exit(1);
  }
  if (nVariable == MAXVAR) {
    printf("too many variables\n");
    printf("  augmente constant \"MAXVAR\" and recompile intersci\n");
    exit(1);
  }
  var = VarAlloc();
  if (var == 0) {
    printf("Running out of memory\n");
    exit(1);
  }
  var->name = (char *)malloc((unsigned)(strlen(name) + 1));
  strcpy(var->name,name);
  var->type = 0;
  var->length = 0;
  var->for_type = 0;
  var->equal = 0;
  var->nfor_name = 0;
  var->kp_state = -1;
  var->list_el = 0;
  var->opt_type = 0;
  var->present = 0;
  variables[nVariable++] = var;
  return(nVariable);
}

/* return the variable number of variable "out"
   which must exist */

IVAR GetExistOutVar()
{
  int i;
  char str[4];
  strcpy(str,"out");
  for (i = 0; i < nVariable; i++) {
    if (strcmp(variables[i]->name,str) == 0)
      return(i+1);
  }
  printf("variable \"out\" must exist\n");
  exit(1);
}

/***************************
 * add name in the for_name array 
 * field of variable ivar 
 ***************************/

void AddForName(ivar,name)
     IVAR ivar;
     char* name;
{
  VARPTR var;
  int l;
  var = variables[ivar-1];
  l = var->nfor_name;
  if (l == MAXARG) {
    printf("too many \"for_name\" for variable \"%s\"\n",var->name);
    printf("  augment constant \"MAXARG\" and recompile intersci\n");
    exit(1);
  }
  var->for_name[l] = (char *)malloc((unsigned)(strlen(name) + 1));
  strcpy(var->for_name[l],name);
  var->nfor_name = l + 1;
}

void AddForName1(ivar,name)
     IVAR ivar;
     char* name;
{
  VARPTR var;
  int l;
  var = variables[ivar-1];
  l = var->nfor_name;
  if ( pass == 0 && var->kp_state == -1 ) 
    {
      var->kp_state = var->nfor_name ;
    }
  if (l == MAXARG) {
    printf("too many \"for_name\" for variable \"%s\"\n",var->name);
    printf("  augment constant \"MAXARG\" and recompile intersci\n");
    exit(1);
  }
  var->for_name[l] = (char *)malloc((unsigned)(strlen(name) + 1));
  strcpy(var->for_name[l],name);
  var->nfor_name = l + 1;
}

void ForNameClean() 
{
  VARPTR var;
  int i;
  for (i = 0; i < nVariable; i++) {
    var = variables[i];
    if ( var->kp_state != -1 ) 
      {
	var->nfor_name = var->kp_state ;
      }
  }
}

void ChangeForName(ivar,name)
     IVAR ivar;
     char* name;
{
  VARPTR var;
  int l;
  var = variables[ivar-1];
  l = var->nfor_name;
  var->for_name[0] = (char *)malloc((unsigned)(strlen(name) + 1));
  strcpy(var->for_name[0],name);
  /* useful ??? */
  if (l == 0) var->nfor_name = 1;
}

/***********************************************************
  Convertions de type entre codage entier 
  et description par chaine de caracteres
  pour les types Scilab et les types Fortran 
************************************************************/

/* Attention tableau en ordre alphabetique */

static struct btype { char *sname ;
		      int  code ;}
SType[] = {
  {"any",	ANY},
  {"bmatrix",    BMATRIX},
  {"bpointer",   SCIBPOINTER},
  {"column",	COLUMN},
  {"empty",	EMPTY},
  {"imatrix",    IMATRIX},
  {"list", 	LIST},
  {"lpointer",	SCILPOINTER},
  {"matrix",	MATRIX},
  {"mpointer",	SCIMPOINTER},
  {"opointer",	SCIOPOINTER},
  {"polynom",	POLYNOM},
  {"row",	ROW},
  {"scalar",	SCALAR},
  {"sequence",	SEQUENCE},
  {"smpointer",  SCISMPOINTER},
  {"sparse",    SPARSE},
  {"string",	STRING},
  {"stringmat",	STRINGMAT},
  {"tlist", 	TLIST},
  {"vector",	VECTOR},
  {"work",	WORK},
  {(char *) 0 ,  -1}
  };

/* Type Scilab:  renvoit un codage du type en nombre entier etant donne une chaine */

int GetBasType(sname)
     char *sname;
{ 
  int i=0;
  while ( SType[i].sname != (char *) NULL)
     {
       int j ;
       j = strcmp(sname,SType[i].sname);
       if ( j == 0 ) 
	 { 
	   return(SType[i].code);
	 }
       else 
	 { 
	   if ( j <= 0) 
	     break;
	   else i++;
	 }
     }
  printf("the type of variable \"%s\" is unknown\n",sname);
  exit(1);
}

/* Type Scilab :  Renvoit la description (string) d'un type a partir de son code */

char *SGetSciType(type)
     int type;
{
  int i=0;
  while ( SType[i].code  != -1 ) 
     {
       if ( SType[i].code == type ) 
	 return(SType[i].sname);
       else 
	 i++;
     }
  return("unknown");
}

/* Attention tableau en ordre alphabetique */

static struct ftype { char *fname ;
		      int  code ;}
FType[] = {
  {"Cstringv",CSTRINGV},
  {"bpointer",BPOINTER},
  {"char",CHAR},
  {"double", DOUBLE},
  {"int",INT},
  {"integer",INT},
  {"lpointer",LPOINTER},
  {"mpointer",MPOINTER},
  {"opointer",OPOINTER},
  {"predef",PREDEF},
  {"real",REAL},
  {"smpointer",SMPOINTER},
  {(char *) 0 ,  -1}
  };

/* Type Fortran:  renvoit un codage du type en nombre entier etant donne une chaine */
/* convert string to integer FORTRAN type */

int GetForType(type)
     char *type;
{
  int i=0;
  while ( FType[i].fname != (char *) NULL)
     {
       int j;
       j = strcmp(type,FType[i].fname);
       if ( j == 0 ) 
	 { 
	   return(FType[i].code);
	 }
       else 
	 { 
	   if ( j <= 0) 
	     break;
	   else i++;
	 }
     }
  return(EXTERNAL);
}

/* Type Scilab :  Renvoit la description (string) d'un type a partir de son code */

char *SGetForType(type)
     int type;
{
  int i=0;
  while ( FType[i].code  != -1 ) 
     {
       if ( FType[i].code == type ) 
	 return(FType[i].fname);
       else 
	 i++;
     }
  return("External");
}

/***************************************************************
  Code generation 
***************************************************************/


void WriteMainHeader(f,fname)
     FILE *f;
     char* fname;
{
  char *scidir;
  Fprintf(f,indent,"subroutine %s\n",fname);
  scidir = getenv("SCI");
  if ( scidir != NULL) 
    Fprintf(f,indent,"include '%s/routines/stack.h'\n",scidir);
  else 
    Fprintf(f,indent,"include 'SCIDIR/routines/stack.h'\n");
  Fprintf(f,indent,"rhs = max(0,rhs)\n");
  FCprintf(f,"c\n");
}


void WriteHeader(f,fname0,fname)
     FILE *f;
     char* fname,*fname0;
{
  char *scidir;
  Fprintf(f,indent,"subroutine %s%s(fname)\n",fname0,fname);
  FCprintf(f,"c\n"); 
  Fprintf(f,indent,"character*(*) fname\n");
  scidir = getenv("SCI");
  if ( scidir != NULL) 
    Fprintf(f,indent,"include '%s/routines/stack.h'\n",scidir);
  else 
    Fprintf(f,indent,"include 'SCIDIR/routines/stack.h'\n");
  FCprintf(f,"c\n");
  Fprintf(f,indent,"integer iadr, sadr\n");
  WriteDeclaration(f);
  Fprintf(f,indent,"iadr(l)=l+l-1\n");
  Fprintf(f,indent,"sadr(l)=(l/2)+1\n");
  Fprintf(f,indent,"rhs = max(0,rhs)\n");
  FCprintf(f,"c\n");
}

void WriteFunctionCode(f)
     FILE* f;
{
  int i;
  IVAR ivar;
  icre=1;
  if ( pass == 1) 
    {
      printf("  generating  code for SCILAB function\"%s\"\n",
	     basfun->name);
      printf("    and FORTRAN subroutine\"%s\"\n",forsub->name);
    }
  FCprintf(f,"c SCILAB function : %s, fin = %d\n",basfun->name,nFun);
  WriteHeader(f,"ints",basfun->name);
  
  /* possibly init for string flag */
  for (i = 0; i < forsub->narg; i++) 
    {
      if (variables[forsub->arg[i]-1]->for_type == CHAR) 
	{
	  Fprintf(f,indent,"lbuf = 1\n");
	  break;
	}
    }
  
  /* init for work space */
  
  AddDeclare(DEC_INT,"topk");
  AddDeclare(DEC_INT,"rhsk");
  Fprintf(f,indent,"topk = top \n");
  Fprintf(f,indent,"rhsk = rhs \n");
  
  /* rhs argument number checking */
  AddDeclare(DEC_LOGICAL,"checkrhs");
  Fprintf(f,indent,"if(.not.checkrhs(fname,%d,%d)) return\n",basfun->nin - maxOpt,basfun->nin);
  
  /* lhs argument number checking */
  ivar = basfun->out;
  if ((variables[ivar-1]->length == 0) || (variables[ivar-1]->type == LIST)
      || (variables[ivar-1]->type == TLIST))
    {
      AddDeclare(DEC_LOGICAL,"checklhs");
      Fprintf(f,indent,"if(.not.checklhs(fname,1,1)) return\n");
    }
  else 
    {
      AddDeclare(DEC_LOGICAL,"checklhs");
      Fprintf(f,indent,"if(.not.checklhs(fname,1,%d)) return\n",variables[ivar-1]->length);
    }
  
  /* SCILAB argument checking */
  for (i = 0; i < basfun->nin; i++)
    {
      switch ( variables[i]->type ) 
	{
	case LIST : 
	case TLIST:
	  WriteListAnalysis(f,i);
	  break;
        default:
	  WriteArgCheck(f,i);
	  break;
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

/* Ckecking and getting infos for datas coming from scilab calling 
   sequence ( datas on the stack ) 
*/

void WriteArgCheck(f,i)
     FILE *f;
     int i;
{
  int i1;
  char str[MAXNAM];
  char str1[MAXNAM];
  char size[MAXNAM];
  char data[MAXNAM];

  VARPTR var = variables[basfun->in[i]-1];
  i1 = i + 1;
  
  FCprintf(f,"c       checking variable %s (number %d)\n",var->name,i1);
  FCprintf(f,"c       \n");

  /* Optional Argument consideration */
  if (var->opt_type != 0) 
    {
      /** if (i1 < basfun->nin) {
	printf("Optional arguments must be at the end of the calling sequence\n");
	exit(1);
	}
	**/
      Fprintf(f,indent++,"if(rhs .le. %d) then\n", i1-1 );
      switch (var->opt_type) {
      case NAME:
	AddDeclare(DEC_LOGICAL,"optvarget");
	Fprintf(f,indent,"if (.not.optvarget(fname,topk,%d,'%s       ')) return\n",i1,var->opt_name);
	break;
      case VALUE:
	switch (var->type) {
	case SCALAR:
	  AddDeclare(DEC_LOGICAL,"cremat");
	  Fprintf(f,indent,"top = top+1\n");
	  Fprintf(f,indent,"rhs = rhs+1\n");
	  Fprintf(f,indent,"if(.not.cremat(fname,top,0,1,1,lr%d,lc%d)) return\n",i1,i1);
	  Fprintf(f,indent,"stk(lr%d)= %s\n",i1,var->opt_name);
	  break;
	case SCISMPOINTER:
	case SCILPOINTER:
	case SCIBPOINTER:
	case SCIOPOINTER:
	case SCIMPOINTER:
	  sprintf(buf,"cre%s", SGetSciType(var->type));
	  AddDeclare(DEC_LOGICAL,buf);
	  Fprintf(f,indent,"top = top+1\n");
	  Fprintf(f,indent,"rhs = rhs+1\n");
	  Fprintf(f,indent,"if(.not.cre%s(fname,top,lwv)) return\n", SGetSciType(var->type));
	  break;
	case MATRIX:
	  OptvarGetSize(var->opt_name,size,data);
	  AddDeclare(DEC_LOGICAL,"cremat");
	  Fprintf(f,indent,"top = top+1\n");
	  Fprintf(f,indent,"rhs = rhs+1\n");
	  sprintf(str,"dat%d %s",i1,data);
	  AddDeclare(DEC_DATA,str);
	  sprintf(str,"dat%d(%s)",i1,size);
	  AddDeclare(DEC_DOUBLE,str);
	  Fprintf(f,indent,"m%d = 1\n",i1);
	  Fprintf(f,indent,"n%d = %s\n",i1,size);
	  Fprintf(f,indent,"if(.not.cremat(fname,top,0,m%d,n%d,lr%d,lc%d)) return\n",i1,i1,i1,i1);
	  Fprintf(f,indent,"call dcopy(%s,dat%d,1,stk(lr%d),1)\n",size,i1,i1);
	  break;
	case STRING:
	  AddDeclare(DEC_LOGICAL,"cresmat2");
	  Fprintf(f,indent,"top = top+1\n");
	  Fprintf(f,indent,"rhs = rhs+1\n");
	  Fprintf(f,indent,"nlr%d = %d\n",i1,strlen(var->opt_name));
	  Fprintf(f,indent,"if(.not.cresmat2(fname,top,nlr%d,lr%d)) return\n",i1,i1,i1);
	  Fprintf(f,indent,"call cvstr(nlr%d,istk(lr%d),'%s',0)\n",i1,i1,var->opt_name);
	  break;
	default:
	  printf("Optional variable with value must be \"SCALAR\" or \"STRING\"\n");
	  exit(1);
	  break;
	}
	break;
      }
      Fprintf(f,--indent,"endif\n");
    }

  /* size checking */
  switch(var->type) 
    {
    case BMATRIX:
      AddDeclare(DEC_LOGICAL,"getbmat");
      Fprintf(f,indent,"if(.not.getbmat(fname,top,top-rhs+%d,m%d,n%d,lr%d)) return\n",i1,i1,i1,i1);
      /* square matrix */
      if (var->el[0] == var->el[1]) {
	/* square matrix */ 
	AddDeclare(DEC_LOGICAL,"checkval");
	Fprintf(f,indent,"if(.not.checkval(fname,m%d,n%d)) return\n",i1,i1);
      }
      sprintf(str,"m%d",i1);
      Check(f,str,var,i1,0);
      sprintf(str,"n%d",i1);
      Check(f,str,var,i1,1);
      break;
    case MATRIX:
    case IMATRIX:
      AddDeclare(DEC_LOGICAL,"getmat");
      Fprintf(f,indent,"if(.not.getmat(fname,top,top-rhs+%d,it%d,m%d,n%d,lr%d,lc%d)) return\n",i1,i1,i1,i1,i1,i1);
      /* square matrix */
      if (var->el[0] == var->el[1]) {
	/* square matrix */ 
	AddDeclare(DEC_LOGICAL,"checkval");
	Fprintf(f,indent,"if(.not.checkval(fname,m%d,n%d)) return\n",i1,i1);
      }
      sprintf(str,"m%d",i1);
      Check(f,str,var,i1,0);
      sprintf(str,"n%d",i1);
      Check(f,str,var,i1,1);
      sprintf(str,"it%d",i1);
      if (var->type == IMATRIX ) AddForName1(var->el[2],str);
      break;
    case SPARSE:
      AddDeclare(DEC_LOGICAL,"getsparse");
      Fprintf(f,indent,"if(.not.getsparse(fname,top,top-rhs+%d,it%d,m%d,n%d,nel%d,mnel%d,icol%d,lr%d,lc%d)) return\n",i1,i1,i1,i1,i1,i1,i1,i1,i1);
      /* square matrix */
      if (var->el[0] == var->el[1]) {
	/* square matrix */ 
	AddDeclare(DEC_LOGICAL,"checkval");
	Fprintf(f,indent,"if(.not.checkval(fname,m%d,n%d)) return\n",i1,i1);
      }
      sprintf(str,"m%d",i1);
      Check(f,str,var,i1,0);
      sprintf(str,"n%d",i1);
      Check(f,str,var,i1,1);
      sprintf(str,"nel%d",i1);
      AddForName1(var->el[2],str);
      sprintf(str,"it%d",i1);
      AddForName1(var->el[3],str);
      break;
    case STRINGMAT:
      AddDeclare(DEC_LOGICAL,"getsmat");
      Fprintf(f,indent,"if(.not.getsmat(fname,top,top-rhs+%d,m%d,n%d,1,1,lr%d,nlr%d)) return\n",
	      i1,i1,i1,i1,i1);
      /* square matrix */
      if (var->el[0] == var->el[1]) {
	/* square matrix */ 
	AddDeclare(DEC_LOGICAL,"checkval");
	Fprintf(f,indent,"if(.not.checkval(fname,m%d,n%d)) return\n",i1,i1);
      }
      sprintf(str,"m%d",i1);
      Check(f,str,var,i1,0);
      strcpy(str1,variables[var->el[0]-1]->name);
      sprintf(str,"n%d",i1);
      Check(f,str,var,i1,1);
      break;
    case ROW:
      AddDeclare(DEC_LOGICAL,"getvectrow");
      Fprintf(f,indent,"if(.not.getvectrow(fname,top,top-rhs+%d,it%d,m%d,n%d,lr%d,lc%d)) return\n",i1,i1,i1,i1,i1,i1);
      sprintf(str,"n%d",i1);
      Check(f,str,var,i1,0);
      break;
    case COLUMN:
      AddDeclare(DEC_LOGICAL,"getvectcol");
      Fprintf(f,indent,"if(.not.getvectcol(fname,top,top-rhs+%d,it%d,m%d,n%d,lr%d,lc%d)) return\n",i1,i1,i1,i1,i1,i1);
      sprintf(str,"m%d",i1);
      Check(f,str,var,i1,0);
      break;
    case VECTOR:
      AddDeclare(DEC_LOGICAL,"getvect");
      Fprintf(f,indent,"if(.not.getvect(fname,top,top-rhs+%d,it%d,m%d,n%d,lr%d,lc%d)) return\n",i1,i1,i1,i1,i1,i1);
      sprintf(str,"n%d*m%d",i1,i1);
      Check(f,str,var,i1,0);
      AddForName1(var->el[0],str);
      break;
    case POLYNOM:
      AddDeclare(DEC_LOGICAL,"getonepoly");
      sprintf(str,"namelr%d*4",i1);
      AddDeclare(DEC_CHAR,str);
      Fprintf(f,indent,"if(.not.getonepoly(fname,top,top-rhs+%d,it%d,m%d,namelr%d,namellr%d,lr%d,lc%d)\n",i1,i1,i1,i1,i1,i1,i1);
      AddDeclare(DEC_LOGICAL,"checkval");
      sprintf(str,"m%d",i1);
      Check(f,str,var,i1,0);
      AddForName(var->el[0],str);
      break;
    case SCALAR:
      AddDeclare(DEC_LOGICAL,"getscalar");
      Fprintf(f,indent,"if(.not.getscalar(fname,top,top-rhs+%d,lr%d)) return\n",i1,i1);
      break;
    case SCIMPOINTER:
    case SCISMPOINTER:
    case SCILPOINTER:
    case SCIBPOINTER:
    case SCIOPOINTER:
      sprintf(buf,"get%s", SGetSciType(var->type));
      AddDeclare(DEC_LOGICAL,buf);
      Fprintf(f,indent,"if(.not.get%s(fname,top,top-rhs+%d,lr%d)) return\n", SGetSciType(var->type),i1,i1);
      break;
    case STRING:
      AddDeclare(DEC_LOGICAL,"getsmat");
      Fprintf(f,indent,"if(.not.getsmat(fname,top,top-rhs+%d,m%d,n%d,1,1,lr%d,nlr%d)) return\n",i1,i1,i1,i1,i1,i1,11);
      AddDeclare(DEC_LOGICAL,"checkval");
      Fprintf(f,indent,"if(.not.checkval(fname,m%d*n%d,1)) return\n",i1,i1);
      sprintf(str,"nlr%d",i1);
      Check(f,str,var,i1,0);
      break;
    case ANY:
    case LIST:
    case TLIST:
      break;
    default:
      printf("unknown variable type %d\n",var->type);
    }
}

void OptvarGetSize(optvar,size,data)
     char *optvar,*size,*data;
{
  int i,j=0,ok=0;
  for ( i = 0 ; i < (int) strlen(optvar) ; i++ ) 
       {
	 if ( optvar[i] == ')' ) 
	   { 
	     size[j++] = '\0'; break;
	   }
	 if ( ok ==1 ) size[j++]= optvar[i];
	 if ( optvar[i] == '(' ) ok =1 ;
       }
  if ( i < (int) strlen(optvar)) strcpy(data,optvar+i+1);
}

/*
  Utility function for WriteArgCheck 
  Check for fixed sized dimensions 
*/

void Check(f,str,var,i1,nel)
     FILE *f;
     char *str;
     int i1, nel;
     VARPTR var;
{
  char str1[MAXNAM];
  strcpy(str1,variables[var->el[nel]-1]->name);
  if (isdigit(str1[0]) != 0) 
    {
      /* the dimension of the variable is a constant integer */
      if ( strcmp(str,str1) != 0) 
	{
	  AddDeclare(DEC_LOGICAL,"checkval");
	  Fprintf(f,indent,"if(.not.checkval(fname,%s,%s)) return\n",str,str1);
	}
    }
  AddForName1(var->el[nel],str);
}


void WriteCrossCheck(f)
     FILE *f;
{
  int i, j;
  char *n1, *n2;
  VARPTR var;
  FCprintf(f,"c     \n");        
  FCprintf(f,"c       cross variable size checking\n"); 
  FCprintf(f,"c     \n");               
  for (i = 0; i < nVariable; i++) {
    var = variables[i];
    /* does not check list elements */
    if (var->nfor_name != 0 && var->list_el == 0) {
      if (strncmp(var->for_name[0],"ne",2) != 0 &&
	  strncmp(var->for_name[0],"me",2) != 0) {
	n1 = Forname2Int(var->for_name[0]);
	for (j = 1; j < var->nfor_name; j++) {
	  n2 = Forname2Int(var->for_name[j]);
	  if ( strcmp(n1,n2) != 0) 
	    {
	      AddDeclare(DEC_LOGICAL,"checkval");
	      Fprintf(f,indent,"if(.not.checkval(fname,%s,%s)) return\n",n1,n2);
	    }
	}
      }
    }
  }
  /*  FCprintf(f,"c     \n");        
  FCprintf(f,"c       cross formal parameter checking\n");
  FCprintf(f,"c       not implemented yet\n");  */
}

void WriteEqualCheck(f)
     FILE *f;
{
  /* FCprintf(f,"c     \n");        
  FCprintf(f,"c       cross equal output variable checking\n");
  FCprintf(f,"c       not implemented yet\n"); */
}

void WriteFortranCall(f)
     FILE *f;
{
  int i, j, ind;
  IVAR ivar, iivar;
  char call[MAXCALL];
  char str1[8],str2[8];
  sprintf(call,"call %s(",forsub->name);
  /* loop on FORTRAN arguments */
  for (i = 0; i < forsub->narg; i++) 
    {
      ivar = forsub->arg[i];
      ind = 0;
      if (variables[ivar-1]->list_el != 0) 
	{
	  /* FORTRAN argument is a list element */
	  iivar = GetExistVar(variables[ivar-1]->list_name);
	  for (j = 0; j < basfun->nin; j++) 
	    {
	      if (iivar == basfun->in[j]) 
		{
		  /* it must be a SCILAB argument */
		  sprintf(str1,"%de%d",iivar,variables[ivar-1]->list_el);
		  sprintf(str2,"%de%d",iivar,variables[ivar-1]->list_el);
		  WriteCallConvertion(f,ivar,str2,str1,call);
		  ind = 1;
		  break;
		}
	    }
	  if (ind == 0) 
	    {
	      printf("list or tlist \"%s\" must be an argument of SCILAB function\n",
		     variables[ivar-1]->list_name);
	      exit(1);
	    }
	}
      else 
	{
	  for (j = 0; j < basfun->nin; j++) 
	    {
	      if (ivar == basfun->in[j]) 
		{
		  /* FORTRAN argument is a SCILAB argument */
		  sprintf(str1,"%d",j+1);
		  sprintf(str2,"%d",i+1);
		  WriteCallConvertion(f,ivar,str2,str1,call);
		  ind = 1;
		  break;
		}
	    }
	}
      if (ind == 0) 
	{
	  /* FORTRAN argument is not a SCILAB argument */
	  WriteCallRest(f,ivar,i+1,call);
	}
    }
  if  (forsub->narg == 0) 
    strcat(call,")");
  else 
    call[strlen(call)-1] = ')';
  Fprintf(f,indent,call);
  Fprintf(f,indent,"\n");
  /*    
      Fprintf(f,indent++,"if(err .gt. 0) then \n");  
      Fprintf(f,indent,"buf = fname // ' Internal Error' \n");  
      Fprintf(f,indent,"call error(999)\n");
      Fprintf(f,indent,"return\n");
      Fprintf(f,--indent,"endif\n");
  */
  Fprintf(f,indent,"if(err .gt. 0 .or. err1 .gt. 0) return \n");  

  FCprintf(f,"c\n");
}

/*
  Convertion to a Fortran type before caling sequence 
  for arguments coming from the scilab stack 
  the part of the caing sequence is adde to the buffer call
*/

void WriteCallConvertion(f,ivar,farg,barg,call)
     FILE *f;
     IVAR ivar; 
     char *farg;
     char *barg;
     char *call;
{
  VARPTR var = variables[ivar-1];
  char str[MAXNAM];
  char str1[MAXNAM];
  switch (var->type) 
    { 
    case POLYNOM:
    case ROW:
    case VECTOR:
    case SCALAR:
    case COLUMN:
    case IMATRIX:
    case MATRIX:
    case SPARSE:
      switch ( var->type )
	{
	case POLYNOM: sprintf(str1,"n%s",barg); break;
	case COLUMN: sprintf(str1,"m%s",barg); break ;
	case VECTOR:  sprintf(str1,"m%s*n%s",barg,barg); break ;
	case SCALAR:  sprintf(str1,"1"); break ;
	case ROW:  sprintf(str1,"n%s",barg); break;
	case SPARSE: sprintf(str1,"nel%s",barg);break;
	case IMATRIX:
	case MATRIX:  sprintf(str1,"n%s*m%s",barg,barg); break;
	}
      switch (var->for_type) 
	{
	case CHAR:
	case CSTRINGV:
	  printf("incompatibility between the variable type and the FORTRAN type for variable \"%s\"\n",var->name);
	  exit(1);
	case INT:
	  Fprintf(f,indent,"call entier(%s,stk(lr%s),istk(iadr(lr%s)))\n",str1,barg,barg);
	  if (var->type == IMATRIX || var->type == SPARSE) 
	    {
	      Fprintf(f,indent++,"if (it%s.eq.1) then \n",barg);
	      Fprintf(f,indent,"call entier(%s,stk(lc%s),istk(iadr(lc%s)))\n",str1,barg,barg);
	      Fprintf(f,--indent,"endif\n");
	      if ( var->type == SPARSE) 
		sprintf(str,"it%s,m%s,n%s,nel%s,istk(mnel%s),istk(icol%s),istk(iadr(lr%s)),istk(iadr(lc%s))"
			,barg,barg,barg,barg,barg,barg,barg,barg);
	      else 
		sprintf(str,"istk(iadr(lr%s)),istk(iadr(lc%s)),it%s",barg,barg,barg);
	      ChangeForName(ivar,str);
	      strcat(call,str);
	      strcat(call,",");
	    }
	  else 
	    {
	      sprintf(str,"istk(iadr(lr%s))",barg);
	      ChangeForName(ivar,str);
	      strcat(call,str);
	      strcat(call,",");
	    }
	  break;
	case REAL:
	  Fprintf(f,indent,"call simple(%s,stk(lr%s),stk(lr%s))\n",str1,barg,barg,barg);
	  if (var->type == IMATRIX || var->type == SPARSE)
	    {
	      Fprintf(f,indent++,"if (it%s.eq.1) then \n",barg);
	      Fprintf(f,indent,"call simple(%s,stk(lc%s),stk(lc%s))\n",str1,barg,barg);
	      Fprintf(f,--indent,"endif\n");
	      if ( var->type == SPARSE) 
		sprintf(str,"it%s,m%s,n%s,nel%s,istk(mnel%s),istk(icol%s),stk(lr%s),stk(lc%s),"
			,barg,barg,barg,barg,barg,barg,barg,barg);
	      else 
		sprintf(str,"stk(lr%s),stk(lc%s),it%s,",barg,barg,barg);
	      strcat(call,str);
	    }
	  else 
	    {
	      sprintf(str,"stk(lr%s),",barg);
	      strcat(call,str);
	    }
	  break;
	case DOUBLE:
	  if (var->type == IMATRIX) 
	    {
	      sprintf(str,"stk(lr%s),stk(lc%s),it%s,",barg,barg,barg);
	      strcat(call,str);
	    }
	  else if (var->type == SPARSE) 
	    {
	      sprintf(str,"it%s,m%s,n%s,nel%s,istk(mnel%s),istk(icol%s),stk(lr%s),stk(lc%s),"
		      ,barg,barg,barg,barg,barg,barg,barg,barg);
	      strcat(call,str);
	    }
	  else 
	    {
	      sprintf(str,"stk(lr%s),",barg);
	      strcat(call,str);
	    }
	  break;
	}    
      break;
    case BMATRIX:
      sprintf(str1,"n%s*m%s",barg,barg);
      if (var->for_type != INT) 
	{
	  printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
		 SGetSciType(var->type),SGetForType(var->for_type),var->name);
	  exit(1);
	}
      sprintf(str,"istk(lr%s)",barg);
      ChangeForName(ivar,str);
      strcat(call,str);
      strcat(call,",");
      break;
    case SCIMPOINTER:
      if (var->for_type != MPOINTER) 
	{
	  printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
		 SGetSciType(var->type),SGetForType(var->for_type),var->name);
	  exit(1);
	}
      sprintf(str,"stk(lr%s),",barg);
      strcat(call,str);
      break;
    case SCISMPOINTER:
      if (var->for_type != SMPOINTER) 
	{
	  printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
		 SGetSciType(var->type),SGetForType(var->for_type),var->name);
	  exit(1);
	}
      sprintf(str,"stk(lr%s),",barg);
      strcat(call,str);
      break;
    case SCILPOINTER:
      if (var->for_type != LPOINTER) 
	{
	  printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
		 SGetSciType(var->type),SGetForType(var->for_type),var->name);
	  exit(1);
	}
      sprintf(str,"stk(lr%s),",barg);
      strcat(call,str);
      break;
    case SCIBPOINTER:
      if (var->for_type != BPOINTER) 
	{
	  printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
		 SGetSciType(var->type),SGetForType(var->for_type),var->name);
	  exit(1);
	}
      sprintf(str,"stk(lr%s),",barg);
      strcat(call,str);
      break;
    case SCIOPOINTER:
      if (var->for_type != OPOINTER) 
	{
	  printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
		 SGetSciType(var->type),SGetForType(var->for_type),var->name);
	  exit(1);
	}
      sprintf(str,"stk(lr%s),",barg);
      strcat(call,str);
      break;
    case STRINGMAT:
      if (var->for_type != CSTRINGV) 
	{
	  printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
		 SGetSciType(STRINGMAT),SGetForType(var->for_type),var->name);
	  exit(1);
	}
      AddDeclare(DEC_LOGICAL,"crestringv");
      Fprintf(f,indent,"if(.not.crestringv(fname,top+%d,lr%s-5-m%s*n%s,lw%s)) return\n",icre++,barg,barg,barg,farg);
      sprintf(str,"stk(lw%s),",farg);
      strcat(call,str);
      break;
    case LIST:
    case TLIST:
    case SEQUENCE:
      printf("a FORTRAN argument cannot have a variable type of \"LIST\"\n");
      printf("  \"TLIST\" or \"SEQUENCE\"\n");
      exit(1);
      break;
    case STRING:
      if (var->for_type != CHAR) 
	{
	  printf("incompatibility between the type %s and FORTRAN type %s for variable \"%s\"\n",
		 SGetSciType(STRING),SGetForType(var->for_type),var->name);
	  exit(1);
	}
      AddDeclare(DEC_LOGICAL,"bufstore");
      Fprintf(f,indent,"if(.not.bufstore(fname,lbuf,lbufi%s,lbuff%s,lr%s,nlr%s)) return\n",farg,farg,barg,barg);
      sprintf(str,"buf(lbufi%s:lbuff%s),",farg,farg);
      strcat(call,str);
      break;
    case ANY:
      sprintf(str,"istk(il%s),",barg);
      strcat(call,str);
      break;
    }
}

/* 
  Calling sequence for variables not coming from the 
  scilab calling sequence 
  working or output variables 
*/

void WriteCallRest(f,ivar,farg,call)
     FILE *f;
     IVAR ivar;
     int farg;
     char *call;
{
  VARPTR var = variables[ivar-1];
  char str[MAXNAM];
  char str1[MAXNAM];
  char str2[MAXNAM];
  char str3[MAXNAM];
  char str4[MAXNAM];
  switch (var->type) 
    {
    case 0:
      /* FORTRAN argument is the dimension of an output variable with EXTERNAL type */
      if (var->nfor_name == 0 && var->for_type != PREDEF) 
	{
	  printf("dimension variable \"%s\" is not defined\n",var->name);
	  exit(1);
	}
      switch (var->for_type) 
	{
	case PREDEF:
	  if ( strcmp(var->name,"rhs") == 0) 
	    sprintf(str,"rhsk");
	  else 
	    sprintf(str,"%s",var->name);
	  strcat(call,str);
	  strcat(call,",");
	  break;
	case 0:
	case INT:
	  sprintf(str,"%s",var->for_name[0]);
	  if ( ~isdigit(str[0])) 
	    {
	      strcat(call,str);
	      strcat(call,",");
	    }
	  else 
	    {
	      Fprintf(f,indent,"locd%d= int(%s)\n",farg,var->for_name[0]);
	      sprintf(str,"locd%d,",farg);
	      strcat(call,str);
	      sprintf(str,"locd%d",farg);
	      AddDeclare(DEC_INT,str);
	    }
	  break;
	case DOUBLE:
	  Fprintf(f,indent,"locd%d= dble(%s)\n",farg,var->for_name[0]);
	  sprintf(str,"locd%d,",farg);
	  strcat(call,str);
	  sprintf(str,"locd%d",farg);
	  AddDeclare(DEC_DOUBLE,str);
	  break;
	case REAL:
	  Fprintf(f,indent,"locr%d=real(%s)\n",farg,var->for_name[0]);
	  sprintf(str,"locr%d,",farg);
	  strcat(call,str);
	  sprintf(str,"locr%d",farg);
	  AddDeclare(DEC_REAL,str);
	  break;
	case CHAR:
	case CSTRINGV:
	  printf("a dimension variable cannot have FORTRAN type \"%s\"\n",SGetForType(var->for_type));
	  exit(1);
	  break;
	}
      break;
    /* working or output argument (always double reservation!) */
    case COLUMN:
    case ROW:
    case WORK:
    case POLYNOM:
    case VECTOR:
      WriteCallRestCheck(f,var,farg,"nn",0,0) ;
      if (var->for_type == EXTERNAL) 
	strcpy(str1,"1");
      else 
	strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
      AddDeclare(DEC_LOGICAL,"cremat");
      Fprintf(f,indent,"if(.not.cremat(fname,top+%d,0,%s,1,lw%d,loc%d)) return\n",icre++,str1,farg,farg);
      sprintf(str,"stk(lw%d),",farg);
      strcat(call,str);
      break;
    case SPARSE :
      WriteCallRestCheck(f,var,farg,"nn",0,0) ;
      WriteCallRestCheck(f,var,farg,"mm",1,0) ;
      if (var->for_type == EXTERNAL) 
	{
	  strcpy(str1,"1");
	  strcpy(str2,"1");
	  Fprintf(f,indent,"if(.not.cremat(fname,top+%d,0,%s,%s,lw%d,lwc%d)) return\n",icre++,str1,str2,farg,farg);
	  AddDeclare(DEC_LOGICAL,"cremat");
	  sprintf(str,"stk(lw%d),",farg);
	  strcat(call,str);
	} 
      else 
	{
	  sprintf(str1,"%s",Forname2Int(variables[var->el[0]-1]->for_name[0]));
	  sprintf(str2,"%s",Forname2Int(variables[var->el[1]-1]->for_name[0]));
	  sprintf(str3,"%s",Forname2Int(variables[var->el[2]-1]->for_name[0]));
	  sprintf(str4,"%s",Forname2Int(variables[var->el[3]-1]->for_name[0]));
	  AddDeclare(DEC_LOGICAL,"cresparse");
	  Fprintf(f,indent,"if(.not.cresparse(fname,top+%d,%s,%s,%s,%s,mnel%d,icol%d,lw%d,lwc%d)) return\n",icre++,str4,str1,str2,str3,farg,farg,farg,farg);
	  sprintf(str,"%s,%s,%s,%s,istk(mnel%d),istk(icol%d),stk(lw%d),stk(lwc%d),",
		  str4,str1,str2,str3,farg,farg,farg,farg);
	  strcat(call,str);
	}
      break;
    case IMATRIX:
      WriteCallRestCheck(f,var,farg,"nn",0,0) ;
      WriteCallRestCheck(f,var,farg,"mm",1,0) ;
      if (var->for_type == EXTERNAL) 
	{
	  strcpy(str1,"1");
	  strcpy(str2,"1");
	  strcpy(str3,"1");
	} 
      else 
	{
	  sprintf(str1,"%s",Forname2Int(variables[var->el[0]-1]->for_name[0]));
	  sprintf(str2,"%s",Forname2Int(variables[var->el[1]-1]->for_name[0]));
	  sprintf(str3,"%s",Forname2Int(variables[var->el[2]-1]->for_name[0]));
	};
      AddDeclare(DEC_LOGICAL,"cremat");
      Fprintf(f,indent,"if(.not.cremat(fname,top+%d,%s,%s,%s,lw%d,lwc%d)) return\n",icre++,str3,str1,str2,farg,farg);
      sprintf(str3,"%s",Forname2Int(variables[var->el[2]-1]->for_name[0]));
      sprintf(str,"stk(lw%d),stk(lwc%d),%s,",farg,farg,str3);
      strcat(call,str);
      break;
    case MATRIX:
      WriteCallRestCheck(f,var,farg,"nn",0,0) ;
      WriteCallRestCheck(f,var,farg,"mm",1,0) ;
      if (var->for_type == EXTERNAL) 
	{
	  strcpy(str1,"1");
	  strcpy(str2,"1");
	} 
      else 
	{
	  sprintf(str1,"%s",Forname2Int(variables[var->el[0]-1]->for_name[0]));
	  sprintf(str2,"%s",Forname2Int(variables[var->el[1]-1]->for_name[0]));
	};
      AddDeclare(DEC_LOGICAL,"cremat");
      Fprintf(f,indent,"if(.not.cremat(fname,top+%d,0,%s,%s,lw%d,lwc%d)) return\n",icre++,str1,str2,farg,farg);
      sprintf(str,"stk(lw%d),",farg);
      strcat(call,str);
      break;
    case BMATRIX:
      WriteCallRestCheck(f,var,farg,"nn",0,0) ;
      WriteCallRestCheck(f,var,farg,"mm",1,0) ;
      if (var->for_type == EXTERNAL) 
	{
	  strcpy(str1,"1");
	  strcpy(str2,"1");
	} 
      else 
	{
	  sprintf(str1,"%s",Forname2Int(variables[var->el[0]-1]->for_name[0]));
	  sprintf(str2,"%s",Forname2Int(variables[var->el[1]-1]->for_name[0]));
	};
      AddDeclare(DEC_LOGICAL,"crebmat");
      Fprintf(f,indent,"if(.not.crebmat(fname,top+%d,%s,%s,lw%d)) return\n",icre++,str1,str2,farg);
      sprintf(str,"istk(lw%d),",farg);
      strcat(call,str);
      break;
    case SCIMPOINTER:
    case SCISMPOINTER:
    case SCILPOINTER:
    case SCIBPOINTER:
    case SCIOPOINTER:
      sprintf(buf,"cre%s", SGetSciType(var->type));
      AddDeclare(DEC_LOGICAL,buf);
      Fprintf(f,indent,"if(.not.cre%s(fname,top+%d,lw%d)) return\n", SGetSciType(var->type),icre++,farg);
      sprintf(str,"stk(lw%d),",farg);
      strcat(call,str);
      break;
    case STRINGMAT:
      if (var->for_type == EXTERNAL || var->for_type == CSTRINGV )
	{
	  /* for external or cstringv parameters, unknown formal dimensions 
	     can be used */
	  WriteCallRestCheck(f,var,farg,"mm",0,1) ;
	  WriteCallRestCheck(f,var,farg,"nn",1,1) ;
	  sprintf(str,"mm%d",farg);
	  AddForName1(var->el[0],str);
	  sprintf(str,"nn%d",farg);
	  AddForName1(var->el[1],str);
	  AddDeclare(DEC_LOGICAL,"crepointer");
	  Fprintf(f,indent,"if(.not.crepointer(fname,top+%d,lw%d)) return\n",icre++,farg);
	  sprintf(str,"stk(lw%d),",farg);
	  strcat(call,str);
	}
      else 
	{
	  /** XXXX dimensions should be specifief **/
	  fprintf(stderr,"WARNING : your code contains a specification \n");
	  fprintf(stderr," not fully implemented in intersci \n");
	  WriteCallRestCheck(f,var,farg,"mm",0,0) ;
	  WriteCallRestCheck(f,var,farg,"nn",1,0) ;
	  AddDeclare(DEC_LOGICAL,"cresmatafaire");
	  Fprintf(f,indent,"if(.not.cresmatafaire(fname,top+%d,lw%d)) return\n",icre++,farg);
	  sprintf(str,"stk(lw%d),",farg);
	  strcat(call,str);
	}
      break;
    case SCALAR:
      AddDeclare(DEC_LOGICAL,"cremat");
      Fprintf(f,indent,"if(.not.cremat(fname,top+%d,0,1,1,lw%d,loc%d)) return\n",icre++,farg,farg);
      sprintf(str,"stk(lw%d),",farg);
      strcat(call,str);
      break;
    case STRING:
      WriteCallRestCheck(f,var,farg,"nn",0,0) ;
      if (var->for_type == EXTERNAL) 
	{
	  AddDeclare(DEC_LOGICAL,"crepointer");
	  Fprintf(f,indent,"if(.not.crepointer(fname,top+%d,lw%d)) return\n",icre++,farg);
	  sprintf(str,"stk(lw%d),",farg);
	  strcat(call,str);
	}
      else 
	{
	  strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
	  AddDeclare(DEC_LOGICAL,"cresmat2");
	  Fprintf(f,indent,"if(.not.cresmat2(fname,top+%d,%s,lr%d)) return\n",icre++,str1,farg);
	  AddDeclare(DEC_LOGICAL,"bufstore");
	  Fprintf(f,indent,"if(.not.bufstore(fname,lbuf,lbufi%d,lbuff%d,lr%d,%s)) return\n",farg,farg,farg,str1);
	  sprintf(str,"buf(lbufi%d:lbuff%d),",farg,farg);
	  strcat(call,str);
	}
      break;
    case LIST:
    case TLIST:
    case SEQUENCE:
    case ANY:
      printf("work or output FORTRAN argument cannot have\n");
      printf("  type \"ANY\", \"LIST\", \"TLIST\" or \"SEQUENCE\"\n");
      exit(1);
      break;
    }
}

/* Utility function for WriteCallRest 
   when flag==1 we acccept undefined dimensions 
   this is used with stringmat/Cstringv 
   where dimensions and space are allocated inside 
   the interfaced function and returned 
   to the interface */

void WriteCallRestCheck(f,var,farg,name,iel,flag) 
     VARPTR var ;
     FILE *f;
     char *name;
     int iel,farg,flag;
{
  char sdim[MAXNAM]; 
  char str[MAXNAM];
  int ind,j;
  if (variables[var->el[iel]-1]->nfor_name == 0) 
    {
      strcpy(str,variables[var->el[iel]-1]->name);
      if (isdigit(str[0]) == 0) 
	{
	  ind = 0;
	  for (j = 0; j < basfun->nin; j++) 
	    {
	      if (strcmp(variables[var->el[iel]-1]->name,
			 variables[basfun->in[j]-1]->name) == 0) 
		{
		  /* dimension of FORTRAN argument is a SCILAB argument */
		  sprintf(sdim,"%s%d",name,farg);
		  Fprintf(f,indent,"%s= int(stk(lr%d))\n",sdim,j+1);
		  AddForName1(var->el[iel],sdim);
		  ind = 1;
		  break;
		}
	    }
	  if (ind == 0 && flag != 1 ) 
	    {
	      /** 
	      printf("dimension variable \"%s\" is not defined\n",
		     variables[var->el[iel]-1]->name);
	      exit(1);
	      **/
	    }
	} 
      else 
	{
	  sprintf(sdim,"%s%d",name,farg);
	  Fprintf(f,indent,"%s=%s\n",sdim,str);
	  AddForName1(var->el[iel],sdim);
	}
    }
}

void WriteOutput(f)
     FILE *f;
{
  IVAR iout,ivar;
  VARPTR var,vout;
  int i;
  
  Fprintf(f,indent,"topk=top-rhs\n");
  AddDeclare(DEC_INT,"topl");
  Fprintf(f,indent,"topl=top+%d\n",icre-1);
  
  iout = GetExistOutVar();
  vout = variables[iout -1];
  
  switch (vout->type) {
  case LIST:
  case TLIST:
    FCprintf(f,"c       Creation of output %s\n",SGetSciType(vout->type));
    Fprintf(f,indent,"top=topl+1\n");
    Fprintf(f,indent,"call cre%s(top,%d,lw)\n",SGetSciType(vout->type),vout->length);
    /* loop on output variables */
    for (i = 0; i < vout->length; i++) 
      {
	ivar = vout->el[i];
	var = variables[ivar-1];
	FCprintf(f,"c     \n");        
	FCprintf(f,"c       Element %d: %s\n",i+1,var->name);
	WriteVariable(f,var,ivar,1,i+1);
      }
    FCprintf(f,"c     \n");        
    FCprintf(f,"c     Putting in order the stack\n");
    Fprintf(f,indent,"call copyobj(fname,topl+1,topk+1)\n");
    Fprintf(f,indent,"top=topk+1\n");
    Fprintf(f,indent,"return\n");
    break;
  case SEQUENCE:
    /* loop on output variables */
    for (i = 0; i < vout->length; i++) 
      {
	ivar = vout->el[i];
	var = variables[ivar-1];
	FCprintf(f,"c     \n");        
	Fprintf(f,indent++,"if(lhs .ge. %d) then\n",i+1);
	FCprintf(f,"c       --------------output variable: %s\n",var->name);
	Fprintf(f,indent,"top=topl+%d\n",i+1);
	WriteVariable(f,var,ivar,0,0);
	Fprintf(f,--indent,"endif\n");
      }
    FCprintf(f,"c     Putting in order the stack\n");
    for (i = 0; i < vout->length; i++) 
      {
	Fprintf(f,indent++,"if(lhs .ge. %d) then\n",i+1);
	Fprintf(f,indent,"call copyobj(fname,topl+%d,topk+%d)\n",i+1,i+1);
	Fprintf(f,--indent,"endif\n");
      }
    Fprintf(f,indent,"top=topk+lhs\n");
    Fprintf(f,indent,"return\n");
    break;
  case EMPTY:
    FCprintf(f,"c       no output variable\n");
    Fprintf(f,indent,"top=topk+1\n");
    Fprintf(f,indent,"call objvide(fname,top)\n");
    Fprintf(f,indent,"return\n");
    break;
  }
  Fprintf(f,indent,"end\n");
  FCprintf(f,"c\n"); 
}

void WriteVariableOutput(f,var,barg,farg,convert,insidelist,nel)
     FILE *f;
     VARPTR var;
     int barg, farg, insidelist,convert,nel;
{
  char strR[MAXNAM];
  char str[MAXNAM];
  char strc[MAXNAM];
  char strit[MAXNAM];
  char str1[MAXNAM];
  char str2[MAXNAM];
  char str3[MAXNAM];
  char str4[MAXNAM];
  if (convert == 1) {
    /* seams to give a bug with ex13fi.desc 
    barg = 0;
    */
  }
  /* farg and barg depends on the list or not list case */

  /* if ( insidelist != 0 && var->list_name != (char *) 0 ) */
  /* jpc sept 1997 : was the var inside a list or not */

  if ( var->list_name != (char *) 0 ) 
    {
      int ivart,fargt,bargt,nel1;
      ivart=GetExistVar(var->list_name);
      bargt=GetNumberInScilabCall(ivart);
      fargt=GetNumberInFortranCall(ivart);
      nel1= var->list_el;
      if (convert == 1)
	{
	  /* tricky, isn'it ? */
	  bargt = 0; 
	}     
      if (bargt == 0) 
	{
	  sprintf(strR,"%de%d",fargt,nel1);
	  sprintf(str,"lw%de%d",fargt,nel1);
	  sprintf(strc,"lwc%de%d",fargt,nel1);
	  sprintf(strit,"itr%de%d",fargt,nel1);
	}
      else
	{
	  sprintf(strR,"%de%d",bargt,nel1);
	  sprintf(str,"lr%de%d",bargt,nel1);
	  sprintf(strc,"lc%de%d",bargt,nel1);
	  sprintf(strit,"it%de%d",bargt,nel1); /** sept97 itr->it **/
	}      
    }
  else 
    {
      if (barg == 0) 
	{
	  sprintf(strR,"%d",farg);
	  sprintf(str,"lw%d",farg);
	  sprintf(strc,"lwc%d",farg);
	  sprintf(strit,"itr%d",farg);
	}
      else
	{
	  sprintf(strR,"%d",barg);
	  sprintf(str,"lr%d",barg);
	  sprintf(strc,"lc%d",barg);
	  sprintf(strit,"it%d",farg); /** Mars 1997 itr -> it **/
	}
    }
  switch (var->type) 
    {
    case COLUMN:
    case ROW:
    case VECTOR:
    case MATRIX:
    case SCALAR:
      switch ( var->type  ) 
	{
	case COLUMN:
	  strcpy(str2,"1");
	  strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
	  break;
	case ROW:
	case VECTOR:
	  strcpy(str1,"1");
	  strcpy(str2,Forname2Int(variables[var->el[0]-1]->for_name[0]));
	  break;
	case MATRIX:
	  strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
	  strcpy(str2,Forname2Int(variables[var->el[1]-1]->for_name[0]));
	  break;
	case SCALAR:
	  strcpy(str1,"1");
	  strcpy(str2,"1");
	  break;
	}
      if ( insidelist != 0)
	{
	  AddDeclare(DEC_LOGICAL,"listcremat");
	  Fprintf(f,indent,"if(.not.listcremat(fname,top,%d,lw,0,%s,%s,lrs,lcs)) return\n",nel,str1,str2);
	}
      else 
	{
	  AddDeclare(DEC_LOGICAL,"cremat");
	  Fprintf(f,indent,"if(.not.cremat(fname,top,0,%s,%s,lrs,lcs)) return\n",str1,str2);
	}
      if (barg != 0 &&  var->type != SCALAR) 
	{
	  sprintf(str1,"n%d",barg);
	  sprintf(str2,"m%d",barg);
	}
      switch (var->for_type) 
	{
	case INT:
	   Fprintf(f,indent,"call int2db(%s*%s,istk(iadr(%s)),-1,stk(lrs),-1)\n", str1,str2,str);
	  break;
	case DOUBLE:
	  Fprintf(f,indent,"call dcopy(%s*%s,stk(%s),1,stk(lrs),1)\n", str1,str2,str);
	  break;
	case REAL:
	  Fprintf(f,indent,"call rea2db(%s*%s,stk(%s),-1,stk(lrs),-1)\n", str1,str2,str);
	  break;
	}  
      break;
    case BMATRIX:
      strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
      strcpy(str2,Forname2Int(variables[var->el[1]-1]->for_name[0]));
      if ( insidelist != 0)
	{
	  AddDeclare(DEC_LOGICAL,"listcrebmat");
	  Fprintf(f,indent,"if(.not.listcrebmat(fname,top,%d,lw,%s,%s,lrs)) return\n",nel,str1,str2);
	}
      else 
	{
	  AddDeclare(DEC_LOGICAL,"crebmat");
	  Fprintf(f,indent,"if(.not.crebmat(fname,top,%s,%s,lrs)) return\n",str1,str2);
	}
      if (barg != 0 &&  var->type != SCALAR) 
	{
	  sprintf(str1,"n%d",barg);
	  sprintf(str2,"m%d",barg);
	}
      Fprintf(f,indent,"call icopy(%s*%s,istk(%s),1,istk(lrs),1)\n", str1,str2,str);
      break;
    case SPARSE:
      /* Sparse matrix */
      strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
      strcpy(str2,Forname2Int(variables[var->el[1]-1]->for_name[0]));
      strcpy(str3,Forname2Int(variables[var->el[2]-1]->for_name[0]));
      strcpy(str4,Forname2Int(variables[var->el[3]-1]->for_name[0]));
      if ( insidelist != 0)
	{
	  AddDeclare(DEC_LOGICAL,"listcresparse");
	  Fprintf(f,indent,"if(.not.listcresparse(fname,top,%d,lw,%s,%s,%s,%s,mnels,icols,lrs,lcs)) return\n"
		  ,nel,str4,str1,str2,str3);
	}
      else
 	{
	  AddDeclare(DEC_LOGICAL,"cresparse");
	  Fprintf(f,indent,"if(.not.cresparse(fname,top,%s,%s,%s,%s,mnels,icols,lrs,lcs)) return\n"
		  ,str4,str1,str2,str3);
	}
      if (barg != 0 &&  var->type != SCALAR) 
	{
	  sprintf(str1,"m%d",barg);
	  sprintf(str2,"n%d",barg);
	  sprintf(str3,"nel%d",barg);
	  sprintf(str4,"it%d",barg);
	}

      Fprintf(f,indent,"call icopy(%s,istk(mnel%s),1,istk(mnels),1)\n",
	      str1,strR);
      Fprintf(f,indent,"call icopy(%s,istk(icol%s),1,istk(icols),1)\n",
	      str3,strR);
      switch (var->for_type) 
	{
	case INT:
	  Fprintf(f,indent,"call int2db(%s,istk(iadr(%s)),-1,stk(lrs),-1)\n", str3,str);
	  Fprintf(f,indent++,"if (%s.eq.1) then\n",str4);
	  Fprintf(f,indent,"call int2db(%s,istk(iadr(%s)),-1,stk(lcs),-1)\n", str3,strc);
	  Fprintf(f,indent--,"endif\n");
	  break;
	case DOUBLE:
	  Fprintf(f,indent,"call dcopy(%s,stk(%s),1,stk(lrs),1)\n", str3,str);
	  Fprintf(f,indent++,"if (%s.eq.1) then\n",str4);
	  Fprintf(f,indent,"call dcopy(%s,stk(%s),1,stk(lcs),1)\n", str3,strc);
	  Fprintf(f,indent--,"endif\n");
	  break;
	case REAL:
	  Fprintf(f,indent,"call rea2db(%s,stk(%s),-1,stk(lrs),-1)\n", str3,str);
	  Fprintf(f,indent++,"if (%s.eq.1) then\n",str4);
	  Fprintf(f,indent,"call rea2db(%s,stk(%s),-1,stk(lcs),-1)\n", str3,strc);
	  Fprintf(f,indent--,"endif\n");
	  break;
	}  
      break ;
    case IMATRIX: 
      /* Imaginary matrix */
      strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
      strcpy(str2,Forname2Int(variables[var->el[1]-1]->for_name[0]));
      strcpy(str3,Forname2Int(variables[var->el[2]-1]->for_name[0]));
      if ( insidelist != 0)
	{
	  AddDeclare(DEC_LOGICAL,"listcremat");
	  Fprintf(f,indent,"if(.not.listcremat(fname,top,%d,lw,%s,%s,%s,lrs,lcs)) return\n"
		  ,nel,str3,str1,str2);
	}
      else 
	{
	  AddDeclare(DEC_LOGICAL,"cremat");
	  Fprintf(f,indent,"if(.not.cremat(fname,top,%s,%s,%s,lrs,lcs)) return\n",
		  str3,str1,str2);
	}
      if (barg != 0 &&  var->type != SCALAR) 
	{
	  sprintf(str1,"m%d",barg);
	  sprintf(str2,"n%d",barg);
	  sprintf(str3,"it%d",barg);
	}
      switch (var->for_type) 
	{
	case INT:
	  Fprintf(f,indent,"call int2db(%s*%s,istk(iadr(%s)),-1,stk(lrs),-1)\n", str1,str2,str);
	  Fprintf(f,indent++,"if (%s.eq.1) then\n",str3);
	  Fprintf(f,indent,"call int2db(%s*%s,istk(iadr(%s)),-1,stk(lcs),-1)\n", str1,str2,strc);
	  Fprintf(f,indent--,"endif\n");
	  break;
	case DOUBLE:
	  Fprintf(f,indent,"call dcopy(%s*%s,stk(%s),1,stk(lrs),1)\n", str1,str2,str);
	  Fprintf(f,indent++,"if (%s.eq.1) then\n",str3);
	  Fprintf(f,indent,"call dcopy(%s*%s,stk(%s),1,stk(lcs),1)\n", str1,str2,strc);
	  Fprintf(f,indent--,"endif\n");
	  break;
	case REAL:
	  Fprintf(f,indent,"call rea2db(%s*%s,stk(%s),-1,stk(lrs),-1)\n", str1,str2,str);
	  Fprintf(f,indent++,"if (%s.eq.1) then\n",str3);
	  Fprintf(f,indent,"call rea2db(%s*%s,stk(%s),-1,stk(lcs),-1)\n", str1,str2,strc);
	  Fprintf(f,indent--,"endif\n");
	  break;
	}  
      break ;
   case POLYNOM:
      strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
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
      break;
    case STRING:
      strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
      if (barg != 0) 
	{
	  sprintf(str1,"nlr%d",barg);
	}

      if ( insidelist != 0)
	{
	  AddDeclare(DEC_LOGICAL,"listcrestring");
	  Fprintf(f,indent,"if(.not.listcrestring(fname,top,%d,lw,%s,ilrs)) return\n",
		  nel,str1);
	}
      else 
	{
	  AddDeclare(DEC_LOGICAL,"cresmat2");
	  Fprintf(f,indent,"if(.not.cresmat2(fname,top,%s,ilrs)) return\n",str1);
	}
      Fprintf(f,indent,"call cvstr(%s,istk(ilrs),buf(lbufi%d:lbuff%d),0)\n",
	      str1,farg,farg);
      break;
    case SCISMPOINTER:
    case SCIMPOINTER:
    case SCILPOINTER:
    case SCIBPOINTER:
    case SCIOPOINTER:
      if ( insidelist != 0)
	{
	  printf(" %s in output list : not implemented ;",SGetSciType(var->type));
	}
      else 
	{
	  sprintf(buf,"cre%s", SGetSciType(var->type));
	  AddDeclare(DEC_LOGICAL,buf);
	  Fprintf(f,indent,"if(.not.cre%s(fname,top,lrs)) return\n", SGetSciType(var->type));
	  Fprintf(f,indent,"stk(lrs)=stk(%s)\n",str);
	}
      break;
    case STRINGMAT:
      strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
      strcpy(str2,Forname2Int(variables[var->el[1]-1]->for_name[0]));
      sprintf(str,"lw%d",farg);
      if ( insidelist != 0)
	{
	  AddDeclare(DEC_LOGICAL,"lcrestringmatfromC");
	  Fprintf(f,indent,"if(.not.lcrestringmatfromC(fname,top,%d,lw,%s,%s,%s)) return\n",
		  nel,str,str1,str2);
	}
      else
	{
	  AddDeclare(DEC_LOGICAL,"crestringmatfromC");
	  Fprintf(f,indent,"if(.not.crestringmatfromC(fname,top,%s,%s,%s)) return\n",
		  str,str1,str2);
	}
      break;
    case WORK:
    case LIST:
    case TLIST:
    case SEQUENCE:
    case ANY:
      printf("output variable \"%s\" cannot have type\n",
	     var->name);
      printf("  \"WORK\", \"LIST\", \"TLIST\", \"SEQUENCE\" or \"ANY\"\n");
      exit(1);
      break;
    }
}

void WriteExternalVariableOutput(f,var,farg,insidelist,nel)
     FILE *f;
     VARPTR var;
     int farg;
     int insidelist,nel;
{
  char str[MAXNAM];
  char str1[MAXNAM];
  char str2[MAXNAM];
  char str3[MAXNAM];
  char str4[MAXNAM];
  switch (var->type) {
  case COLUMN:
  case ROW:
  case VECTOR:
  case MATRIX:
  case SCALAR:
    switch ( var->type  ) 
      {
      case COLUMN:
	strcpy(str2,"1");
	strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
	break;
      case ROW:
      case VECTOR:
	strcpy(str1,"1");
	strcpy(str2,Forname2Int(variables[var->el[0]-1]->for_name[0]));
	break;
      case MATRIX:
	strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
	strcpy(str2,Forname2Int(variables[var->el[1]-1]->for_name[0]));
	break;
      case SCALAR:
	strcpy(str1,"1");
	strcpy(str2,"1");
	break;
      }
    if ( insidelist != 0)
      {
	AddDeclare(DEC_LOGICAL,"listcremat");
	Fprintf(f,indent,"if(.not.listcremat(fname,top,%d,lw,0,%s,%s,lrs,lcs)) return\n",nel,
		str1,str2);
      }
    else 
      {
	AddDeclare(DEC_LOGICAL,"cremat");
	Fprintf(f,indent,"if(.not.cremat(fname,top,0,%s,%s,lrs,lcs)) return\n",str1,str2);
      }
    sprintf(str,"lw%d",farg);
    Fprintf(f,indent,"call %s(%s*%s,stk(%s),stk(lrs))\n",  var->fexternal,str1,str2,str);
    break;
  case IMATRIX:
    strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
    strcpy(str2,Forname2Int(variables[var->el[1]-1]->for_name[0]));
    strcpy(str3,Forname2Int(variables[var->el[2]-1]->for_name[0]));
    if ( insidelist != 0)
      {
	AddDeclare(DEC_LOGICAL,"listcremat");
	Fprintf(f,indent,"if(.not.listcremat(fname,top,%d,lw,%s,%s,%s,lrs,lcs)) return\n",
		nel,str3,str1,str2);
      }
    else 
      {
	AddDeclare(DEC_LOGICAL,"cremat");
	Fprintf(f,indent,"if(.not.cremat(fname,top,%s,%s,%s,lrs,lcs)) return\n",str3,str1,str2);
      }
    sprintf(str,"lw%d",farg);
    Fprintf(f,indent,"call %s(%s*%s,stk(%s),stk(lrs))\n",  var->fexternal,str1,str2,str);
    sprintf(str,"lwc%d",farg);
    Fprintf(f,indent++,"if  (%s.eq.1) then \n",str3);
    Fprintf(f,indent,"call %s(%s*%s,stk(%s),stk(lcs))\n",  var->fexternal,str1,str2,str);
    Fprintf(f,--indent,"endif\n");
    break;
  case SPARSE:
    strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
    strcpy(str2,Forname2Int(variables[var->el[1]-1]->for_name[0]));
    strcpy(str3,Forname2Int(variables[var->el[2]-1]->for_name[0]));
    strcpy(str4,Forname2Int(variables[var->el[3]-1]->for_name[0]));
    if ( insidelist != 0)
      {
	AddDeclare(DEC_LOGICAL,"listcresparse");
	Fprintf(f,indent,"if(.not.listcresparse(fname,top,%d,lw,%s,%s,%s,%s,mnels,icols,lrs,lcs)) return\n"
		,nel,str4,str1,str2,str3);
      }
    else 
      {
	AddDeclare(DEC_LOGICAL,"cresparse");
	Fprintf(f,indent,"if(.not.cresparse(fname,top,%s,%s,%s,%s,mnels,icols,lrs,lcs)) return\n"
		,str4,str1,str2,str3);
      }
    sprintf(str,"lw%d",farg);
    Fprintf(f,indent,"call %s(stk(%s),istk(mnels),istk(icols),stk(lrs),stk(lcs))\n",var->fexternal,str);
    break;
  case BMATRIX:
    strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
    strcpy(str2,Forname2Int(variables[var->el[1]-1]->for_name[0]));
    if ( insidelist != 0)
      {
	AddDeclare(DEC_LOGICAL,"listcrebmat");
	Fprintf(f,indent,"if(.not.listcrebmat(fname,top,%d,lw,%s,%s,lrs)) return\n",
		nel,str1,str2);
      }
    else 
      {
	AddDeclare(DEC_LOGICAL,"crebmat");
	Fprintf(f,indent,"if(.not.crebmat(fname,top,%s,%s,lrs)) return\n",str1,str2);
      }
    sprintf(str,"lw%d",farg);
    Fprintf(f,indent,"call %s(%s*%s,istk(%s),istk(lrs))\n",var->fexternal,str1,str2,str);
    break;
  case POLYNOM:
    strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
    strcpy(str2,variables[var->el[1]-1]->name);
    Fprintf(f,indent,"err=sadr(ilw+10)+%s-lstk(bot)\n",str1);
    Fprintf(f,indent,"if(err .gt. 0) then\n");
    Fprintf(f,indent,"call error(17)\n");
    Fprintf(f,indent,"return\n");
    Fprintf(f,indent,"endif\n");
    Fprintf(f,indent,"istk(ilw)=1\n");
    Fprintf(f,indent,"istk(ilw+1)=1\n");		    
    Fprintf(f,indent,"istk(ilw+2)=1\n");
    Fprintf(f,indent,"istk(ilw+3)=0\n");
    Fprintf(f,indent,"call cvstr(4,'%s    ',istk(ilw+4),0)\n",str2);
    /* str2 comes from SCILAB input ??? */
    Fprintf(f,indent,"istk(ilw+8)=1\n");
    Fprintf(f,indent,"istk(ilw+9)=1+%s\n",str1);
    sprintf(str,"lw%d",farg);
    Fprintf(f,indent,"lw=sadr(ilw+10)\n");
    Fprintf(f,indent,"call %s(%s,stk(%s),stk(lw))\n",
	    var->fexternal,str1,str);
    Fprintf(f,indent,"lw=lw+%s\n",str1);
    break;
  case STRING:
    strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
    if ( insidelist != 0)
      {
	AddDeclare(DEC_LOGICAL,"listcrestring");
	Fprintf(f,indent,"if(.not.listcrestring(fname,top,%d,lw,%s,ilrs)) return\n",nel,str1,str2);
      }
    else 
      {
	AddDeclare(DEC_LOGICAL,"crestring");
	Fprintf(f,indent,"if(.not.crestring(fname,top,%s,ilrs)) return\n",str1);
      }
    sprintf(str,"lw%d",farg);	
    Fprintf(f,indent,"call %s(%s,stk(%s),istk(ilrs))\n", var->fexternal,str1,str);
    break;
  case STRINGMAT:
    strcpy(str1,Forname2Int(variables[var->el[0]-1]->for_name[0]));
    strcpy(str2,Forname2Int(variables[var->el[1]-1]->for_name[0]));
    sprintf(str,"lw%d",farg);
    Fprintf(f,indent,"call %s(stk(%s),istk(ilw),%s,%s,lstk(bot)-sadr(ilw),ierr)\n",
	    var->fexternal,str,str1,str2);
    Fprintf(f,indent,"if(ierr .gt. 0) then\n");
    Fprintf(f,indent,"buf='not enough memory'\n");
    Fprintf(f,indent,"call error(1000)\n");
    Fprintf(f,indent,"return\n");
    Fprintf(f,indent,"endif\n");
    sprintf(str,"istk(ilw+4+%s*%s)-1",str1,str2);
    Fprintf(f,indent,"lw=sadr(ilw+5+%s*%s+%s)\n",str1,str2,str);
    break;
  }
}

void WriteListAnalysis(f,i)
     /* is nearly a copy of WriteArgCheck */
     FILE *f;
     int i;
{
  int k,i1,iel;
  char str1[MAXNAM],str[MAXNAM];
  VARPTR var;
  i1=i+1;
  AddDeclare(DEC_LOGICAL,"getilist");
  Fprintf(f,indent,"if(.not.getilist(fname,topk,top-rhs+%d,n%d,1,il%d)) return\n",i1,i1,i1);
  for (k = 0; k < nVariable ; k++) 
    {
      var = variables[k];
      if ((var->list_el != 0) &&
	  (strcmp(var->list_name,variables[i]->name) == 0) &&
	  var->present)
	{
	  iel=var->list_el;
	  FCprintf(f,"c      \n");
	  FCprintf(f,"c       --   list element number %d %s --\n",iel,var->name);
	  sprintf(str1,"%de%d",i1,iel);
	  AddDeclare(DEC_LOGICAL,"getilist");
	  switch (var->type) 
	    {
	    case SPARSE:
	      AddDeclare(DEC_LOGICAL,"getlistsparse");
	      Fprintf(f,indent,"if(.not.getlistsparse(fname,topk,top-rhs+%d,%d,it%s,m%s,n%s,nel%s,mnel%s,icol%s,lr%s,lc%s)) return\n",
		      i1,iel,str1,str1,str1,str1,str1,str1,str1,str1);
	      if (var->el[0] == var->el[1]) {
		/* square matrix */ 
		AddDeclare(DEC_LOGICAL,"checkval");
		Fprintf(f,indent,"if(.not.checkval(fname,m%s,n%s)) return\n",str1,str1);
	      }
	      sprintf(str,"m%s",str1);
	      AddForName1(var->el[0],str);
	      sprintf(str,"n%s",str1);
	      AddForName1(var->el[1],str);
	      sprintf(str,"nel%s",str1);
	      AddForName1(var->el[2],str);
	      sprintf(str,"it%s",str1);
	      AddForName1(var->el[3],str);
	      break;
	    case IMATRIX:
	      AddDeclare(DEC_LOGICAL,"getlistmat");
	      Fprintf(f,indent,"if(.not.getlistmat(fname,topk,top-rhs+%d,%d,it%s,m%s,n%s,lr%s,lc%s)) return\n",
		      i1,iel,str1,str1,str1,str1,str1);
	      if (var->el[0] == var->el[1]) {
		/* square matrix */ 
		AddDeclare(DEC_LOGICAL,"checkval");
		Fprintf(f,indent,"if(.not.checkval(fname,m%s,n%s)) return\n",str1,str1);
	      }
	      sprintf(str,"m%s",str1);
	      AddForName1(var->el[0],str);
	      sprintf(str,"n%s",str1);
	      AddForName1(var->el[1],str);
	      sprintf(str,"it%s",str1);
	      AddForName1(var->el[2],str);
	      break;
	    case MATRIX:
	      AddDeclare(DEC_LOGICAL,"getlistmat");
	      Fprintf(f,indent,"if(.not.getlistmat(fname,topk,top-rhs+%d,%d,it%s,m%s,n%s,lr%s,lc%s)) return\n",
		      i1,iel,str1,str1,str1,str1,str1);
	      if (var->el[0] == var->el[1]) {
		/* square matrix */ 
		AddDeclare(DEC_LOGICAL,"checkval");
		Fprintf(f,indent,"if(.not.checkval(fname,m%s,n%s)) return\n",str1,str1);
	      }
	      sprintf(str,"m%s",str1);
	      AddForName1(var->el[0],str);
	      sprintf(str,"n%s",str1);
	      AddForName1(var->el[1],str);
	      break;
	    case BMATRIX:
	      AddDeclare(DEC_LOGICAL,"getlistbmat");
	      Fprintf(f,indent,"if(.not.getlistbmat(fname,topk,top-rhs+%d,%d,m%s,n%s,lr%s)) return\n",
		      i1,iel,str1,str1,str1,str1,str1);
	      if (var->el[0] == var->el[1]) {
		/* square matrix */ 
		AddDeclare(DEC_LOGICAL,"checkval");
		Fprintf(f,indent,"if(.not.checkval(fname,m%s,n%s)) return\n",str1,str1);
	      }
	      sprintf(str,"m%s",str1);
	      AddForName1(var->el[0],str);
	      sprintf(str,"n%s",str1);
	      AddForName1(var->el[1],str);
	      break;
	    case STRINGMAT:
	      AddDeclare(DEC_LOGICAL,"getlistsimat");
	      Fprintf(f,indent,"if(.not.getlistsimat(fname,topk,top-rhs+%d,%d,m%s,n%s,1,1,lr%s,nlr%s)) return\n",
		      i1,iel,str1,str1,str1,str1);
	      /* square matrix */
	      if (var->el[0] == var->el[1]) {
		/* square matrix */ 
		AddDeclare(DEC_LOGICAL,"checkval");
		Fprintf(f,indent,"if(.not.checkval(fname,m%s,n%s)) return\n",str1,str1);
	      }
	      sprintf(str,"m%s",str1);
	      AddForName1(var->el[0],str);
	      sprintf(str,"n%s",str1);
	      AddForName1(var->el[1],str);
	      break;
	    case ROW:
	      AddDeclare(DEC_LOGICAL,"getlistvectrow");
	      Fprintf(f,indent,"if(.not.getlistvectrow(fname,topk,top-rhs+%d,%d,it%s,m%s,n%s,lr%s,lc%s)) return\n",
		      i1,iel,str1,str1,str1,str1,str1);
	      sprintf(str,"n%s",str1);
	      AddForName1(var->el[0],str);
	      break;
	    case COLUMN :
	      AddDeclare(DEC_LOGICAL,"getlistvectcol");
	      Fprintf(f,indent,"if(.not.getlistvectcol(fname,topk,top-rhs+%d,%d,it%s,m%s,n%s,lr%s,lc%s)) return\n",
		      i1,iel,str1,str1,str1,str1,str1);
	      sprintf(str,"n%s",str1);
	      AddForName1(var->el[0],str);
	      break;
	    case VECTOR:
	      AddDeclare(DEC_LOGICAL,"getlistvect");
	      Fprintf(f,indent,"if(.not.getlistvect(fname,top,top-rhs+%d,%d,it%s,m%s,n%s,lr%s,lc%s)) return\n",
		      i1,iel,str1,str1,str1,str1,str1);
	      sprintf(str,"n%s*m%s",str1,str1);
	      AddForName1(var->el[0],str);
	      break;
	    case POLYNOM:
	      AddDeclare(DEC_LOGICAL,"getlistpoly");
	      Fprintf(f,indent,"if(.not.getlistpoly(fname,topk,top-rhs+%d,%d,it%s,m%s,n%s,name%s,\n",
		      i1,iel,str1,str1,str1,str1);
	      Fprintf(f,indent,"$     namel%s,ilp%s,lr%s,lc%s)\n",str1,str1,str1,str1);
	      AddDeclare(DEC_LOGICAL,"checkval");
	      Fprintf(f,indent,"if(.not.checkval(fname,m%s,n%s)) return\n",str1,str1);
	      break;
	    case SCALAR:
	      AddDeclare(DEC_LOGICAL,"getlistscalar");
	      Fprintf(f,indent,"if(.not.getlistscalar(fname,topk,top-rhs+%d,%d,lr%s)) return\n",i1,iel,str1);
	      break;
	    case STRING:
	      AddDeclare(DEC_LOGICAL,"getlistsmat");
	      Fprintf(f,indent,"if(.not.getlistsmat(fname,topk,top-rhs+%d,%d,m%s,n%s,1,1,lr%s,nlr%s)) return\n",
		      i1,iel,str1,str1,str1,str1);
	      AddDeclare(DEC_LOGICAL,"checkval");
	      Fprintf(f,indent,"if(.not.checkval(fname,m%s*n%s,1)) return\n",str1,str1);
	      sprintf(str,"n%s",str1);
	      strcpy(str1,variables[var->el[0]-1]->name);
	      AddForName1(var->el[0],str);
	      break;
	    case ANY:
	      break;
	    }
	}
    }
}

/***********************************************
 * Output of variable var 
 *   if variable is outputed inside a list 
 *   insidelist is set to true and nel is the number 
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
      WriteExternalVariableOutput(f,var,farg,insidelist,nel);
    }
  else 
    {
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
	  WriteVariableOutput(f,var,barg,farg,1,insidelist,nel);
	}
      else 
	{
	  /* no SCILAB type convertion */
	  if (farg == 0) {
	    printf("variable without convertion \"%s\" must be an argument\n",
		   var->name);
	    printf("  of FORTRAN subroutine\n");
	    exit(1);
	  }
	  WriteVariableOutput(f,var,barg,farg,0,insidelist,nel);
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

char unknown[]="ukn";

char *Forname2Int(str)
     char *str;
{
  int l;
  char *p;
  if (str == (char *) 0) 
    {
      printf("Error in Forname2Int  \n");
      printf("Maybe an internal variable has a dimension\n");
      printf("which can't be evaluated\n");
      abort();
      return(unknown);
    }
  if (strncmp(str,"stk",3) == 0) {
    l = strlen(str);
    p = (char *)malloc((unsigned)(l + 6));
    sprintf(p,"int(%s)",str);
    return p;
  }
  else return str;
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

/**********************************************************
  Function to add decaration during the first pass 
  and to print them during code generation pass 2
**********************************************************/


static struct Declare {
  int type;
  char *name;
  char **decls ; /* declaration de logical */
  int  ndecls;
} Init[] = {
  { DEC_CHAR,"char",(char **) 0,0},
  { DEC_INT ,"integer",(char **) 0,0},
  { DEC_DOUBLE,"double precision",(char **) 0,0},
  { DEC_REAL,"real",(char **) 0,0},
  { DEC_LOGICAL,"logical",(char **) 0,0},
  { DEC_DATA,"data",(char **) 0,0},
  { -1 ,"void",(char **) 0,0}
  };

void InitDeclare() 
{ 
  int i = 0;
  while ( Init[i].type != -1) 
    {
      Init[i].decls = (char **) 0;
      Init[i].ndecls =0 ;
      i++;
    }
}

void ResetDeclare()
{
  int j = 0;
  while ( Init[j].type != -1) 
    {
      if ( Init[j].decls != (char **) 0) 
	{
	  int i;
	  for ( i = 0 ; i < Init[j].ndecls ; i++ ) 
	    free((char *) Init[j].decls[i]);
	  free (( char *) Init[j].decls );
	}
      Init[j].decls=(char **) 0;
      Init[j].ndecls=0;
      j++;
    }
}

int  CheckDeclare(type,declaration)
     int type;
     char *declaration;
{
  int j = 0;
  while ( Init[j].type != -1) 
    {
      if ( Init[j].type == type ) 
	{
	  int i;
	  for ( i = 0 ; i < Init[j].ndecls ; i++ ) 
	    {
	      if ( strcmp(declaration,Init[j].decls[i])==0) 
		return(1);
	    }
	  return(0);
	}
      j++;
    }
  return(0);
}

void AddDeclare(type,declaration) 
     int type;
     char *declaration;
{
  int j = 0;
  if ( CheckDeclare(type,declaration)== 1) return ;
  while ( Init[j].type != -1) 
    {
      if ( Init[j].type == type ) 
	{
	  if ( Init[j].decls != (char **) 0) 
	    {
	      (Init[j].ndecls)++;
	      Init[j].decls =  (char **) realloc((char *) Init[j].decls, (unsigned) (Init[j].ndecls ) *sizeof(char *));
	    }
	  else 
	    {
	      (Init[j].ndecls)++;
	      Init[j].decls = (char **) malloc ( (unsigned) (Init[j].ndecls ) *sizeof(char *));
	    }
	  if ( Init[j].decls == ( char **) 0) 
	    {
	      fprintf(stderr,"No more space\n");
	      exit(1);
	    }
	  Init[j].decls[Init[j].ndecls-1]=(char*) malloc((unsigned) (strlen(declaration)+1)*sizeof(char));
	  if ( 	  Init[j].decls[Init[j].ndecls-1] == ( char *) 0) 
	    {
	      fprintf(stderr,"No more space\n");
	      exit(1);
	    }
	  strcpy(   Init[j].decls[Init[j].ndecls-1], declaration );
	}
      j++;
    }
}

void WriteDeclaration(f) 
     FILE *f;
{
  int j = 0;
  int i;
  while ( Init[j].type != -1) 
    {
      if ( Init[j].ndecls != 0) 
	Fprintf(f,indent,"%s ",Init[j].name);
      for (i= 0 ; i < Init[j].ndecls ; i++) 
	{
	  Fprintf(f,indent,"%s",Init[j].decls[i]);
	  if ( i != Init[j].ndecls -1 ) Fprintf(f,indent,",");
	  else Fprintf(f,indent,"\n");
	}
      j++;
    }
}

/**********************************************************
  Dealing With Fortran OutPut 
  taking into account indentation and line breaks after column 72
***********************************************************/
#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif 

#define MAXBUF 4096 
char sbuf[MAXBUF];

#ifdef __STDC__ 
void Fprintf(FILE *f,int indent,char *format,...)
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
  FILE *f;
  int indent;
  char *format;
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
	  white(f,7+indent);
	  count = 7+indent;
	}
      if ( count == 72 && sbuf[i] != '\n' ) { fprintf(f,"\n     $ ");count=7;}
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
void  FCprintf(va_alist) va_dcl
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



/********************************
 * Set up environment variables if 
 * necessary for SCIDIR 
 ********************************/

#ifdef WIN32 
static void SciEnv ()
{
  char *p,*p1;
  char modname[MAX_PATH+1];
  char env[MAX_PATH+1+10];
  if (!GetModuleFileName (NULL, modname+1, MAX_PATH))
    return;
  if ((p = strrchr (modname+1, '\\')) == NULL)
    return;
  *p = 0;

  /* Set SCI variable */
  if ((p = strrchr (modname+1, '\\')))
    {
      *p = 0;
      for (p = modname+1; *p; p++)
	{
	  if (*p == '\\') *p = '/';
	}
#ifdef __CYGWIN32__ 
      if ( modname[2] == ':' ) 
	{
	  modname[2] = modname[1];
	  modname[0] = '/';
	  modname[1] = '/';
	  p = modname;
	}
      else 
	{
	  p = modname + 1;
	}
#else 
      p = modname + 1;
#endif 
      if ( ( p1 = getenv("SCI"))  == (char *) 0 )
	{  
	  sprintf(env,"SCI=%s",p); 
	  putenv(env); 
	}
    }
}
#endif /** WIN32 **/
