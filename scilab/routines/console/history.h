#ifndef _SCI_HISTORY 
#define _SCI_HISTORY 

/* Allan CORNET 2004 */
/* Scilab INRIA */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#ifndef	WIN32
#include "../stack-c.h"
#endif

#ifndef STRICT
  #define STRICT
#endif

#ifdef	WIN32
  #include "../wsci/wcommon.h" /* for BOOL */
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
/*-----------------------------------------------------------------------------------*/
char *ASCIItime(const struct tm *timeptr);
void GetCommentDateSession(char *line,int BeginSession);
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
void reset_history(void);
int C2F(loadhistory) _PARAMS((char *fname));
int C2F(savehistory) _PARAMS((char *fname));
int C2F(gethistory) _PARAMS((char *fname)); /* Affiche le contenu de l'historique */
int CreSmatFromHist(char *fname, int number, sci_hist *Parcours);
/*-----------------------------------------------------------------------------------*/

#endif 
