#include "history.h"
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
void add_history_sci (char *line)
{
  struct hist *entry;
  
  if (history)
  {
  	if  (strcmp(history->line,line) == 0)
  	{
  		return;
  	}
  	
  }
  
  /*  entry = (struct hist *) alloc ((unsigned long) sizeof (struct hist), "history");
      entry->line = alloc ((unsigned long) (strlen (line) + 1), "history"); SS */
  entry = (struct hist *) malloc ((unsigned long) sizeof (struct hist));
  entry->line = malloc ((unsigned long) (strlen (line) + 1)); 
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
struct hist * SearchBackwardInHistory(char *line)
/* Effectue la recherche via ! dans l'historique*/
{

	struct hist *Parcours=NULL;
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
	
	while (Parcours)
	{
		int len=strlen(line);
		strncpy(LineComp,Parcours->line,len);
		LineComp[len]='\0';
		
		if (strcmp(LineComp,line)==0)
		{
			if (Parcours->prev) research_knot_last=GoPrevKnot(Parcours);
			/*write_scilab(Parcours->line);*/
			return Parcours;
		}
		Parcours=GoPrevKnot(Parcours);
	}

	research_knot_last=NULL;
	return (struct hist *)NULL;

}
struct hist * SearchForwardInHistory(char *line)
/* Effectue la recherche via ! dans l'historique*/
{

	struct hist *Parcours=NULL;
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
	
	while (Parcours)
	{
		int len=strlen(line);
		strncpy(LineComp,Parcours->line,len);
		LineComp[len]='\0';
		
		if (strcmp(LineComp,line)==0)
		{
			if (Parcours->next) research_knot_last=GoNextKnot(Parcours);
			/*write_scilab(Parcours->line);*/
			return Parcours;
		}
		Parcours=GoNextKnot(Parcours);
	}

	research_knot_last=NULL;
	return (struct hist *)NULL;

}

/*-----------------------------------------------------------------------------------*/
struct hist * GoFirstKnot(struct hist * CurrentKnot)
{
	while(CurrentKnot->prev) CurrentKnot=GoPrevKnot(CurrentKnot);
	return (struct hist *) CurrentKnot;
}
/*-----------------------------------------------------------------------------------*/
struct hist * GoLastKnot(struct hist * CurrentKnot)
{
	while(CurrentKnot->next) CurrentKnot=GoNextKnot(CurrentKnot);
	return (struct hist *) CurrentKnot;
}

/*-----------------------------------------------------------------------------------*/
struct hist * GoPrevKnot(struct hist * CurrentKnot)
{
	CurrentKnot=CurrentKnot->prev;
	return (struct hist *) CurrentKnot;
}
/*-----------------------------------------------------------------------------------*/
struct hist * GoNextKnot(struct hist * CurrentKnot)
{
	CurrentKnot=CurrentKnot->next;
	return (struct hist *) CurrentKnot;
}
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
/*interface routine for Scilab function savehistory  */
int C2F(savehistory) _PARAMS((char *fname))
{
	FILE * pFile;
  	char *Home;
  	char *HistoryFileNamePath;
	struct hist *Parcours = history;
	
	Home = getenv ("HOME");
	if (Home)
	{
	
	HistoryFileNamePath=(char*)malloc( (strlen(Home)+2+strlen(HistoryFileName) )*sizeof(char)) ;
	
	strcpy(HistoryFileNamePath,Home);
  	strcat(HistoryFileNamePath,"/");
  	strcat(HistoryFileNamePath,HistoryFileName);

  	if (history)
  		{
  		pFile = fopen (HistoryFileNamePath,"wt");
  		if (pFile)
  			{
  			char Commentline[MAXBUF];
  			
  			Parcours=GoFirstKnot(Parcours);
  			
  			while(Parcours->next)
  				{
  				fputs(Parcours->line,pFile );
  				fputs("\n",pFile );
  				Parcours=GoNextKnot(Parcours);  
  				}
  			
  			GetCommentDateSession(Commentline,FALSE);
  			
			fputs(Commentline,pFile );
  			
  			fclose(pFile);
  			}
  		
        	free(HistoryFileNamePath);	
   		}
	}
	else
	{
		sciprint("\nHOME variable error \n");
	}
	C2F(objvide)(fname,&(Top),strlen(fname));
	return 0;
}

/*-----------------------------------------------------------------------------------*/
/*interface routine for Scilab function resethistory  */
int C2F(resethistory) _PARAMS((char *fname))
{
	if (history)
	{
		struct hist *Parcours = history;
		struct hist *PrevParcours=NULL;
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
		add_history_sci (Commentline);
	}
	C2F(objvide)(fname,&(Top),strlen(fname));
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/*interface routine for Scilab function loadhistory  */
int C2F(loadhistory) _PARAMS((char *fname))
{
	FILE * pFile;
	char  line[MAXBUF];
	char *Home;
	char *HistoryFileNamePath;
		
	Home = getenv ("HOME");
	if (Home)
	{
	HistoryFileNamePath=(char*)malloc( (strlen(Home)+2+strlen(HistoryFileName) )*sizeof(char)) ;
	
	strcpy(HistoryFileNamePath,Home);
	strcat(HistoryFileNamePath,"/");
	strcat(HistoryFileNamePath,HistoryFileName);
	
  	pFile = fopen (HistoryFileNamePath,"rt");
  	if (pFile)
  		{
  		struct hist *Parcours = history;
  		if (Parcours) Parcours=GoLastKnot(Parcours);
  			
  		while(fgets (line,sizeof(line),pFile) != NULL)
			{
				line[strlen(line)-1]='\0'; /* enleve le retour chariot */
				add_history_sci(line);
			}
		
		cur_entry=history;
		fclose(pFile);
  		}

  	free(HistoryFileNamePath);
	}
	else
	{
		sciprint("\nHOME variable error \n");
	}
	// Ajout date & heure debut session
	{
		char Commentline[MAXBUF];
		GetCommentDateSession(Commentline,TRUE);		
		add_history_sci (Commentline);  	
	}
	C2F(objvide)(fname,&(Top),strlen(fname));
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/*interface routine for Scilab function gethistory  */
int C2F(gethistory) _PARAMS((char *fname))
{

  static int l1, m1, n1;	
  int indice=0;
  struct hist *Parcours = history;
  
   if (Rhs == 0) /* aucun parametre --> affichage de la liste de l'historique */
    {
    	if (history)
        {
        	
  		if (Parcours) Parcours=GoFirstKnot(Parcours);
  				
  		/* Parcours la liste jusqu'au dernier element */
  		while(Parcours->next)
  		{
  			
  			sciprint("%d : %s\n",indice,Parcours->line);
  			Parcours=GoNextKnot(Parcours);
  			indice++;
  		}
  	
     	}	
  	else
    	{
  		sciprint("No history\n");
    	}
     }
  else
  {
  	/* Affichage d'une ligne en particulier */
  	int GotoLine=0;	
        int IndiceMax=0;	
        
        if (history)
        {
        	if (Parcours) Parcours=GoFirstKnot(Parcours);
        	
        	while(Parcours->next)
  		{
  			Parcours=GoNextKnot(Parcours);
  			IndiceMax++;
  		}
  		
  		if (Parcours) Parcours=GoFirstKnot(Parcours);
  		
  		CheckRhs(1,1);
  		CheckLhs(1,1);
  		GetRhsVar(1,"i",&m1,&n1,&l1);
  		GotoLine=*istk(l1);
  		LhsVar(1)=0;
  	
  		if ( (GotoLine>=0) && (GotoLine<=IndiceMax) )
  		{
  			while  ( Parcours->next )
  			{	
  				if ( indice == GotoLine ) break;
  				Parcours=GoNextKnot(Parcours);
  				indice++;
  			}
  			
  			write_scilab(Parcours->line);
  			
  		}
  		else
  		{
  			sciprint("Error with param. %d not in [0,%d]\n",GotoLine,IndiceMax);
  		}
        }
        else
        {
  		sciprint("No history\n");
    	}
   	
  }
   C2F(objvide)(fname,&(Top),strlen(fname));
   return 0;
 }	
 
