/*-----------------------------------------------------------------------------------*/
/* Scilab function table */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashtable_core.h"
#include "stack-c.h"
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
/** maximum number of entries in the htable **/
/** in fact create_hashtable_scilab_functions used a prime > MAXTAB **/
/** WARNING : MAXTAB must be chosen > 2* the number of entries in fundef **/
/** for good efficiency of the hash code **/
#define MAXTAB 1536
/*-----------------------------------------------------------------------------------*/
typedef struct 
{
	char *name;
	int codeI;
	int code;
	int level;
} Funcs ;
/*-----------------------------------------------------------------------------------*/
Funcs SciFuncs[]={
#include "../../sci_gateway/fundef"
	{(char*) 0 ,  0,  0  ,   1},
};
/*-----------------------------------------------------------------------------------*/
extern void sciprint __PARAMS((char *fmt,...));
/*-----------------------------------------------------------------------------------*/
/************************************************************
 *    Hash table for scilab functions 
 *    job is used to specify a job
 *   job : indicateur  de l'operation a effectuer
 *   0 impression de la table des primitives id et fptr ne sont pas
 *           references
 *   1 recherche du pointeur fptr associe au nom id, le sous programme
 *           retourne la valeur du pointeur dans fptr. S'il n'y a pas
 *           de fonction correspondant a id funtab retourne fptr=0
 *           si id est un nom incorrect funtab retourne fptr=-1
 *   2 recherche du nom id associe au pointeur fptr s'il n'y a pas
 *           de fonction associee a ce pointeur funtab retourne
 *           id(1)=0
 *   3 ajout du nom donne par id avec le pointeur donne par fptr
 *           en cas d'erreur (nom incorrecte ou capacite de la table
 *           depasse funtab appele error et retourne avec err>0
 *   4 suppression du nom donne par id, le sous programme retourne
 *           la valeur du pointeur associe dans fptr. S'il n'y a pas
 *           de fonction correspondant a id funtab retourne fptr=0
 *           si id est un nom incorrect funtab retourne fptr=-1
 *   id  :vecteur de taille nsiz contenant le code scilab du nom
 *   fptr:entier
 ************************************************************/
int C2F(funtab)(int *id, int *fptr, int *job)
{
  int level=0, j=0;
  switch ( *job) 
    {
    case 0 : /* print */ 
	  /* OBSOLETE : what() scilab function  is so far the only user of this case */
      sciprint("Internal functions: \n\n");      
      while ( SciFuncs[j].name != (char *) 0 )
	  {
        sciprint("%s",SciFuncs[j].name); j++;
        if (j%5==0) sciprint("\n"); else sciprint(" ");
      }
      sciprint("\n\n");      
    break;
    case 1 :
      *fptr=0;
      action_hashtable_scilab_functions(id,fptr,&level,SCI_HFUNCTIONS_FIND);
      break;
    case 2 : 
      action_hashtable_scilab_functions(id,fptr,&level,SCI_HFUNCTIONS_BACKSEARCH);
      break;
    case 3 : 
      if ( action_hashtable_scilab_functions(id,fptr,&level,SCI_HFUNCTIONS_ENTER) == FAILED );
      break;
    case 4 : 
      action_hashtable_scilab_functions(id,fptr,&level,SCI_HFUNCTIONS_DELETE);
      break;
    }
  return(0);
}
/*-----------------------------------------------------------------------------------*/  
