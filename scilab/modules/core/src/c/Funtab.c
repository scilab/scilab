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
  switch ( *job) 
    {
    case 0 : /* before do a printf for What OBSOLETE */
    break;
    case 1 :
      *fptr=0;
      action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_FIND);
      break;
    case 2 : 
      action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_BACKSEARCH);
      break;
    case 3 : 
      if ( action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_ENTER) == FAILED );
      break;
    case 4 : 
      action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_DELETE);
      break;
    }
  return(0);
}
/*-----------------------------------------------------------------------------------*/  
