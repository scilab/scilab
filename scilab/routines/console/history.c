#include "history.h"

sci_hist *history = NULL;	/* no history yet */
sci_hist *cur_entry = NULL;
/* Use for SearchInHistory --> ! */
sci_hist *research_knot_last = NULL;
BOOL NewSearchInHistory=FALSE; /* rlgets wsci\command.c */
static char *HistoryFileNamePath = "~/history.scilab";

#include  "../stack-c.h"
#ifndef Max 
#define Max(x,y)	(((x)>(y))?(x):(y))
#endif 

extern int C2F(cluni0) __PARAMS((char *name, char *nams, integer *ln, long int name_len,
			        long int nams_len));  

/*-----------------------------------------------------------------------------------*/
char *ASCIItime(const struct tm *timeptr)
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
/* add line to the history at the end of history*/
void AddHistory (char *line)
{
  sci_hist *entry;
  
  if (history)
  {
  	if  (strcmp(history->line,line) == 0)
  	{
  		return;
  	}
  	
  }
 
  entry = (sci_hist *) malloc ((unsigned long) sizeof (sci_hist));
  entry->line = (char *) malloc ((strlen (line) + 1)*sizeof(char));  
  strcpy (entry->line, line);

  entry->prev = history;
  entry->next = NULL;
  if (history != NULL)
    {
      history->next = entry;
    }
  history = entry;
  
}
/*-----------------------------------------------------------------------------------*/
sci_hist * SearchBackwardInHistory(char *line)
/* Effectue la recherche via ! dans l'historique*/
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
sci_hist * SearchForwardInHistory(char *line)
/* Effectue la recherche via ! dans l'historique*/
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
  FILE * pFile;
  char  line[MAXBUF];
  char *Path;
  int l1, m1, n1, out_n, lout;
  sci_hist *Parcours = history;

  Rhs=Max(Rhs,0);
  CheckRhs(0,1) ;
  CheckLhs(0,1) ;

	
  if (Rhs == 0) {
    Path=HistoryFileNamePath;
  }
  else {
    GetRhsVar(1,"c",&m1,&n1,&l1);
    Path=cstk(l1);
  }
  if (history) {
    lout=MAXBUF;
    C2F(cluni0)(Path, line, &out_n,(long)strlen(Path),lout);
    pFile = fopen (line,"wt");
    if (pFile)
      {
	Parcours=GoFirstKnot(Parcours);
	while(Parcours->next)
	  {
	    fputs(Parcours->line,pFile );
	    fputs("\n",pFile );
	    Parcours=GoNextKnot(Parcours);  
	  }

	if (Parcours)
	 {
		fputs(Parcours->line,pFile );
		fputs("\n",pFile );
	 }

	GetCommentDateSession(line,FALSE);
	fputs(line,pFile );
	fclose(pFile);
      }
  }
  LhsVar(1)=0;
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/*interface routine for Scilab function resethistory  */
int C2F(resethistory) _PARAMS((char *fname))
{
  Rhs=Max(Rhs,0);
  CheckRhs(0,0) ;
  CheckLhs(0,1) ;
  reset_history();
  LhsVar(1)=0;
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/
void reset_history(void)
{
  if (history)
    {
      sci_hist *Parcours = history;
      sci_hist *PrevParcours=NULL;
      char Commentline[MAXBUF];

      Parcours=GoFirstKnot(Parcours);
				
      while(Parcours->next)
	{
	  PrevParcours=Parcours;
	  free(Parcours->line);
	  Parcours->line=NULL;
	  Parcours=GoNextKnot(Parcours); 
	  PrevParcours->next=NULL;
	  PrevParcours->prev=NULL;
	}
      history=NULL;
      cur_entry=NULL;

      GetCommentDateSession(Commentline,TRUE);		
      AddHistory (Commentline);
    }

}
/*-----------------------------------------------------------------------------------*/
/*interface routine for Scilab function loadhistory  */
int C2F(loadhistory) _PARAMS((char *fname))
{
  FILE * pFile;
  char  line[MAXBUF];
  char  *Path;
  int l1, m1, n1, out_n, lout;
  Rhs=Max(Rhs,0);
  CheckRhs(0,1) ;
  CheckLhs(0,1) ;
	

  if (Rhs == 0) {
    Path=HistoryFileNamePath;
  }
  else {
    GetRhsVar(1,"c",&m1,&n1,&l1);
    Path=cstk(l1);
  }

  lout=MAXBUF;
  C2F(cluni0)(Path, line, &out_n,(long)strlen(Path),lout);
  pFile = fopen (line,"rt");

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
	
  LhsVar(1)=0;
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/*interface routine for Scilab function gethistory  */
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
  il = iadr(*lstk(lw));
  ix1 = il + 4 + (nnchar + 1) + (indice + 1);
  Err = sadr(ix1) - *lstk(Bot );
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
  *lstk(lw+1) = sadr(pos);

  C2F(intersci).iwhere[number - 1] = *lstk(lw);
  C2F(intersci).ntypes[number - 1] = '$';
  return TRUE_;
} 
