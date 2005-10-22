/* Copyright ENPC */
#ifdef WIN32 
#include "wmen_scilab.h"
#else
#include "men_scilab.h"
#endif

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

#define OK 1
#define CANCEL 2
#define MEMERR 3

PrintDial ScilabPrintD;

static char *Print_Formats[] = {
  "Postscript",
  "Postscript No Preamble",
  "Postscript-Latex",
  "Xfig",
  "Gif",
  "PPM",
};

static int nF=6;

/******************************************
 * main function 
 ******************************************/

int prtdlg(integer *flag, char *printer, integer *colored, integer *orientation, char *file, integer *ok)
{ 
  static int firstentry=0;
  static int multiple_call=0;
  int rep ;
  if ( firstentry == 0) 
    {
      ScilabPrintD.numChoice=1;
      ScilabPrintD.Pbuffer = NULL;
      ScilabPrintD.PList = NULL;
      firstentry++;
    }
  /** multiple calls are forbidden **/
  if ( multiple_call == 1 )
    {
      sciprint(" Can't raise the print menu : you must quit another raised menu before \r\n");
      *ok=0;
      return(0);
    }
  else
    {
      multiple_call = 1;
    }
  *ok=1;
  rep = ExposePrintdialogWindow((int) *flag,colored,orientation);
  if ( rep == TRUE ) 
    {
      strcpy(printer,ScilabPrintD.PList[ScilabPrintD.numChoice-1]);
      if (*flag==2) {strcpy(file,ScilabPrintD.filename);FREE(ScilabPrintD.filename);}
    }
  else 
    {
      *ok=0;
    }
  if (*flag==1) 
    {
      FREE(ScilabPrintD.Pbuffer);
      FREE(ScilabPrintD.PList);
    }
  multiple_call =0;
  return(0);
}


/******************************************
 * Initialize list of printers 
 ******************************************/

int SetPrinterList(int flag)
{
  int n,i,npr;
  char *getenv(const char *),*str,*p;
  if (flag == 1) 
    {
      /* searching for printers */

      if ( (str=getenv("PRINTERS")) == 0) str="lp";
      n=strlen(str);
      if (n==0) 
	{
	  str="lp";n=strlen(str);
	}
      /* counting number of printers */
      npr=1;
      for (i=0 ; str[i] != '\0' ;i++)
	if(str[i]==':' ) npr++;
      ScilabPrintD.PList=(char **) MALLOC((npr)*sizeof(char *));
      ScilabPrintD.Pbuffer=(char *) MALLOC( (strlen(str)+1)*sizeof(char));
      if ( ScilabPrintD.Pbuffer != (char *) 0 && ScilabPrintD.PList != (char **) 0)
	{
	  strcpy(ScilabPrintD.Pbuffer,str);
	  ScilabPrintD.ns=0;
	  while ( ScilabPrintD.ns < npr ) 
	    {
	      p=(ScilabPrintD.ns == 0) ? strtok(ScilabPrintD.Pbuffer,":") : strtok((char *)0,":");
	      ScilabPrintD.PList[ScilabPrintD.ns]=p;
	      ScilabPrintD.ns++;
	    }
	}
      else 
	{
	  sciprint("x_choices : No more place\r\n");
	  return(MEMERR);
	}
    }
  else 
    {
      ScilabPrintD.PList=Print_Formats;
      ScilabPrintD.ns=nF;
    }
  return(OK);
}
