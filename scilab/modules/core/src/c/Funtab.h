/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __FUNTAB_H__
#define __FUNTAB_H__

#include "machine.h"
/**
 *    Hash table for scilab functions 
 *    job is used to specify a job
 *   @param job : indicateur  de l'operation a effectuer
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
 *   @param id  :vecteur de taille nsiz contenant le code scilab du nom
 *   @param fptr
 */
int C2F(funtab)(int *id, int *fptr, int *job,char *namefunction,unsigned long namefunction_len);

#endif /* __FUNTAB_H__ */
