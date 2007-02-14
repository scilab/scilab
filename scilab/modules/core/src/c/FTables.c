/**************************************
 * Copyright Jean-Philippe Chancelier 
 * ENPC 
 **************************************/

#include <string.h>
#include "FTables0.h"
#include "FTables.h"
#define FTable_H   /* to prevent  a type conflict with GetFuncPtr */ 
#include "stack-c.h"
#include "stack2.h"
#include "sciprint.h"
#include "link.h"


static int SearchComp  __PARAMS((FTAB *Ftab, char *op, void (**realop) ( )));  
static void Emptyfunc  __PARAMS((void)) {} ;

voidf GetFuncPtr(char *name, int n, FTAB *Table, voidf scifun, int *ifunc, int *lhs, int *rhs);
voidf SetFunction  __PARAMS((char *name, int *rep, FTAB *table));  


/***********************************
 * Search Table for fbutn
 **********************************/

/** the current function fixed by setfbutn **/

static fbutnf fbutnfonc ;

/** function call **/

void C2F(fbutn)(char *name, integer *win, integer *entry)
{
  (*fbutnfonc)(name,win,entry);
}

/** fixes the function associated to name **/

void C2F(setfbutn)(char *name, int *rep)
{
  fbutnfonc = (fbutnf) SetFunction(name,rep,FTab_fbutn);
}


/***********************************
 * Search Table for interf 
 **********************************/

/** the current function fixed by setinterf **/

static interff interffonc ;

/** function call **/

void C2F(interf)(void * x1, void * x2, void * x3, void * x4, void * x5, void * x6, void * x7, void * x8, void * x9, void * x10, void * x11, void * x12, void * x13, void * x14, void * x15, void * x16, void * x17, void * x18, void * x19, void * x20, void * x21, void * x22, void * x23, void * x24, void * x25, void * x26, void * x27, void * x28, void * x29, void * x30)
{
  (*interffonc)(x1 ,x2 ,x3 ,x4 ,x5 ,x6 ,x7 ,x8 ,x9 ,x10,
	       x11,x12,x13,x14,x15,x16,x17,x18,x19,x20,
	       x21,x22,x23,x24,x25,x26,x27,x28,x29,x30);
}


void C2F(interf1)(char *name, long int size)
{
  ((interff1) *interffonc)(name,strlen(name));
}

/** fixes the function associated to name **/

void C2F(setinterf)(char *name, int *rep)
{
  interffonc = (interff) SetFunction(name,rep,FTab_interf);
}

/** function call **/


/*******************************************
 * General functions 
 *******************************************/

voidf SetFunction(char *name, int *rep, FTAB *table)
{
  void (*loc)();
  char *s;
  strncpy(buf,name,MAXNAME);
  s=buf ; while ( *s != ' ' && *s != '\0') { s++;};
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
	  Scierror(999,"%s: external %s not found \r\n",name,cstk(*ifunc));
          return (voidf) 0;
        }  
      return f ;
    case  a_function :
      GetRhsVar(n, "f", &nlhs,&nrhs, ifunc);
      /*
      if ( nlhs != *lhs || nrhs != *rhs ) 
	{
	  Scierror(999,"%s: given external has wrong (lhs,rhs) values (%d,%d), (%d,%d) required\r\n",name,
		   nlhs,nrhs,*lhs,*rhs);
	  return (voidf) 0 ;
	}
      */
      return (voidf) scifun ;
    default:
      sciprint("Wrong parameter in %s ! (number %d)\r\n",name,n);
      Error(999);
      return (voidf) 0 ;
    }
}
