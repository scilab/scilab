#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <conio.h>
#include <string.h>
#include <time.h>

/* Allan CORNET 2004 */
/* Scilab INRIA */

#ifndef STRICT
	#define STRICT
#endif
	
#include "wcommon.h"

/*-----------------------------------------------------------------------------------*/
#define HistoryFileName "history.scilab"
#define MAXBUF	1024
/*-----------------------------------------------------------------------------------*/
extern BOOL IsWindowInterface(void);
extern write_scilab_synchro(char *line);
/*-----------------------------------------------------------------------------------*/
struct hist
{
    		char *line;
    		struct hist *prev;
    		struct hist *next;
};
/*-----------------------------------------------------------------------------------*/
struct hist *history = NULL;	/* no history yet */
struct hist *cur_entry = NULL;

static struct hist *research_knot_last = NULL;
BOOL NewSearchInHistory=FALSE;
/*-----------------------------------------------------------------------------------*/
void GetCommentDateSession(char *line,BOOL BeginSession);
void SaveHistoryFunction(void);
void add_history (char *line);
void ResetHistoryFunction(void);
void LoadHistoryFunction(void);
void HistoryFunction _PARAMS((char *fname));
void SearchInHistory(char *line);

struct hist * GoFirstKnot(struct hist * CurrentKnot);
struct hist * GoLastKnot(struct hist * CurrentKnot);
struct hist * GoPrevKnot(struct hist * CurrentKnot);
struct hist * GoNextKnot(struct hist * CurrentKnot);
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
void GetCommentDateSession(char *line,BOOL BeginSession)
{
	char dbuffer [9];
	char tbuffer [9];
	if (BeginSession)
	wsprintf(line,"// Begin Session : %s %s ",_strdate( dbuffer ),_strtime( tbuffer ));
	else
	wsprintf(line,"// End Session : %s %s ",_strdate( dbuffer ),_strtime( tbuffer ));
}
/*-----------------------------------------------------------------------------------*/
void SaveHistoryFunction(void)
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
}

/*-----------------------------------------------------------------------------------*/
/* add line to the history at the end of history*/
void add_history (char *line)

{
  struct hist *entry;
  
  if (history)
  {
  	if  (strcmp(history->line,line) == 0)
  	{
  		return;
  	}
  	
  }
  
  entry = (struct hist *) alloc ((unsigned long) sizeof (struct hist), "history");
  entry->line = alloc ((unsigned long) (strlen (line) + 1), "history");
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
void ResetHistoryFunction(void)
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
		add_history (Commentline);
	}
}
/*-----------------------------------------------------------------------------------*/
void LoadHistoryFunction(void)
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
				add_history(line);
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
		add_history (Commentline);  	
	}
}
/*-----------------------------------------------------------------------------------*/
void HistoryFunction _PARAMS((char *fname))
{
  static int l1, m1, n1;	
  int indice=0;
  struct hist *Parcours = history;
  
  if (IsWindowInterface())
  {
  
  
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
  			
  			write_scilab_synchro(Parcours->line);
  			
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
 }	
 else
 {
 	sciprint("Not in Console mode\n");
 }
}
/*-----------------------------------------------------------------------------------*/
void SearchInHistory(char *line)
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
	
	while (Parcours->prev)
	{
		int len=strlen(line);
		strncpy(LineComp,Parcours->line,len);
		LineComp[len]='\0';
		
		if (strcmp(LineComp,line)==0)
		{
			if (Parcours->prev) research_knot_last=GoPrevKnot(Parcours);
			write_scilab_synchro(Parcours->line);
			return;
		}
		Parcours=GoPrevKnot(Parcours);
	}

	research_knot_last=NULL;
	
}
/*-----------------------------------------------------------------------------------*/
