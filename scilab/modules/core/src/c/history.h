#ifndef _SCI_HISTORY 
#define _SCI_HISTORY 

/* Allan CORNET 2004 */
/* Scilab INRIA */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#ifndef	_MSC_VER
#include "../stack-c.h"
#endif

#ifdef  WITH_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#else 
extern void write_history(char *filename); 
#endif 
extern void reset_history(void);

#ifndef STRICT
  #define STRICT
#endif


#ifdef	_MSC_VER
  #include "wcommon.h" /* for BOOL */
#else
  typedef int BOOL;
  extern char *getenv();
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

extern void write_scilab  __PARAMS((char *s));
/*-----------------------------------------------------------------------------------*/
#define MAXBUF	1024
#define HISTORY_ID 3
/*-----------------------------------------------------------------------------------*/
typedef struct hist_
{
  char *line;
  struct hist_ *prev;
  struct hist_ *next;
} sci_hist;

/*-----------------------------------------------------------------------------------*/
extern sci_hist *history ;	/* no history yet */
extern sci_hist *cur_entry;
/* Use for SearchInHistory --> ! */
extern sci_hist *research_knot_last;
extern BOOL NewSearchInHistory; /* rlgets wsci\command.c */
extern int savehistoryafterncommands(int N);
/*-----------------------------------------------------------------------------------*/
/* char *ASCIItime(const struct tm *timeptr); */
/* void GetCommentDateSession(char *line,int BeginSession); */
void AddHistory (char *line);
sci_hist * SearchBackwardInHistory(char *line); /* Effectue la recherche via ! dans l'historique*/
sci_hist * SearchForwardInHistory(char *line); /* Effectue la recherche via ! dans l'historique*/
/*-----------------------------------------------------------------------------------*/
sci_hist * GoFirstKnot(sci_hist * CurrentKnot);
sci_hist * GoLastKnot(sci_hist * CurrentKnot);
sci_hist * GoPrevKnot(sci_hist * CurrentKnot);
sci_hist * GoNextKnot(sci_hist * CurrentKnot);

/*-----------------------------------------------------------------------------------*/
int C2F(resethistory) _PARAMS((char *fname));
int C2F(loadhistory) _PARAMS((char *fname));
int C2F(savehistory) _PARAMS((char *fname));
char * getfilenamehistory(void);
int C2F(gethistory) _PARAMS((char *fname)); /* Affiche le contenu de l'historique */
/*-----------------------------------------------------------------------------------*/
void GetCommentDateSession(char *line,int BeginSession);
extern char *get_sci_data_strings(int n);
extern int C2F(cluni0) __PARAMS((char *name, char *nams, integer *ln, long int name_len,long int nams_len));  
#ifndef  WITH_READLINE
int CreSmatFromHist(char *fname, int number, sci_hist *Parcours);
#endif
#endif 
