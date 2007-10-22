
/*************************************************************************/
/* Doubly Linked DoublyLinkedList Management                             */
/* DoublyLinkedList.c                                                    */
/*                                                                       */
/* Authors : Daniel Lacroix (Public Domain)                              */
/* Software by Daniel Lacroix 15/09/2006.                                */
/* Modification : Jean-Baptiste Silvy for compatibility with Scilab      */
/*                                                                       */
/* TODO : Translate comments and use doxygen help syntax                 */
/*                                                                       */
/*************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "DoublyLinkedList.h"
#include "MALLOC.h"

/*****************************************************/
/**
 * Rajoute en tete de la DoublyLinkedListe pDoublyLinkedList l'element data.
 * Renvoie le nouveau point d'entree de la DoublyLinkedListe.
 * @param pDoublyLinkedList
 * @param data
 * @return DoublyLinkedList
 */
DoublyLinkedList *List_prepend(DoublyLinkedList *pDoublyLinkedList, void * data)
{
	DoublyLinkedList *vDoublyLinkedList;

	/* on alloue la memoire pour le nouvel element */
	if((vDoublyLinkedList = (DoublyLinkedList *)MALLOC(sizeof(DoublyLinkedList))) == NULL)
		{ printf("MALLOC failed.\n"); return NULL ; }

	if(pDoublyLinkedList == NULL)
		{ /* the DoublyLinkedListe is empty */
			vDoublyLinkedList->data = data;
			vDoublyLinkedList->prev = vDoublyLinkedList;
			vDoublyLinkedList->next = vDoublyLinkedList;
		} else {
			/* il y a deja au moins un element dans la DoublyLinkedListe */
			vDoublyLinkedList->data = data;
			vDoublyLinkedList->prev = pDoublyLinkedList->prev;
			vDoublyLinkedList->next = pDoublyLinkedList;
			pDoublyLinkedList->prev->next = vDoublyLinkedList;
			pDoublyLinkedList->prev = vDoublyLinkedList;
		}
	return(vDoublyLinkedList);
}
/*****************************************************/

/******************************************************/
/**
 * Rajoute en queue de la DoublyLinkedListe pDoublyLinkedList l'element data.
 * Renvoie le nouveau point d'entree de la DoublyLinkedListe.
 * @param pDoublyLinkedList
 * @param data
 * @return DoublyLinkedList
 */
DoublyLinkedList *List_append(DoublyLinkedList *pDoublyLinkedList, void *  data)
{
	DoublyLinkedList *vDoublyLinkedList;

	/* on alloue la memoire pour le nouvelle element */
	if((vDoublyLinkedList = (DoublyLinkedList *)MALLOC(sizeof(DoublyLinkedList))) == NULL)
		{ printf("MALLOC failed.\n"); return NULL ; }

	if(pDoublyLinkedList == NULL)
		{ /* la DoublyLinkedListe est vide */
			vDoublyLinkedList->data = data;
			vDoublyLinkedList->prev = vDoublyLinkedList;
			vDoublyLinkedList->next = vDoublyLinkedList;
			return(vDoublyLinkedList);
		} else {
			/* il y a deja au moins un element dans la DoublyLinkedListe */
			vDoublyLinkedList->data = data;
			vDoublyLinkedList->prev = pDoublyLinkedList->prev;
			vDoublyLinkedList->next = pDoublyLinkedList;
			pDoublyLinkedList->prev->next = vDoublyLinkedList;
			pDoublyLinkedList->prev = vDoublyLinkedList;
			return(pDoublyLinkedList);
		}
}
/******************************************************/

/**************************************/
/**
 * Libere la DoublyLinkedListe (pas les elements)
 * @param DoublyLinkedList
 */
void List_free(DoublyLinkedList *pDoublyLinkedList)
{
	DoublyLinkedList *vDoublyLinkedListMove,*vDoublyLinkedListMoveNext;

	vDoublyLinkedListMove = pDoublyLinkedList;
	while(!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove))
		{
			vDoublyLinkedListMoveNext = List_next(pDoublyLinkedList,vDoublyLinkedListMove);
			FREE(vDoublyLinkedListMove);
			vDoublyLinkedListMove = vDoublyLinkedListMoveNext;
		}
}
/**************************************/

/*************************************************************************/
/* Libere la DoublyLinkedListe et appele la procedure FREE_func pour chaque elements */
void List_free_full(DoublyLinkedList *pDoublyLinkedList, void (* FREE_func)(void *  data))
{
	DoublyLinkedList *vDoublyLinkedListMove,*vDoublyLinkedListMoveNext;

	vDoublyLinkedListMove = pDoublyLinkedList;
	while(!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove))
		{
			vDoublyLinkedListMoveNext = List_next(pDoublyLinkedList,vDoublyLinkedListMove);
			FREE_func(vDoublyLinkedListMove->data);
			FREE(vDoublyLinkedListMove);
			vDoublyLinkedListMove = vDoublyLinkedListMoveNext;
		}
}
/*************************************************************************/

/***************************************************************************/
/* Libere la DoublyLinkedListe et appele la procedure FREE (du C) pour chaque elements */
void List_free_full_simple(DoublyLinkedList *pDoublyLinkedList)
{
	DoublyLinkedList *vDoublyLinkedListMove,*vDoublyLinkedListMoveNext;

	vDoublyLinkedListMove = pDoublyLinkedList;
	while(!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove))
		{
			vDoublyLinkedListMoveNext = List_next(pDoublyLinkedList,vDoublyLinkedListMove);
			if(vDoublyLinkedListMove->data != NULL) FREE(vDoublyLinkedListMove->data);
			FREE(vDoublyLinkedListMove);
			vDoublyLinkedListMove = vDoublyLinkedListMoveNext;
		}
}
/***************************************************************************/

/*****************************************************/
/* Renvoi le nombre d'element contenue dans la DoublyLinkedListe */
/* Cette operation est realisee par comptage.        */
int List_nb_item(DoublyLinkedList *pDoublyLinkedList)
{
	DoublyLinkedList *vDoublyLinkedListMove;
	int compteur = 0;

	vDoublyLinkedListMove = pDoublyLinkedList;
	while(!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove))
		{
			compteur++;
			List_move_next(pDoublyLinkedList,vDoublyLinkedListMove);
		}
	return(compteur);
}
/*****************************************************/

/************************************************************************************/
/* Libere un element de la DoublyLinkedListe (pas ce que l'element pointe si c'est un pointeur) */
/* Renvoie le nouveau point d'entree de la DoublyLinkedListe.                                   */
DoublyLinkedList *List_free_item(DoublyLinkedList *pDoublyLinkedList, void *  data)
{
	DoublyLinkedList *vDoublyLinkedListMove,*vDoublyLinkedListMoveNext;

	/* on parcourt la DoublyLinkedListe pDoublyLinkedList a la recherche de l'element a supprimer */
	vDoublyLinkedListMove = pDoublyLinkedList;
	while(!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove) && (vDoublyLinkedListMove->data != data))
		{ List_move_next(pDoublyLinkedList,vDoublyLinkedListMove); }
	/* si on a trouve l'element, on le libere */
	if(!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove))
		{
			/* si il n'y a que notre element dans la DoublyLinkedListe */
			if(vDoublyLinkedListMove->next == vDoublyLinkedListMove)
				{ /* on libere l'element */
					FREE(vDoublyLinkedListMove);
					/* on renvoie une DoublyLinkedListe vide */
					return(NULL);
				} else {
					/* on recupere l'element qui suit celui que l'on veut supprimer */
					vDoublyLinkedListMoveNext = vDoublyLinkedListMove->next;
					/* on chaine l'element precedent avec celui qui suit */
					vDoublyLinkedListMove->prev->next = vDoublyLinkedListMoveNext;
					vDoublyLinkedListMoveNext->prev = vDoublyLinkedListMove->prev;
					/* on libere l'element a liberer */
					FREE(vDoublyLinkedListMove);
					if(pDoublyLinkedList == vDoublyLinkedListMove)
						{
							/* l'element a supprimer etait le premier, c'est */
							/* donc le suivant qui devient le premier        */
							return(vDoublyLinkedListMoveNext);
						} else {
							/* l'element a supprimer n'etait pas le premier.       */
							/* le premiere element de la DoublyLinkedListe ne change donc pas. */
							return(pDoublyLinkedList);
						}
				}
		}
	/* si on est ici c'est l'element a supprimer n'existe */
	/* pas dans la DoublyLinkedListe. on rend donc la DoublyLinkedListe intacte.  */
	return(pDoublyLinkedList);
}
/************************************************************************************/

/************************************************************************************/
/* Libere un element de la DoublyLinkedListe (pas ce que l'element pointe si c'est un pointeur) */
/* Si l'element a ete libere alors done == TRUE sinon FALSE.                        */
/* Renvoie le nouveau point d'entree de la DoublyLinkedListe.                                   */
DoublyLinkedList *List_free_item_with_check(DoublyLinkedList *pDoublyLinkedList, void *  data, BOOL *done)
{
	DoublyLinkedList *vDoublyLinkedListMove,*vDoublyLinkedListMoveNext;

	/* on parcourt la DoublyLinkedListe pDoublyLinkedList a la recherche de l'element a supprimer */
	vDoublyLinkedListMove = pDoublyLinkedList;
	while(!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove) && (vDoublyLinkedListMove->data != data))
		{ List_move_next(pDoublyLinkedList,vDoublyLinkedListMove); }
	/* si on a trouve l'element, on le libere */
	if(!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove))
		{
			/* on signal que l'element a bien etait trouve */
			*done = TRUE;
			/* si il n'y a que notre element dans la DoublyLinkedListe */
			if(vDoublyLinkedListMove->next == vDoublyLinkedListMove)
				{ /* on libere l'element */
					FREE(vDoublyLinkedListMove);
					/* on renvoie une DoublyLinkedListe vide */
					return(NULL);
				} else {
					/* on recupere l'element qui suit celui que l'on veut supprimer */
					vDoublyLinkedListMoveNext = vDoublyLinkedListMove->next;
					/* on chaine l'element precedent avec celui qui suit */
					vDoublyLinkedListMove->prev->next = vDoublyLinkedListMoveNext;
					vDoublyLinkedListMoveNext->prev = vDoublyLinkedListMove->prev;
					/* on libere l'element a liberer */
					FREE(vDoublyLinkedListMove);
					if(pDoublyLinkedList == vDoublyLinkedListMove)
						{
							/* l'element a supprimer etait le premier, c'est */
							/* donc le suivant qui devient le premier        */
							return(vDoublyLinkedListMoveNext);
						} else {
							/* l'element a supprimer n'etait pas le premier.       */
							/* le premiere element de la DoublyLinkedListe ne change donc pas. */
							return(pDoublyLinkedList);
						}
				}
		}
	/* si on est ici c'est l'element a supprimer n'existe pas dans la DoublyLinkedListe.*/
	/* on signal de fait que l'on n'a pas trouve l'element cherche.         */
	*done = FALSE;
	/* on rend donc la DoublyLinkedListe intacte. */
	return(pDoublyLinkedList);
}
/************************************************************************************/

/***************************************************************/
/* Renvoie l'element a la position item_nb dans la DoublyLinkedListe pDoublyLinkedList */
/* Si l'element item_nb n'exite pas, la fonction renvoi NULL.  */
/* item_nb commence a 1.                                       */
void *  List_item(DoublyLinkedList *pDoublyLinkedList, int item_nb)
{
	DoublyLinkedList *vDoublyLinkedListMove;
	int offset=0;

	vDoublyLinkedListMove = pDoublyLinkedList;
	while(!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove) && (offset<item_nb-1))
		{ List_move_next(pDoublyLinkedList,vDoublyLinkedListMove); offset++; }

	if(!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove))
		{ /* si il y a bien un element a la position demande, on le renvoie */
			return(vDoublyLinkedListMove->data);
		} else {
			/* si on c'est retrouve a la fin de la DoublyLinkedListe, il n'y a pas d'element */
			/* a la postion demande. On renvoie donc NULL pour le signaler       */
			return(NULL);
		}
}
/***************************************************************/

/*******************************************************************/
/* Rajoute un element apres l'element before. Si l'element before  */
/* n'est pas trouve alors le nouvel element sera rajoute a la fin. */
/* Renvoie ne nouveau point d'entree de la DoublyLinkedListe pDoublyLinkedList.            */
DoublyLinkedList *List_insert_after_item(DoublyLinkedList *pDoublyLinkedList, void *  before, void *  data)
{
	DoublyLinkedList *vDoublyLinkedList;
	DoublyLinkedList *vDoublyLinkedListMove;

	/* on alloue la memoire pour le nouvelle element */
	if((vDoublyLinkedList = (DoublyLinkedList *)MALLOC(sizeof(DoublyLinkedList))) == NULL)
		{ printf("MALLOC failed.\n"); return NULL; }

	vDoublyLinkedListMove = pDoublyLinkedList;
	while(!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove) && (vDoublyLinkedListMove->data != before))
		{ List_move_next(pDoublyLinkedList,vDoublyLinkedListMove); }

	/* si on a trouve l'element precedent */
	if(!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove))
		{
			vDoublyLinkedList->data = data;
			vDoublyLinkedList->prev = vDoublyLinkedListMove;
			vDoublyLinkedList->next = vDoublyLinkedListMove->next;
			vDoublyLinkedListMove->next = vDoublyLinkedList;
			vDoublyLinkedList->next->prev = vDoublyLinkedList;
			return(pDoublyLinkedList);
			/* si on n'a pas trouve l'element precedent */
		} else {
			if(pDoublyLinkedList == NULL)
				{ /* si la DoublyLinkedListe est vide */
					vDoublyLinkedList->data = data;
					vDoublyLinkedList->prev = vDoublyLinkedList;
					vDoublyLinkedList->next = vDoublyLinkedList;
					return(vDoublyLinkedList);
				} else {
					/* c'est une insertion en queue d'une DoublyLinkedListe non vide */
					vDoublyLinkedList->data = data;
					vDoublyLinkedList->prev = pDoublyLinkedList->prev;
					vDoublyLinkedList->next = pDoublyLinkedList;
					vDoublyLinkedList->prev->next = vDoublyLinkedList;
					pDoublyLinkedList->prev = vDoublyLinkedList;
					return(pDoublyLinkedList);
				}
		}
}
/*********************************************************************/

/****************************************************************************/
/* Concatene la DoublyLinkedListe p2 a la DoublyLinkedListe p1 (p1 devant, p2 derriere)             */
/* Renvoie la nouvelle DoublyLinkedListe cree. p1 et p2 sont consideres comme detruites */
DoublyLinkedList *List_concat(DoublyLinkedList *p1, DoublyLinkedList *p2)
{
	DoublyLinkedList *vDoublyLinkedListPrev;

	if(p1 == NULL) return(p2);
	if(p2 == NULL) return(p1);

	p1->prev->next = p2;
	vDoublyLinkedListPrev = p1->prev;
	p1->prev = p2->prev;
	p2->prev->next = p1;
	p2->prev = vDoublyLinkedListPrev;

	return(p1);
}
/****************************************************************************/

/*****************************************************************/
/* Depile le dernier element de la DoublyLinkedListe pDoublyLinkedList.                  */
/* Le dernier est place dans data et est sorti de la DoublyLinkedListe       */
/* List_pop renvoie le nouveau point d'entree de la DoublyLinkedListe pDoublyLinkedList. */
DoublyLinkedList *List_pop(DoublyLinkedList *pDoublyLinkedList, void *  *data)
{
	DoublyLinkedList *vDoublyLinkedListPrev;

	if(pDoublyLinkedList == NULL) { *data = NULL; return(NULL); }

	/* si il n'y a que notre element dans la DoublyLinkedListe */
	if(pDoublyLinkedList->next == pDoublyLinkedList)
		{
			*data = pDoublyLinkedList->data;
			/* on libere l'element */
			FREE(pDoublyLinkedList);
			/* on renvoie une DoublyLinkedListe vide */
			return(NULL);
		} else {
			*data = pDoublyLinkedList->prev->data;
			vDoublyLinkedListPrev = pDoublyLinkedList->prev->prev;
			vDoublyLinkedListPrev->next = pDoublyLinkedList;
			FREE(pDoublyLinkedList->prev);
			pDoublyLinkedList->prev = vDoublyLinkedListPrev;
			return(pDoublyLinkedList);
		}
}
/*****************************************************************/

/***********************************************************************/
/* Renvoie une copie de la DoublyLinkedListe pDoublyLinkedList. Les pointeurs data sont copies */
/* pas le contenu pointe. Il s'agit d'une DoublyLinkedListe avec des "alias" sur   */
/* les elements de la premiere DoublyLinkedListe.                                  */
DoublyLinkedList *List_copy(DoublyLinkedList *pDoublyLinkedList)
{
	DoublyLinkedList *vDoublyLinkedListMove, *vCopy;

	vCopy = DoublyLinkedList_new();
	for(vDoublyLinkedListMove = pDoublyLinkedList;!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove);List_move_next(pDoublyLinkedList,vDoublyLinkedListMove))
		{
			vCopy = List_append(vCopy, vDoublyLinkedListMove->data);
		}
	return(vCopy);
}
/***********************************************************************/

/**************************************************************************************/
/* Renvoie une copie de la DoublyLinkedListe pDoublyLinkedList. Les donnees contenu dans la DoublyLinkedListe sont copies */
/* avec la fonction copy_func. Elle recoit le pointeur de l'element d'origine et doit */
/* renvoyer un pointeur vers la copie.                                                */
DoublyLinkedList *List_copy_full(DoublyLinkedList *pDoublyLinkedList, void *  (* copy_func)(void *  data))
{
	DoublyLinkedList *vDoublyLinkedListMove, *vCopy;

	vCopy = DoublyLinkedList_new();
	for(vDoublyLinkedListMove = pDoublyLinkedList;!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove);List_move_next(pDoublyLinkedList,vDoublyLinkedListMove))
		{
			vCopy = List_append(vCopy, copy_func(vDoublyLinkedListMove->data));
		}
	return(vCopy);
}
/**************************************************************************************/

/***********************************************************************/
/* Libere un element de la DoublyLinkedListe pDoublyLinkedList (pas la donnee pointe par data) */
/* Renvoie le nouveau point d'entree de la DoublyLinkedListe.                      */
DoublyLinkedList *List_free_chunk(DoublyLinkedList *pDoublyLinkedList, DoublyLinkedList *pToFREE)
{
	DoublyLinkedList *vDoublyLinkedListNext;

	if(pDoublyLinkedList == NULL) return(NULL);
	if(pToFREE == NULL) return(pDoublyLinkedList);

	/* si il n'y a que notre element dans la DoublyLinkedListe */
	if(pToFREE->next == pToFREE)
		{ /* on libere l'element */
			FREE(pToFREE);
			/* on renvoie une DoublyLinkedListe vide */
			return(NULL);
		} else {
			/* on recupere l'element qui suit celui que l'on veut supprimer */
			vDoublyLinkedListNext = pToFREE->next;
			/* on chaine l'element precedent avec celui qui suit */
			pToFREE->prev->next = vDoublyLinkedListNext;
			vDoublyLinkedListNext->prev = pToFREE->prev;
			/* on libere l'element a liberer */
			FREE(pToFREE);
			if(pDoublyLinkedList == pToFREE)
				{
					/* l'element a supprimer etait le premier, c'est */
					/* donc le suivant qui devient le premier        */
					return(vDoublyLinkedListNext);
				} else {
					/* l'element a supprimer n'etait pas le premier.       */
					/* le premiere element de la DoublyLinkedListe ne change donc pas. */
					return(pDoublyLinkedList);
				}
		}
}
/***********************************************************************/

/***************************************************************/
/* Trie la DoublyLinkedListe pDoublyLinkedList. La fonction cmp_func est utilise pour  */
/* determine l'ordre. Elle doit renvoyer 0 si p1 pointe sur un */
/* element equivalent a p2. < 0 si p1 est inferieur a p2, >0   */
/* si p2 est superieur a p1.                                   */
/* Renvoie la DoublyLinkedListe triee.                                     */
DoublyLinkedList *List_sort(DoublyLinkedList *pDoublyLinkedList,int (* cmp_func)(void *  p1, void *  p2))
{
	DoublyLinkedList *vDoublyLinkedListMove;
	DoublyLinkedList *vDoublyLinkedListSmallest;
	DoublyLinkedList *vDoublyLinkedListRes;

	vDoublyLinkedListRes = DoublyLinkedList_new();

	/* l'algorithme de trie utilise est le trie par selection          */
	/* cela consiste a chercher le plus petit element le sortir de     */
	/* la DoublyLinkedListe pDoublyLinkedList et le rajouter a la fin de la DoublyLinkedListe vDoublyLinkedListRes     */
	/* et de repeter cela jusqu'a ce qu'il n'y ai plus rien dans pDoublyLinkedList */
	while(!List_is_empty(pDoublyLinkedList))
		{
			for(vDoublyLinkedListMove = List_next(pDoublyLinkedList,pDoublyLinkedList), vDoublyLinkedListSmallest = pDoublyLinkedList;
				!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove); List_move_next(pDoublyLinkedList,vDoublyLinkedListMove))
				{
					if(cmp_func(List_data(vDoublyLinkedListMove),List_data(vDoublyLinkedListSmallest)) < 0)
						{ vDoublyLinkedListSmallest = vDoublyLinkedListMove; }
				}
			vDoublyLinkedListRes = List_append(vDoublyLinkedListRes,List_data(vDoublyLinkedListSmallest));
			pDoublyLinkedList = List_free_chunk(pDoublyLinkedList, vDoublyLinkedListSmallest);
		}

	return(vDoublyLinkedListRes);
}
/***************************************************************/

/*************************************************************/
/* Insere l'element data dans la DoublyLinkedListe pDoublyLinkedList de maniere trie */
/* d'apres la fonction cmp_func. Elle doit renvoyer 0 si p1  */
/* pointe sur un element equivalent a p2. < 0 si p1 est      */
/* inferieur a p2, >0 si p2 est superieur a p1.              */
/* Renvoie le nouveau point d'entree de la DoublyLinkedListe.            */
DoublyLinkedList *List_insert_sorted(DoublyLinkedList *pDoublyLinkedList,int (* cmp_func)(void *  p1, void *  p2), void *  data)
{
	DoublyLinkedList *vDoublyLinkedListMove, *vDoublyLinkedList, *tmp;

	/* on recherche le premier element superieur ou egale au nouveau */
	for(vDoublyLinkedListMove = pDoublyLinkedList; !List_is_end(pDoublyLinkedList,vDoublyLinkedListMove) &&
	      (cmp_func(List_data(vDoublyLinkedListMove),data) < 0); ) {
	  tmp = List_move_next(pDoublyLinkedList,vDoublyLinkedListMove);
	  vDoublyLinkedListMove = tmp;
	}

	/* on alloue la memoire pour le nouvelle element */
	if((vDoublyLinkedList = (DoublyLinkedList *)MALLOC(sizeof(DoublyLinkedList))) == NULL)
		{ printf("MALLOC failed.\n"); return NULL; }

	if(List_is_empty(pDoublyLinkedList))
		{ /* si la DoublyLinkedListe est vide */
			vDoublyLinkedList->data = data;
			vDoublyLinkedList->prev = vDoublyLinkedList;
			vDoublyLinkedList->next = vDoublyLinkedList;
			return(vDoublyLinkedList);
		} else {
			vDoublyLinkedList->data = data;
			vDoublyLinkedList->prev = vDoublyLinkedListMove->prev;
			vDoublyLinkedList->next = vDoublyLinkedListMove;
			vDoublyLinkedListMove->prev = vDoublyLinkedList;
			vDoublyLinkedList->prev->next = vDoublyLinkedList;
			if(pDoublyLinkedList == vDoublyLinkedListMove)
				return(vDoublyLinkedList);
			else
				return(pDoublyLinkedList);
		}
}
/*************************************************************/

/***********************************************************/
/* Inverse l'ordre des elements de la DoublyLinkedListe pDoublyLinkedList          */
/* Renvoie le nouveau point d'entree de la DoublyLinkedListe           */
/* Cette operation peut ce faire pendant que l'on parcours */
/* (pas au sens multithreade du terme)                     */
/* la DoublyLinkedListe mais bien evidemment, le sens s'inverse.       */
DoublyLinkedList *List_invert(DoublyLinkedList *pDoublyLinkedList)
{
	DoublyLinkedList *vDoublyLinkedListMove, *vDoublyLinkedListMoveNext;

	if(pDoublyLinkedList == NULL) return(NULL);

	vDoublyLinkedListMove = pDoublyLinkedList;
	do {
		vDoublyLinkedListMoveNext = vDoublyLinkedListMove->next;
		vDoublyLinkedListMove->next = vDoublyLinkedListMove->prev;
		vDoublyLinkedListMove->prev = vDoublyLinkedListMoveNext;
		vDoublyLinkedListMove = vDoublyLinkedListMoveNext;
	} while(vDoublyLinkedListMove != pDoublyLinkedList);
	/* pDoublyLinkedList est devenu le dernier element, donc le premier est celui qui suit */
	return(pDoublyLinkedList->next);
}
/***********************************************************/

/*********************************************************************/
/* Recherche l'element de DoublyLinkedListe qui contient la premiere donnee data */
/* Renvoie l'element de DoublyLinkedListe si trouve, NULL sinon.                 */
DoublyLinkedList *List_find(DoublyLinkedList *pDoublyLinkedList, void *  data)
{
	DoublyLinkedList *vDoublyLinkedListMove;

	for(vDoublyLinkedListMove = pDoublyLinkedList; !List_is_end(pDoublyLinkedList,vDoublyLinkedListMove) && (List_data(vDoublyLinkedListMove) != data);
		List_move_next(pDoublyLinkedList,vDoublyLinkedListMove));

	return(vDoublyLinkedListMove);
}
/*********************************************************************/

/*********************************************************************************/
/* Recherche l'element de DoublyLinkedListe qui contient la premiere donnee pour laquelle la */
/* fonction find_func renvoie TRUE. La fonction find_func prend en parametre le  */
/* pointeur contenu dans la DoublyLinkedListe et le pointeur your_data qui est donne par     */
/* l'utilisateur (pour y stocker ce dont il a besoin pour la comparaison).       */
/* Renvoie l'element de DoublyLinkedListe si trouve, NULL sinon.                             */
DoublyLinkedList *List_find_full(DoublyLinkedList *pDoublyLinkedList, void *  your_data, BOOL (* find_func)(void *  data, void *  your_data))
{
	DoublyLinkedList *vDoublyLinkedListMove;

	for(vDoublyLinkedListMove = pDoublyLinkedList;
		!List_is_end(pDoublyLinkedList,vDoublyLinkedListMove) && !find_func(List_data(vDoublyLinkedListMove),your_data);
		List_move_next(pDoublyLinkedList,vDoublyLinkedListMove));

	return(vDoublyLinkedListMove);
}
/*********************************************************************************/
