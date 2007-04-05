/**************************************
 * Copyright Jean-Philippe Chancelier 
 * ENPC 
 **************************************/
#include <string.h>
#include "FTables.h"
#include "stack-c.h"
#include "stack2.h"
#include "link.h"

#define MAXNAME 32
static char buf[MAXNAME];

static BOOL SearchComp  __PARAMS((FTAB *Ftab, char *op, void (**realop) ( )));
static void Emptyfunc  __PARAMS((void)) {}

/*******************************************
 * General functions 
 *******************************************/

voidf SetFunction(char *name, int *rep, FTAB *table)
{
  void (*loc)();
  char *s=NULL;

  strncpy(buf,name,MAXNAME);
  s=buf;
  while ( *s != ' ' && *s != '\0') { s++;};
  *s= '\0';

  if ( ( SearchComp(table,buf,&loc) == TRUE) || ( SearchInDynLinks(buf,&loc) >= 0 ) )
  {
    *rep = 0;
  }
  else
  {
	loc = Emptyfunc;
	*rep = 1;
  }
  return(loc);
}
/*******************************************
 * Attention trier la table 
 * cherche un operateur dans une table : 
 * a ameliorer en utilisant bsearch 
 *******************************************/
static BOOL SearchComp(FTAB *Ftab, char *op, void (**realop) (/* ??? */))
{
  int i=0;
  while ( Ftab[i].name != (char *) 0) 
  {
     int j=0;
     j = strcmp(op,Ftab[i].name);
     if ( j == 0 )
     {
       *realop = Ftab[i].f;
	   return TRUE;
	 }
     else
     { 
	   if ( j <= 0) return FALSE;
	   else i++;
     }
  }
  return FALSE;
}
