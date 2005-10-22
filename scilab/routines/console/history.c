/*------------------------------------------------------------------------
 *    Copyright (C) Inria 2004
 *--------------------------------------------------------------------------*/

#include "history.h"

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h"  /* malloc */
#else
#include "../os_specific/sci_mem_alloc.h"  /* malloc */
#endif
#include  "../stack-c.h"
#define HISTORY_ID 3

#ifndef Max 
#define Max(x,y)	(((x)>(y))?(x):(y))
#endif

#define MaxHistorySize  450*1024 /* 450 ko limitation history size in memory */

sci_hist *history = NULL;	/* no history yet */
sci_hist *cur_entry = NULL;
/* Use for SearchInHistory --> ! */
sci_hist *research_knot_last = NULL;

static int HistorySizeInMemory=0;

BOOL NewSearchInHistory=FALSE; /* rlgets wsci\command.c */

extern char *get_sci_data_strings(int n);
extern int C2F(cluni0) __PARAMS((char *name, char *nams, integer *ln, long int name_len,long int nams_len));  
/*-----------------------------------------------------------------------------------*/
static char *ASCIItime(const struct tm *timeptr)
{
    static char wday_name[7][3] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };
    static char mon_name[12][3] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    
    static char result[26];

    sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d",
        wday_name[timeptr->tm_wday],
        mon_name[timeptr->tm_mon],
        timeptr->tm_mday, timeptr->tm_hour,
        timeptr->tm_min, timeptr->tm_sec,
        1900 + timeptr->tm_year);
        
    return result;
}
/*-----------------------------------------------------------------------------------*/
void GetCommentDateSession(char *line,int BeginSession)
{
	time_t timer;
  	timer=time(NULL);
  	
  	if (BeginSession)
		sprintf(line,"// Begin Session : %s ",ASCIItime(localtime(&timer)) );
	else
		sprintf(line,"// End Session : %s  ",ASCIItime(localtime(&timer)) );
}
/*-----------------------------------------------------------------------------------*/
/* adds string line to the end of history 
 */
void AddHistory (char *line)
{
  sci_hist *entry=NULL;

  if ( (HistorySizeInMemory-MaxHistorySize) > 0 )
  {
	  	  reset_history();
		  AddHistory ("// -- Unknown date --");
  }
  
  if (history)
  {
  	if  (strcmp(history->line,line) == 0)
  	{
  		return;
  	}
  }
  
  entry = (sci_hist *) MALLOC ((unsigned long) sizeof (sci_hist));
  
  if (entry)
  {
	  entry->line = (char *) MALLOC ((strlen (line) + 1)*sizeof(char));  
	  if (entry->line)
	  {
		  HistorySizeInMemory=HistorySizeInMemory+( ((int)(strlen (line)) + 1)*sizeof(char) );
		  strcpy (entry->line, line);
		  entry->prev = history;
		  entry->next = NULL;
		  if (history != NULL)
		  {
			  history->next = entry;
		  }
		  history = entry;
	  }
	  else
	  {
		  reset_history();
	  }
  }
  else
  {
	  reset_history();
  }
}
/*-----------------------------------------------------------------------------------*/
/* Backward search in history (used by key ! ) */
sci_hist * SearchBackwardInHistory(char *line)
{
	sci_hist *Parcours=NULL;
	char LineComp[MAXBUF];
	
	if (research_knot_last)
	{
		Parcours=research_knot_last;
		if (NewSearchInHistory) 
		{
			NewSearchInHistory=FALSE;
			Parcours=history;
		}
	}
	else
	{
		Parcours=history;
	}
	
	while (Parcours->prev)
	{
		int len=(int)strlen(line);
		strncpy(LineComp,Parcours->line,len);
		LineComp[len]='\0';
		
		if (strcmp(LineComp,line)==0)
		{
			if (Parcours->prev) research_knot_last=GoPrevKnot(Parcours);
			return Parcours;
		}
		Parcours=GoPrevKnot(Parcours);
	}

	research_knot_last=NULL;
	return (sci_hist *)NULL;

}
/*-----------------------------------------------------------------------------------*/
/* Forward search in history */
sci_hist * SearchForwardInHistory(char *line)
{
	sci_hist *Parcours=NULL;
	char LineComp[MAXBUF];
	
	if (research_knot_last)
	{
		Parcours=research_knot_last;
		if (NewSearchInHistory) 
		{
			NewSearchInHistory=FALSE;
			Parcours=history;
		}
	}
	else
	{
		Parcours=history;
	}
	
	while (Parcours->next)
	{
		int len=(int)strlen(line);
		strncpy(LineComp,Parcours->line,len);
		LineComp[len]='\0';
		
		if (strcmp(LineComp,line)==0)
		{
			if (Parcours->next) research_knot_last=GoNextKnot(Parcours);
			return Parcours;
		}
		Parcours=GoNextKnot(Parcours);
	}
	research_knot_last=NULL;
	return (sci_hist *)NULL;
}
/*-----------------------------------------------------------------------------------*/
sci_hist * GoFirstKnot(sci_hist * CurrentKnot)
{
	while(CurrentKnot->prev) CurrentKnot=GoPrevKnot(CurrentKnot);
	return (sci_hist *) CurrentKnot;
}
/*-----------------------------------------------------------------------------------*/
sci_hist * GoLastKnot(sci_hist * CurrentKnot)
{
	while(CurrentKnot->next) CurrentKnot=GoNextKnot(CurrentKnot);
	return (sci_hist *) CurrentKnot;
}
/*-----------------------------------------------------------------------------------*/
sci_hist * GoPrevKnot(sci_hist * CurrentKnot)
{
	CurrentKnot=CurrentKnot->prev;
	return (sci_hist *) CurrentKnot;
}
/*-----------------------------------------------------------------------------------*/
sci_hist * GoNextKnot(sci_hist * CurrentKnot)
{
	CurrentKnot=CurrentKnot->next;
	return (sci_hist *) CurrentKnot;
}
/*-----------------------------------------------------------------------------------*/
/*interface routine for Scilab function savehistory  */

int C2F(savehistory) _PARAMS((char *fname))
{
  
  char  line[MAXBUF];
  char *Path;
  int l1, m1, n1, out_n;
  
  Rhs=Max(Rhs,0);
  CheckRhs(0,1) ;
  CheckLhs(0,1) ;
	
  if (Rhs == 0)
  {
    Path=get_sci_data_strings(HISTORY_ID);
	C2F(cluni0)(Path, line, &out_n,(long)strlen(Path),MAXBUF);
	write_history (line);
  }
  else
  {
	if ( GetType(1) == 1 ) 
	{
		GetRhsVar(1,"i",&m1,&n1,&l1);
		savehistoryafterncommands(*istk(l1));
	}
	else if ( GetType(1) == 10 )
	{
		GetRhsVar(1,"c",&m1,&n1,&l1);
		Path=cstk(l1);

		C2F(cluni0)(Path, line, &out_n,(long)strlen(Path),MAXBUF);
		write_history (line);
	}
  }

  LhsVar(1)=0;
  C2F(putlhsvar)();
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* save history in filemane: */

#ifndef WITH_READLINE

void write_history(char *filename)
{
	FILE * pFile;
	sci_hist *Parcours = history;
	char  line[MAXBUF];
	BOOL SaveLine=TRUE;
	if ( history == NULL) return;
	pFile = fopen (filename,"wt");
	if (pFile)
    {
		Parcours=GoFirstKnot(Parcours);
		while(Parcours->next)
		{
			if (strncmp(Parcours->line,"// Begin Session :",strlen("// Begin Session :"))==0)
			{
					sci_hist *ParcoursNextforTest = GoNextKnot(Parcours);
					if (ParcoursNextforTest)
					{
						if (strncmp(ParcoursNextforTest->line,"// End Session :",strlen("// End Session :"))==0)
						{
							SaveLine=FALSE;
						}
			}
		}
		if (SaveLine)
		{
			fputs(Parcours->line,pFile );
			fputs("\n",pFile );
		}
		else 
		{ 
			SaveLine=TRUE;
			Parcours=GoNextKnot(Parcours);
		}

	    Parcours=GoNextKnot(Parcours);  
	  }

		if (Parcours)
		{
			fputs(Parcours->line,pFile );
			fputs("\n",pFile );
		}

		GetCommentDateSession(line,FALSE);
		fputs(line,pFile);
		fclose(pFile);
	}
}
#endif 

/*-----------------------------------------------------------------------------------*/
char * getfilenamehistory(void)
{
  char  *history_name = get_sci_data_strings(HISTORY_ID);
  char  *filename= MALLOC(MAXBUF*sizeof(char));
  int out_n;
  if ( filename == NULL ) return NULL;
  C2F(cluni0)(history_name, filename, &out_n,(long)strlen(history_name),MAXBUF);
  return filename;
}

/*-----------------------------------------------------------------------------------*/

#ifndef WITH_READLINE

void reset_history(void)
{
  if (history)
    {
      sci_hist *Parcours = history;
      sci_hist *PrevParcours=NULL;

      Parcours=GoFirstKnot(Parcours);
				
      while(Parcours->next)
	{
	  PrevParcours=Parcours;
	  FREE(Parcours->line);
	  Parcours->line=NULL;
	  Parcours=GoNextKnot(Parcours); 
	  PrevParcours->next=NULL;
	  PrevParcours->prev=NULL;
	}
	  FREE(history);
      history=NULL;
	  FREE(cur_entry);
      cur_entry=NULL;
	  HistorySizeInMemory=0;
    }

}
#else 
void reset_history(void)
{
  register HIST_ENTRY **the_list;
  register int i;
  the_list = history_list ();
  if (the_list)
    {
      int count;
      for (count = 0; the_list[count]; count++) ;
      for ( i = count ; i >= 0 ; i--)
	{
	  HIST_ENTRY *entry = remove_history (i);
	  if (entry)
	    {
	      FREE (entry->line);
	      FREE (entry);
	    }
	}
    }
  HistorySizeInMemory=0;
}

#endif 

/*-----------------------------------------------------------------------------------*/
/*interface routine for Scilab function resethistory  */
int C2F(resethistory) _PARAMS((char *fname))
{
  char Commentline[MAXBUF];	

  Rhs=Max(Rhs,0);
  CheckRhs(0,0) ;
  CheckLhs(0,1) ;
  reset_history();

  GetCommentDateSession(Commentline,TRUE);		
  AddHistory (Commentline);

  LhsVar(1)=0;
  C2F(putlhsvar)();
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/*interface routine for Scilab function loadhistory  */

#ifndef  WITH_READLINE
static void read_history(char *filename);
#endif 

int C2F(loadhistory) _PARAMS((char *fname))
{
  char  line[MAXBUF];
  char  *Path;
  int l1, m1, n1, out_n;
  Rhs=Max(Rhs,0);
  CheckRhs(0,1) ;
  CheckLhs(0,1) ;
	

  if (Rhs == 0) {
    Path=get_sci_data_strings(HISTORY_ID);
  }
  else {
    GetRhsVar(1,"c",&m1,&n1,&l1);
    Path=cstk(l1);
  }
  
  C2F(cluni0)(Path, line, &out_n,(long)strlen(Path),MAXBUF);

  read_history (line);

  LhsVar(1)=0;
  C2F(putlhsvar)();
  return 0;
}

#ifndef  WITH_READLINE

static void read_history(char *filename)
{
  char  line[MAXBUF];
  FILE * pFile;
  pFile = fopen (filename,"rt");
  if (pFile)
    {
      sci_hist *Parcours = history;
      if (Parcours) Parcours=GoLastKnot(Parcours);
	
      while(fgets (line,sizeof(line),pFile) != NULL)
	{
	  line[strlen(line)-1]='\0'; /* enleve le retour chariot */
	  AddHistory(line);
	}
		
      cur_entry=history;
      fclose(pFile);
    }
  /* Ajout date & heure debut session */

  GetCommentDateSession(line,TRUE);		
  AddHistory (line);  
}

#endif 

/*-----------------------------------------------------------------------------------*/
/*interface routine for Scilab function gethistory  */

#ifndef  WITH_READLINE

static int CreSmatFromHist(char *fname, int number, sci_hist *Parcours);

int C2F(gethistory) _PARAMS((char *fname))
{

  static int l1, m1, n1;	
  int indice=1,GotoLine;
  sci_hist *Parcours = history;


  Rhs=Max(Rhs,0);
  CheckRhs(0,1) ;
  CheckLhs(1,1) ;

  if (!history)   goto empty;

  if (Rhs == 1) {
    GetRhsVar(1,"i",&m1,&n1,&l1);
    GotoLine=Max(1,*istk(l1)); 
  }
  else {
    GotoLine=1; 
  }

  /* looking for the top of the hystory */
  if (Parcours) Parcours=GoFirstKnot(Parcours);

  /* get the first requested record */
  while  ( Parcours->next ) {	
    if ( indice == GotoLine ) break;
    Parcours=GoNextKnot(Parcours);
    indice++;
  }
  if (!Parcours->next) goto empty;

  if(!CreSmatFromHist(fname, Rhs+1, Parcours)) return 0;
  LhsVar(1)=Rhs+1;
  C2F(putlhsvar)();
  return 0;

 empty:
  m1=0;
  n1=0;
  CreateVar(Rhs+1,"d",  &m1, &n1, &l1);
  LhsVar(1)=Rhs+1;
  C2F(putlhsvar)();
  return 0;
}	
 
/*-----------------------------------------------------------------------------------*/
static int CreSmatFromHist(char *fname, int number, sci_hist *Parcours)
{
  int ix1, il, nnchar, kij, ilp, lw;
  int pos;
  int indice;
  static int  cx0 = 0;
  sci_hist *Htop;

  Nbvars = Max(number,Nbvars);
  lw = number + Top - Rhs;

  /* preserve the top of history adress */
  Htop=Parcours;

  /* get the number of history lines and total number of characters */
  indice=0;nnchar = 0;
  while  ( Parcours->next ) {	
    indice++;
    nnchar =nnchar + (int)strlen(Parcours->line);
    Parcours=GoNextKnot(Parcours);
  }
  
  /* Check for available memory */
  il = iadr(*Lstk(lw));
  ix1 = il + 4 + (nnchar + 1) + (indice + 1);
  Err = sadr(ix1) - *Lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",
	     fname);
    return  FALSE_;
  } ;
  /* create the variable header */
  *istk(il ) = 10;
  *istk(il + 1) = indice;
  *istk(il + 2) = 1;
  *istk(il + 3) = 0;
  ilp = il + 4;
  *istk(ilp ) = 1;
  pos = ilp + indice + 1;

  /* fill in the variable */
  Parcours=Htop;
  kij = ilp + 1;
  while  ( Parcours->next ) {
    int l = (int)strlen(Parcours->line);
     *istk(kij ) = *istk(kij - 1) + l;
     C2F(cvstr)(&l, istk(pos), Parcours->line, &cx0, l);
    Parcours=GoNextKnot(Parcours);
    kij++;
    pos = pos + l;
  }
  /* close the variable */
  *Lstk(lw+1) = sadr(pos);

  C2F(intersci).iwhere[number - 1] = *Lstk(lw);
  C2F(intersci).ntypes[number - 1] = '$';
  return TRUE_;
} 

#else 

/* readline version */

static int CreSmatFromHist(char *fname, int number, int from_line, int count);

int C2F(gethistory) _PARAMS((char *fname))
{
  register HIST_ENTRY **the_list;
  int l1, m1, n1, GotoLine,count;

  Rhs=Max(Rhs,0);
  CheckRhs(0,1) ;
  CheckLhs(1,1) ;

  if (Rhs == 1) {
    GetRhsVar(1,"i",&m1,&n1,&l1);
    GotoLine=Max(1,*istk(l1)); 
  }
  else {
    GotoLine=1; 
  }

  the_list = history_list ();
  for (count = 0; the_list[count]; count++) ;
  if ( GotoLine -1 >= count ) goto empty;

  if(!CreSmatFromHist(fname, Rhs+1,GotoLine,count)) return 0;
  LhsVar(1)=Rhs+1;
  C2F(putlhsvar)();
  return 0;

 empty:
  m1=0;
  n1=0;
  CreateVar(Rhs+1,"d",  &m1, &n1, &l1);
  LhsVar(1)=Rhs+1;
  C2F(putlhsvar)();
  return 0;
}	
 

static int CreSmatFromHist(char *fname, int number, int from_line, int count)
{
  int ix1, il, nnchar, kij, ilp, lw, pos, indice,i;
  static int  cx0 = 0;
  register HIST_ENTRY **the_list;

  Nbvars = Max(number,Nbvars);
  lw = number + Top - Rhs;

  the_list = history_list ();

  /* get the number of history lines and total number of characters */

  indice=0;nnchar = 0;
  if ( from_line -1 >= count ) return 0;
  for ( i = Max(from_line -1,0) ; i < count  ; i++)
    {
      nnchar += strlen(the_list[i]->line);
      indice++;
    }
  
  /* Check for available memory */

  il = iadr(*Lstk(lw));
  ix1 = il + 4 + (nnchar + 1) + (indice + 1);
  Err = sadr(ix1) - *Lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",
	     fname);
    return FALSE_;
  } ;
  /* create the variable header */
  *istk(il ) = 10;
  *istk(il + 1) = indice;
  *istk(il + 2) = 1;
  *istk(il + 3) = 0;
  ilp = il + 4;
  *istk(ilp ) = 1;
  pos = ilp + indice + 1;

  /* fill in the variable */

  kij = ilp + 1;
  for ( i = Max(from_line -1,0) ; i < count  ; i++)
    {
      int l = strlen(the_list[i]->line);
      *istk(kij ) = *istk(kij - 1) + l;
      C2F(cvstr)(&l, istk(pos), the_list[i]->line, &cx0, l);
      kij++;
      pos = pos + l;
    }
  /* close the variable */
  *Lstk(lw+1) = sadr(pos);

  C2F(intersci).iwhere[number - 1] = *Lstk(lw);
  C2F(intersci).ntypes[number - 1] = '$';
  return TRUE_;
} 

#endif 

