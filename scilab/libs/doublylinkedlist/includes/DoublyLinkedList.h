
/*************************************************************************/
/* Doubly Linked DoublyLinkedList Management                             */
/* DoublyLinkedList.h                                                    */
/*                                                                       */
/* Authors : Daniel Lacroix (Public Domain)                              */
/* Software by Daniel Lacroix 15/09/2006.                                */
/* Modification : Jean-Baptiste Silvy for compatibility with Scilab      */
/*                                                                       */
/*************************************************************************/


#ifndef _DOUBLY_LINKED_LIST_
#define _DOUBLY_LINKED_LIST_

#include "BOOL.h"

/* pour les DoublyLinkedListes chainees */
typedef struct _DlDoublyLinkedList DoublyLinkedList;

struct _DlDoublyLinkedList {
  void * data;
  DoublyLinkedList *next;
  DoublyLinkedList *prev;
};

/* Cree une nouvelle DoublyLinkedListe chainee. */
/* DoublyLinkedList *List_new()                 */
#define DoublyLinkedList_new() NULL

/* Rajoute en tete de la DoublyLinkedListe pDoublyLinkedList l'element data. */
/* Renvoie le nouveau point d'entree de la DoublyLinkedListe.    */
DoublyLinkedList *List_prepend(DoublyLinkedList *pDoublyLinkedList, void * data);

/* Rajoute en queue de la DoublyLinkedListe pDoublyLinkedList l'element data. */
/* Renvoie le nouveau point d'entree de la DoublyLinkedListe.     */
DoublyLinkedList *List_append(DoublyLinkedList *pDoublyLinkedList, void * data);

/* Rajoute un element apres l'element before. Si l'element before  */
/* n'est pas trouve alors le nouvel element sera rajoute a la fin. */
/* Renvoie ne nouveau point d'entree de la DoublyLinkedListe pDoublyLinkedList.            */
DoublyLinkedList *List_insert_after_item(DoublyLinkedList *pDoublyLinkedList, void * before, void * data);

/* Concatene la DoublyLinkedListe p2 a la DoublyLinkedListe p1 (p1 devant, p2 derriere)             */
/* Renvoie la nouvelle DoublyLinkedListe cree. p1 et p2 sont consideres comme detruites */
DoublyLinkedList *List_concat(DoublyLinkedList *p1, DoublyLinkedList *p2);

/* Empile data a la fin de la DoublyLinkedListe pDoublyLinkedList.              */
/* Renvoie le nouveau point d'entree de la DoublyLinkedListe pDoublyLinkedList. */
/* void List_push(DoublyLinkedList *pDoublyLinkedList, void * data);           */
#define List_push(pDoublyLinkedList,data) List_append(pDoublyLinkedList,data)

/* Depile le dernier element de la DoublyLinkedListe pDoublyLinkedList.                  */
/* Le dernier est place dans data et est sorti de la DoublyLinkedListe       */
/* List_pop renvoie le nouveau point d'entree de la DoublyLinkedListe pDoublyLinkedList. */
DoublyLinkedList *List_pop(DoublyLinkedList *pDoublyLinkedList, void ** data);

/* Renvoie une copie de la DoublyLinkedListe pDoublyLinkedList. Les pointeurs data sont copies */
/* pas le contenu pointe. Il s'agit d'une DoublyLinkedListe avec des "alias" sur   */
/* les elements de la premiere DoublyLinkedListe.                                  */
DoublyLinkedList *List_copy(DoublyLinkedList *pDoublyLinkedList);

/* Renvoie une copie de la DoublyLinkedListe pDoublyLinkedList. Les donnees contenu dans la DoublyLinkedListe sont copies */
/* avec la fonction copy_func. Elle recoit le pointeur de l'element d'origine et doit */
/* renvoyer un pointeur vers la copie.                                                */
DoublyLinkedList *List_copy_full(DoublyLinkedList *pDoublyLinkedList, void * (* copy_func)(void * data));

/* Libere la DoublyLinkedListe (pas les elements) */
void List_free(DoublyLinkedList *pDoublyLinkedList);

/* Libere la DoublyLinkedListe et appele la procedure free_func pour chaque elements */
void List_free_full(DoublyLinkedList *pDoublyLinkedList, void (* free_func)(void * data));

/* Libere la DoublyLinkedListe et appele la procedure free (du C) pour chaque elements */
void List_free_full_simple(DoublyLinkedList *pDoublyLinkedList);

/* Libere un element de la DoublyLinkedListe (pas ce que l'element pointe si c'est un pointeur) */
/* Renvoie le nouveau point d'entree de la DoublyLinkedListe.                                   */
DoublyLinkedList *List_free_item(DoublyLinkedList *pDoublyLinkedList, void * data);

/* Libere un element de la DoublyLinkedListe (pas ce que l'element pointe si c'est un pointeur) */
/* Si l'element a ete libere alors done == TRUE sinon FALSE.                        */
/* Renvoie le nouveau point d'entree de la DoublyLinkedListe.                                   */
DoublyLinkedList *List_free_item_with_check(DoublyLinkedList *pDoublyLinkedList, void * data, BOOL *done);

/* Libere un element de la DoublyLinkedListe pDoublyLinkedList (pas la donnee pointe par data) */
/* Renvoie le nouveau point d'entree de la DoublyLinkedListe.                      */
DoublyLinkedList *List_free_chunk(DoublyLinkedList *pDoublyLinkedList, DoublyLinkedList *pToFree);

/* Renvoi le nombre d'element contenue dans la DoublyLinkedListe */
/* Cette operation est realisee par comptage.        */
int List_nb_item(DoublyLinkedList *pDoublyLinkedList);

/* Renvoie l'element a la position item_nb dans la DoublyLinkedListe pDoublyLinkedList */
/* Si l'element item_nb n'exite pas, la fonction renvoi NULL.  */
/* item_nb commence a 1.                                       */
void * List_item(DoublyLinkedList *pDoublyLinkedList, int item_nb);

/* Renvoie la donnee correspondant a l'element de DoublyLinkedListe pDoublyLinkedList */
/* void * List_data(DoublyLinkedList *pDoublyLinkedList);                            */
#define List_data(pDoublyLinkedList) ((pDoublyLinkedList)?(pDoublyLinkedList)->data:NULL)

/* Renvoie l'element a la derniere position de la DoublyLinkedListe pDoublyLinkedList. */
/* Renvoie NULL si la DoublyLinkedListe est vide.                          */
/* DoublyLinkedList *List_last(DoublyLinkedList *pDoublyLinkedList);                               */
#define List_last(pDoublyLinkedList) ((pDoublyLinkedList)?(pDoublyLinkedList)->prev:NULL)

/* Renvoie TRUE si la DoublyLinkedListe pDoublyLinkedList est vide. Renvoie FALSE sinon. */
/* BOOL List_is_empty(DoublyLinkedList *pDoublyLinkedList);                           */
#define List_is_empty(pDoublyLinkedList) (pDoublyLinkedList == NULL)

/* Renvoie TRUE si pCurrentDoublyLinkedList est au dela du dernier   */
/* element de la DoublyLinkedListe pDoublyLinkedList. Sinon renvoie FALSE        */
/* BOOL List_is_end(DoublyLinkedList *pDoublyLinkedList, DoublyLinkedList *pCurrentDoublyLinkedList); */
#define List_is_end(pDoublyLinkedList,pCurrentDoublyLinkedList) (pCurrentDoublyLinkedList == NULL)

/* Renvoie TRUE si pCurrentDoublyLinkedList est le premier            */
/* element de la DoublyLinkedListe pDoublyLinkedList. Sinon renvoie FALSE         */
/* BOOL List_is_first(DoublyLinkedList *pDoublyLinkedList, DoublyLinkedList *pCurrentDoublyLinkedList);*/
#define List_is_first(pDoublyLinkedList,pCurrentDoublyLinkedList) ((pCurrentDoublyLinkedList) == pDoublyLinkedList)

/* Renvoie TRUE si pCurrentDoublyLinkedList est le dernier            */
/* element de la DoublyLinkedListe pDoublyLinkedList. Sinon renvoie FALSE         */
/* BOOL List_is_last(DoublyLinkedList *pDoublyLinkedList, DoublyLinkedList *pCurrentDoublyLinkedList); */
#define List_is_last(pDoublyLinkedList,pCurrentDoublyLinkedList)\
  ((pCurrentDoublyLinkedList != NULL) && (pDoublyLinkedList != NULL) && ((pCurrentDoublyLinkedList)->next == pDoublyLinkedList))

/* Renvoie l'element apres pCurrentDoublyLinkedList dans la DoublyLinkedListe pDoublyLinkedList.       */
/* Si on arrive au dela de la fin de la DoublyLinkedListe pDoublyLinkedList, renvoie NULL. */
/* DoublyLinkedList *List_next(DoublyLinkedList *pDoublyLinkedList, DoublyLinkedList *pCurrentDoublyLinkedList);               */
#define List_next(pDoublyLinkedList,pCurrentDoublyLinkedList)\
  (((pCurrentDoublyLinkedList)&&((pCurrentDoublyLinkedList)->next!=pDoublyLinkedList))?(pCurrentDoublyLinkedList)->next:NULL)

/* Renvoie l'element avant pCurrentDoublyLinkedList dans la DoublyLinkedListe pDoublyLinkedList.          */
/* Si l'element precedent est avant le premier element, renvoie NULL. */
/* DoublyLinkedList *List_prev(DoublyLinkedList *pDoublyLinkedList, DoublyLinkedList *pCurrentDoublyLinkedList);                  */
#define List_prev(pDoublyLinkedList,pCurrentDoublyLinkedList)\
  (((pCurrentDoublyLinkedList)&&((pCurrentDoublyLinkedList)!=pDoublyLinkedList))?(pCurrentDoublyLinkedList)->prev:NULL)

/* Deplace pCurrentDoublyLinkedList sur le prochain element de la DoublyLinkedListe pDoublyLinkedList */
/* Si on arrive a la fin pCurrentDoublyLinkedList devient NULL.               */
/* void List_move_next(DoublyLinkedList *pDoublyLinkedList, DoublyLinkedList *pCurrentDoublyLinkedList);          */
#define List_move_next(pDoublyLinkedList,pCurrentDoublyLinkedList) (pCurrentDoublyLinkedList = List_next(pDoublyLinkedList,pCurrentDoublyLinkedList))

/* Deplace pCurrentDoublyLinkedList sur l'element precedent de la DoublyLinkedListe pDoublyLinkedList         */
/* Si on arrive avant le premier element alors pCurrentDoublyLinkedList devient NULL. */
/* void List_move_prev(DoublyLinkedList *pDoublyLinkedList, DoublyLinkedList *pCurrentDoublyLinkedList);                  */
#define List_move_prev(pDoublyLinkedList,pCurrentDoublyLinkedList) (pCurrentDoublyLinkedList = List_prev(pDoublyLinkedList,pCurrentDoublyLinkedList))

/* Trie la DoublyLinkedListe pDoublyLinkedList. La fonction cmp_func est utilise pour  */
/* determine l'ordre. Elle doit renvoyer 0 si p1 pointe sur un */
/* element equivalent a p2. < 0 si p1 est inferieur a p2, >0   */
/* si p2 est superieur a p1.                                   */
/* Renvoie la DoublyLinkedListe triee.                                     */
DoublyLinkedList *List_sort(DoublyLinkedList *pDoublyLinkedList,int (* cmp_func)(void * p1, void * p2));

/* Insere l'element data dans la DoublyLinkedListe pDoublyLinkedList de maniere trie */
/* d'apres la fonction cmp_func. Elle doit renvoyer 0 si p1  */
/* pointe sur un element equivalent a p2. < 0 si p1 est      */
/* inferieur a p2, >0 si p2 est superieur a p1.              */
/* Renvoie le nouveau point d'entree de la DoublyLinkedListe.            */
DoublyLinkedList *List_insert_sorted(DoublyLinkedList *pDoublyLinkedList,int (* cmp_func)(void * p1, void * p2),
                         void * data);

/* Inverse l'ordre des elements de la DoublyLinkedListe pDoublyLinkedList          */
/* Renvoie le nouveau point d'entree de la DoublyLinkedListe           */
/* Cette operation peut ce faire pendant que l'on parcours */
/* (pas au sens multithreade du terme)                     */
/* la DoublyLinkedListe mais bien evidemment, le sens s'inverse.       */
DoublyLinkedList *List_invert(DoublyLinkedList *pDoublyLinkedList);

/* Recherche l'element de DoublyLinkedListe qui contient la premiere donnee data */
/* Renvoie l'element de DoublyLinkedListe si trouve, NULL sinon.                 */
DoublyLinkedList *List_find(DoublyLinkedList *pDoublyLinkedList, void * data);

/* Recherche l'element de DoublyLinkedListe qui contient la premiere donnee pour laquelle la */
/* fonction find_func renvoie TRUE. La fonction find_func prend en parametre le  */
/* pointeur contenu dans la DoublyLinkedListe et le pointeur your_data qui est donne par     */
/* l'utilisateur (pour y stocker ce dont il a besoin pour la comparaison).       */
/* Renvoie l'element de DoublyLinkedListe si trouve, NULL sinon.                             */
DoublyLinkedList *List_find_full(DoublyLinkedList *pDoublyLinkedList, void * your_data,  BOOL (* find_func)(void * data, void * your_data));

#endif /* _DOUBLY_LINKED_LIST_ */

