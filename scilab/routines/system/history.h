#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#ifndef	WIN32
#include "../stack-c.h"
#endif

/* Allan CORNET 2004 */
/* Scilab INRIA */

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
#define HistoryFileName "history.scilab"
#define MAXBUF	1024
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

/* Use for SearchInHistory --> ! */
struct hist *research_knot_last = NULL;
BOOL NewSearchInHistory=FALSE; /* rlgets wsci\command.c */
/*-----------------------------------------------------------------------------------*/
char *ASCIItime(const struct tm *timeptr);
void GetCommentDateSession(char *line,int BeginSession);
void add_history_sci (char *line);
struct hist * SearchBackwardInHistory(char *line); /* Effectue la recherche via ! dans l'historique*/
struct hist * SearchForwardInHistory(char *line); /* Effectue la recherche via ! dans l'historique*/
/*-----------------------------------------------------------------------------------*/
struct hist * GoFirstKnot(struct hist * CurrentKnot);
struct hist * GoLastKnot(struct hist * CurrentKnot);
struct hist * GoPrevKnot(struct hist * CurrentKnot);
struct hist * GoNextKnot(struct hist * CurrentKnot);

/*-----------------------------------------------------------------------------------*/
int C2F(resethistory) _PARAMS((char *fname));
int C2F(loadhistory) _PARAMS((char *fname));
int C2F(savehistory) _PARAMS((char *fname));
int C2F(gethistory) _PARAMS((char *fname)); /* Affiche le contenu de l'historique */

/*-----------------------------------------------------------------------------------*/
