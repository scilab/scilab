/*------------------------------------------------------------------------
 *    Copyright (C) Inria 2004
 *--------------------------------------------------------------------------*/

#include "history.h"
#include "MALLOC.h" 
#include "stack-c.h"
#include "error_scilab.h"

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

static int SaveHistoryAfterNcommands=0;
extern int getNumberOfCommands(void);
extern void resetNumberOfCommands(void);
/*-----------------------------------------------------------------------------------*/
static char *ASCIItime(const struct tm *timeptr)
{
    static char wday_name[7][4] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };
    static char mon_name[12][4] = {
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

/*-----------------------------------------------------------------------------------*/
/* save history in filemane: */
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
/*interface routine for Scilab function resethistory  */
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
/*-----------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------*/
/*interface routine for Scilab function loadhistory  */
static void read_history(char *filename);


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


/*-----------------------------------------------------------------------------------*/
/*interface routine for Scilab function gethistory  */

int CreSmatFromHist(char *fname, int number, sci_hist *Parcours);

/*-----------------------------------------------------------------------------------*/
int CreSmatFromHist(char *fname, int number, sci_hist *Parcours)
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
  if (Err > 0) 
  {
    error_scilab(17,"core_error_17",fname);
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
/*-----------------------------------------------------------------------------------*/
int savehistoryafterncommands(int N)
{
	int SaveHistoryAfterNcommandsTemp=0;

	SaveHistoryAfterNcommandsTemp=N;

	if (SaveHistoryAfterNcommandsTemp>=0)
	{
		SaveHistoryAfterNcommands=SaveHistoryAfterNcommandsTemp;
		resetNumberOfCommands();
	}

	return 0;	
}
/*-----------------------------------------------------------------------------------*/
int GetSaveHistoryAfterNcommands(void)
{
	return SaveHistoryAfterNcommands;
}
/*-----------------------------------------------------------------------------------*/
void setNewSearchInHistory(int state)
{
  NewSearchInHistory = (BOOL) state;
}
/*-----------------------------------------------------------------------------------*/
