/* UI generator main module  */
/* Bertrand Guiheneuf, INRIA 1997 */

#include "C-LAB_Interf.h"
#include "TK_uicontrol.h"
#include "TK_ScilabCallback.h"
#include "tksci.h"



/***/
/* forward declarations */
char *Mat2Str();

int Str2MatReal();

int Str2ListStr();

char *ListStr2Str();

char *UiStyleName[] = {"button", "checkbutton", "checkbutton", "entry", "label",
		       "scale", "frame", "scrolllistbox", "popupmenu"};






/***/
int GetStyle(StyleStr)
     char *StyleStr;

{
  if (! strcmp(StyleStr,"pushbutton"           )) return 0;
  else if (! strcmp(StyleStr,"radiobutton"     )) return 1;
  else if (! strcmp(StyleStr,"checkbox"        )) return 2;
  else if (! strcmp(StyleStr,"edit"            )) return 3;
  else if (! strcmp(StyleStr,"text"            )) return 4;
  else if (! strcmp(StyleStr,"slider"          )) return 5;
  else if (! strcmp(StyleStr,"frame"           )) return 6;
  else if (! strcmp(StyleStr,"listbox"         )) return 7;
  else if (! strcmp(StyleStr,"popupmenu"       )) return 8;
  else return(-1);

}



/***/
int TK_UiSet( int  Handle,Matrix * Mfield,Matrix * Mvalue)
{
  char *StrField,  *StrValue;
  char MyCommand[2000];
  int mem_siz;
    
  StrField = MatrixReadString(Mfield);
  nocase(StrField);
  if (MatrixIsString(Mvalue))
    {
      StrValue = MatrixReadString(Mvalue);
      /* nocase(StrValue);  (modif Bruno 14/1/2001) */
    }
  else if (MatrixIsReal(Mvalue))
    StrValue = Mat2Str(Mvalue);
  else if (MatrixIsList(Mvalue))
    StrValue = ListStr2Str(Mvalue);
  else 
    {
      return(-1);
    }
  
  if (!strcmp(StrField,"userdata")) 
    {
      mem_siz = MatrixMemSize(Mvalue);
      UserData[Handle]=(Matrix *)malloc(mem_siz);
      MatrixCopy(Mvalue,UserData[Handle]);
    }
  else
    {
      sprintf(MyCommand,"SetField %d \"%s\" \"%s\"",Handle,StrField,StrValue);
      Tcl_Eval(TKinterp,MyCommand);
    }
  return(0);

}
    
 


/***/
int TK_UiGet(int Handle,Matrix * Mfield,Matrix ** Mvalue)
{
  char *StrField;
  
  char MyCommand[2000];
  char *MyAnswer;
  Matrix * TmpMat;
  int mem_siz;

  StrField = MatrixReadString(Mfield);
  nocase(StrField);

  sprintf(MyCommand,"set MyTmpBertrand [GetField %d \"%s\"]", Handle, StrField);
   
  Tcl_Eval(TKinterp,MyCommand);
  MyAnswer = Tcl_GetVar(TKinterp, "MyTmpBertrand", 0);

  if ( MyAnswer == NULL) 
    {
      *Mvalue = MatrixCreate(0,0,"real");
      return(0);
    }

    
  if (!strcmp(StrField,"position")) {Str2MatReal(MyAnswer, Mvalue); return(0); }
  if (!strcmp(StrField,"value"))    {Str2MatReal(MyAnswer, Mvalue); return(0); }
  if (!strcmp(StrField,"min"))      {Str2MatReal(MyAnswer, Mvalue); return(0); }
  if (!strcmp(StrField,"max"))      {Str2MatReal(MyAnswer, Mvalue); return(0); }
  if (!strcmp(StrField,"userdata")) 
    {
      if (UserData[Handle]!=NULL) 
	{
	  mem_siz = MatrixMemSize(UserData[Handle]);
	  TmpMat=(Matrix *)malloc(mem_siz);
	  MatrixCopy(UserData[Handle],TmpMat);
	  *Mvalue = TmpMat;
	}
      else
	*Mvalue = MatrixCreate(0,0,"real");
      return(0);
    }
    
  Str2ListStr(MyAnswer, Mvalue);
  return(0);
    
}
 





/***/
char *Mat2Str(Min)
     Matrix *Min;
{
#define ENT 10
#define MANT 5

  double *in;
  int n,m,sz;
  int i,strl;

  char *retstr;
  char *tmpstr;

  in = MatrixGetPr(Min);
  m=MatrixGetHeight(Min);
  n=MatrixGetWidth(Min);
  sz=m*n;

  if (sz>0) 
    {
      strl = sz * (ENT * MANT + 2);
      /* the before the dot, after the dot, the dot and a space */
      
      retstr = malloc( (1+strl) * sizeof(char));
      tmpstr = malloc( (ENT * MANT + 2) * sizeof(char));
      
      strcpy(retstr,"");
      
      for (i=0; i<(sz-1); i++)
	{
	  sprintf(tmpstr,"%.10lf|", in[i]);
	  strcat( retstr,tmpstr );
	}
      
      sprintf(tmpstr,"%.10lf", in[i]);
      strcat( retstr,tmpstr );
      
      free(tmpstr);
    }
  else
    { 
      retstr = malloc( sizeof(char) );
      strcpy(retstr,"");
    }
  
  return(retstr);
  
}
  
  



/***/
int Str2MatReal(str, Mat)
     char *str;
     Matrix **Mat;
     /* Split a string (str) which contains float numbers  separated by '|' */
     /* and return a vector formed with those numbers.                      */

{
  int 
    lgth,
    nbelem,
    i,
    begin_elem,
    end_elem,
    elem;
  char *tmpstr;
  double *MatPr;
  

  lgth = strlen(str);
  nbelem=1;

  if (lgth>0)
    {
      tmpstr = (char *)malloc((lgth+1) * sizeof(char));
      
      /* How many elements in the string ? */
      for (i=0; i<(lgth-1); i++)
	if (str[i]=='|')	nbelem++;
      
      /* Let's ask for some mem. */
      *Mat=(Matrix *)MatrixCreate(1, nbelem, "real");
      MatPr=MatrixGetPr(*Mat);

      
      begin_elem=0;
      end_elem=0;

      for (elem=0; elem<nbelem; elem++)
	{
	  
	  while ( (end_elem<lgth) && (str[end_elem] != '|') )
	    {
	      
	      end_elem++;
	    }
	  
	  strncpy(tmpstr, str+begin_elem, end_elem-begin_elem);
	  *(MatPr++)=atof(tmpstr);
	  begin_elem = end_elem+1;
	  end_elem = begin_elem;
	}
    }
  
  /* string was empty */
  else *Mat = MatrixCreate(0,0,"real");

  return 1;



}
	  
	      
  
/***/
int Str2ListStr(str, Mat)
     char *str;
     Matrix **Mat;
     /* Split a string (str) which contains substrings separated by '|' */
     /* and return a scilab list  formed with those substrings          */

{
  int 
    lgth,
    nbelem,
    i,
    begin_elem,
    end_elem,
    elem;
  char *tmpstr;

  Matrix *MTmpStr;
  

  lgth = strlen(str);
  nbelem=1;

  if (lgth>0)
    {
      tmpstr = (char *)malloc((lgth+1) * sizeof(char));
      
      /* How many elements in the string ? */
      for (i=0; i<(lgth-1); i++)
	if (str[i]=='|')	nbelem++;
      
      /* Let's ask for some mem. */
      *Mat=(Matrix *)ListCreate();
      

      
      begin_elem=0;
      end_elem=0;
      if (nbelem>1)
	/* there is more than one substring  in the string */
	/* so we create a scilab list of strings           */
	{
	  for (elem=0; elem<nbelem; elem++)
	    {
	      
	      while ( (end_elem<lgth) && (str[end_elem] != '|') )
		{
		  
		  end_elem++;
		}
	      
	      strncpy(tmpstr, str+begin_elem, end_elem-begin_elem);
	      tmpstr[end_elem-begin_elem]='\0';
	      MTmpStr = MatrixCreateString(tmpstr);
	      *Mat=AppendList(*Mat, MTmpStr);
	      free(MTmpStr);
	      begin_elem = end_elem+1;
	      end_elem = begin_elem;
	    }
	}
      else
	/* the string contains no separator, so we return a scilab string */
	{
	  *Mat = MatrixCreateString(str);
	}
    }
  /* string was empty */
  else *Mat = MatrixCreateString("");
      
      
  return 1;
}
  
	      
  
	

char *ListStr2Str(l)
     Matrix *l;
     /* concat. a list of strings into a single string */
     /* the substrings are saprated by a '|'           */
{
  Matrix *Mtmp;
  int bigsize=0;
  char *tmpstr;
  char *bigstr;
  int sz;
  char *begin;
  int nbelem, i;

  if (!MatrixIsList(l)) { InterfError("Input parameter must be a list"); return(NULL);}
  nbelem = ListGetSize(l);
  
  for (i=0; i<nbelem; i++) 
    {
      Mtmp = ListGetCell(i, l);
      if (!MatrixIsString(Mtmp) ) 
	{ 
	  InterfError("One element of the list is not a string");
	  return(NULL);
	}
      
      tmpstr = MatrixReadString(Mtmp);
      bigsize += strlen(tmpstr);
      free(tmpstr);
    }

  bigstr = (char *)malloc( (bigsize + nbelem) *sizeof(char));

  /* set the first string into the result */
  begin = bigstr;

  Mtmp = ListGetCell(0, l);
  tmpstr = MatrixReadString(Mtmp);
  sz=strlen(tmpstr);
  strncpy(begin, tmpstr, sz);
  free(tmpstr);
  begin += sz;

  for (i=1; i<nbelem; i++)
    {
      /* now set all the other */
      *(begin++)='|';
      Mtmp = ListGetCell(i , l);
      tmpstr = MatrixReadString(Mtmp);
      sz=strlen(tmpstr);
      strncpy(begin, tmpstr, sz);
      free(tmpstr);
      begin += sz;
  
    }
  /* add EOL mark */
  *begin='\0';

  return(bigstr);


}
