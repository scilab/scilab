/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TCL_Global.h"
/*-----------------------------------------------------------------------------------*/
Tcl_Interp *TCLinterp=NULL;
Tk_Window TKmainWindow=NULL;
/*-----------------------------------------------------------------------------------*/
#define NBRPROPERTIES 25
static char *UiPropertiesName[NBRPROPERTIES]={
												"backgroundcolor",
												"callback",
												"fontangle",
												"fontsize",
												"fontunits",
												"fontweight",
												"fontname",
												"listboxtop",
												"max",
												"min",
												"parent",
												"position",
												"sliderstep",
												"string",
												"style",
												"tag",
												"units",
												"userdata",
												"label",
												"figure_name",
												"value",
												"verticalalignment",
												"horizontalalignment",
												"path",
												"foregroundcolor"
											};
/*-----------------------------------------------------------------------------------*/
void nocase (char *s)
{
#ifndef WIN32
  int lg;
  int i;
  char c;
 
  lg = strlen(s);
  for (i=0; i<lg; i++)
    {
      c = s[i];
      if ( (c >= 'A') && (c <= 'Z') ) s[i] = 'a' + (c-'A');
    }
#else
  s=_strlwr(s);
#endif
} 
/*-----------------------------------------------------------------------------------*/
char *Matrix2String(int RhsMatrix)
{
	
	static int l1,n1,m1;
	double *param=NULL;
	int sz=0;
	char *retstr=NULL;
	char *tmpstr=NULL;
    
	GetRhsVar(RhsMatrix,"d",&m1,&n1,&l1);
	param=stk(l1);
	
	sz=m1*n1;

    if (sz>0) 
    {
		#define ENT 10
		#define MANT 5
		int i=0;
		int strl=0;
		strl = sz * (ENT * MANT + 2);
		/* the before the dot, after the dot, the dot and a space */
      
		retstr = (char*) MALLOC( (1+strl) * sizeof(char));
		tmpstr = (char*) MALLOC( (ENT * MANT + 2) * sizeof(char));
      
		strcpy(retstr,"");
        strcpy(tmpstr,"");

		for (i=0; i<(sz-1); i++)
		{
			sprintf(tmpstr,"%.10lf|", param[i]);
			strcat( retstr,tmpstr );
		}
      
		sprintf(tmpstr,"%.10lf", param[i]);
		strcat( retstr,tmpstr );
      
        if (tmpstr) {FREE(tmpstr);tmpstr=NULL;}
    }
	else
    { 
      retstr = (char*) MALLOC( sizeof(char) );
      strcpy(retstr,"");
    }
  
  return(retstr);
  
}
/*-----------------------------------------------------------------------------------*/  
double *String2Matrix(char *StringIn,int *nbelemOut)
{
    double *MatrixReturn=NULL;
	int nbelem=0;
	int Stringlen=strlen(StringIn);

	if (Stringlen>0)
	{
		int begin_elem=0;
		int end_elem=0;
		int elem=0;
		int i=0;
		char *tmpstr=NULL;

		/* How many elements in the string ? */
		for (i=0; i<(Stringlen-1); i++)  if (StringIn[i]=='|')	nbelem++;

		nbelem++;

		MatrixReturn=(double*) MALLOC(nbelem*sizeof(double));
		tmpstr = (char *)MALLOC((Stringlen+1) * sizeof(char));

		for (elem=0; elem<nbelem; elem++)
		{
			while ( (end_elem<Stringlen) && (StringIn[end_elem] != '|') )
			{
	            end_elem++;
			}
	  
			strncpy(tmpstr, StringIn+begin_elem, end_elem-begin_elem);
			MatrixReturn[elem]=atof(tmpstr);
			begin_elem = end_elem+1;
			end_elem = begin_elem;
		}
        if (tmpstr) {FREE(tmpstr);tmpstr=NULL;}
		*nbelemOut=nbelem;
	}

	return MatrixReturn;
}
/*-----------------------------------------------------------------------------------*/
int MustReturnAString(char *FieldPropertie)
{
	int bOK=0;
	char Propertie[256];

	strcpy(Propertie,FieldPropertie);
	nocase(Propertie);

	if ( (strcmp(Propertie,"style") == 0) ||
		 (strcmp(Propertie,"tag") == 0) ||
		 (strcmp(Propertie,"units") == 0) ||
		 (strcmp(Propertie,"callback") == 0) ||
		 (strcmp(Propertie,"fontangle") == 0) ||
		 (strcmp(Propertie,"fontunits") == 0) ||
		 (strcmp(Propertie,"fontweight") == 0) ||
		 (strcmp(Propertie,"fontname") == 0) ||
		 (strcmp(Propertie,"string") == 0) ||
		 (strcmp(Propertie,"label") == 0) ||
		 (strcmp(Propertie,"figure_name") == 0) ||
	     (strcmp(Propertie,"verticalalignment") == 0) ||
	     (strcmp(Propertie,"horizontalalignment") == 0) ||
	     (strcmp(Propertie,"path") == 0) ) bOK=1;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int MustReturnAMatrix(char *FieldPropertie)
{
	int bOK=0;
	char Propertie[256];

	strcpy(Propertie,FieldPropertie);
    nocase(Propertie);

	if ( (strcmp(Propertie,"backgroundcolor") == 0) ||
		 (strcmp(Propertie,"fontsize") == 0) ||
		 (strcmp(Propertie,"listboxtop") == 0) ||
		 (strcmp(Propertie,"max") == 0) ||
		 (strcmp(Propertie,"min") == 0) ||
		 (strcmp(Propertie,"parent") == 0) ||
		 (strcmp(Propertie,"position") == 0) ||
		 (strcmp(Propertie,"sliderstep") == 0) ||
		 (strcmp(Propertie,"value") == 0) ||
         (strcmp(Propertie,"foregroundcolor") == 0)) bOK=1;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int ValueMustBeAMatrix(char *FieldPropertie)
{
	return (MustReturnAMatrix(FieldPropertie));
}
/*-----------------------------------------------------------------------------------*/
int ValueMustBeAString(char *FieldPropertie)
{
	return (MustReturnAString(FieldPropertie));
}
/*-----------------------------------------------------------------------------------*/
int CheckPropertyField(char *FieldPropertie)
{
	int bOK=0;
	int i=0;
	char Propertie[256];

	strcpy(Propertie,FieldPropertie);
    nocase(Propertie);
	
	for (i=0;i<NBRPROPERTIES;i++)
	{
		if ( strcmp(Propertie,UiPropertiesName[i])==0 )
		{
			bOK=1;
		}
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
char *UTF8toANSI(Tcl_Interp *TCLinterp,char *StringUTF8)
{
	#define AddCharacters 4
	char *ReturnANSIString=NULL;
	if (StringUTF8)
	{
		ReturnANSIString=MALLOC(sizeof(char)*(strlen(StringUTF8)+AddCharacters));
		/* UTF to ANSI */
		Tcl_UtfToExternal(TCLinterp, NULL, StringUTF8, strlen(StringUTF8), 0, NULL, ReturnANSIString, (int)(strlen(StringUTF8)+AddCharacters), NULL, NULL,NULL);
	}
	return ReturnANSIString;
}
/*-----------------------------------------------------------------------------------*/
