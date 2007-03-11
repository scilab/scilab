/**************************************
 * Copyright Jean-Philippe Chancelier 
 * ENPC 
 **************************************/

#include <string.h>
#include "error_scilab.h"
#include "FTables.h"
#define FTable_H   /* to prevent  a type conflict with GetFuncPtr */ 
#include "stack-c.h"
#include "stack2.h"
#include "sciprint.h"
#include "link.h"

static char buf[MAXNAME];

static int SearchComp  __PARAMS((FTAB *Ftab, char *op, void (**realop) ( )));
static void Emptyfunc  __PARAMS((void)) {}

voidf GetFuncPtr(char *name, int n, FTAB *Table, voidf scifun, int *ifunc, int *lhs, int *rhs);
voidf SetFunction  __PARAMS((char *name, int *rep, FTAB *table));  

/*******************************************
 * General functions 
 *******************************************/

voidf SetFunction(char *name, int *rep, FTAB *table)
{
  void (*loc)();
  char *s;

  strncpy(buf,name,MAXNAME);
  s=buf;
  while ( *s != ' ' && *s != '\0') { s++;};
  *s= '\0';
  if ( SearchComp(table,buf,&loc) == OK) 
    {
      *rep = 0;
      return(loc);
    }
  if ( SearchInDynLinks(buf,&loc) >= 0 )
    {
      *rep = 0;
      return(loc);
    }
  loc = Emptyfunc;
  *rep = 1;
  /* sciprint(" Function %s not found\r\n",name); */
  return(loc);
}



/*******************************************
 * Attention trier la table 
 * cherche un operateur dans une table : 
 * a ameliorer en utilisant bsearch 
 *******************************************/

static int SearchComp(FTAB *Ftab, char *op, void (**realop) (/* ??? */))
{
  int i=0;
  while ( Ftab[i].name != (char *) 0) 
     {
       int j;
       j = strcmp(op,Ftab[i].name);
       if ( j == 0 )
         {
           *realop = Ftab[i].f;
	   return(OK);
	 }
       else
         { 
	   if ( j <= 0)
             {
               /* sciprint("\nUnknow function <%s>\r\n",op); */
               return(FAIL);
             }
	   else i++;
         }
     }
  /* sciprint("\n Unknow function <%s>\r\n",op); */
  return(FAIL);
}


/*********************************************************
 * General function to get an external 
 * when writting an interface 
 * In 
 *   name : name of calling function for error message 
 *   n    : position of external in argument list 
 *   Table: table of stored function 
 *   scifun: a function 
 *   lhs,rhs : expected lhs,rhs values of the external 
 * 
 * if external argument is a 'string' : an associated function 
 * is searched in Table and in dynamically linked functions 
 * if found the function is returned 
 *
 * if external argument is a macro (lhs,rhs) are checked 
 * and scifun is returned 
 * 
 *********************************************************/

#define a_chain 10
#define a_function 13

voidf GetFuncPtr(char *name, int n, FTAB *Table, voidf scifun, int *ifunc, int *lhs, int *rhs)
{
  int type,rep,mm,nn,nlhs,nrhs;
  voidf f;
  type=C2F(vartype)(&n);
  switch ( type)
    {
    case a_chain :
      GetRhsVar(n, "c", &mm, &nn, ifunc);
      f = SetFunction(cstk(*ifunc),&rep,Table);
      if ( rep == 1 )
        {
		  error_scilab(999,"core_error_146",name,cstk(*ifunc));
          return (voidf) 0;
        }  
      return f ;
    case  a_function :
      GetRhsVar(n, "f", &nlhs,&nrhs, ifunc);
      return (voidf) scifun ;
    default:
      sciprint("Wrong parameter in %s ! (number %d)\r\n",name,n);
      Error(999);
      return (voidf) 0 ;
    }
}
