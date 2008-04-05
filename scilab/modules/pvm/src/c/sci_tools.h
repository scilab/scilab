/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1997/1998 - INRIA - Eric FLEURY
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/***
   NAME
     sci_tools
   PURPOSE
     
   NOTES
     
   HISTORY
     fleury - Dec 17, 1997: Created.
     Revision 1.1  2001/04/26 07:49:01  scilab
     Initial revision

     Revision 1.4  1998/04/03 11:41:04  fleury
     Ajout des commandes pvm_start et pvm_halt
     Correction d'erreurs en version non ANSI
     teste sur alpha et sun
     PBM de link en dynamique sur hpux9

     Revision 1.3  1998/03/27 12:20:24  fleury
     Version pvm OK.
     TODO: faire des tests de compil sur plateforme separee (POPC0
     TODO: commenter source (-;
     TODO: faire un peu de netoyage

     Revision 1.2  1998/03/13 13:57:09  fleury
     Version send/recv avec pack. A tester.
     TODO: ajouter les listes + BROADCAST
     TODO: faire un clean du dir et des fichiers...

     Revision 1.1  1997/12/18 18:35:58  fleury
     Premier commit

     Ajout de fct permettant de tester le type d une variable scilab
     conversion de complex format scilab to complex format f77
     TODO:use memcpy
     TODO:use imatrix

***/

#ifndef __SCI_TOOLS
#define __SCI_TOOLS

#define DATA_TYPE_SCALAR     1

#define TYPE_DOUBLE  0
#define TYPE_COMPLEX 1

				/* Valide for Real or Complex matrix */
#define TYPE(ptr)   (((int*) (ptr))[-1])
#define NB_COL(ptr) (((int*) (ptr))[-2])
#define NB_ROW(ptr) (((int*) (ptr))[-3])
#define DATA_TYPE(ptr) (((int*) (ptr))[-4])

#define SET_DATA_TYPE_SCALAR(ptr)  (((int*) (ptr))[-4] = DATA_TYPE_SCALAR)
#define SET_TYPE_DOUBLE(ptr)  (((int*) (ptr))[-1] = TYPE_DOUBLE)
#define SET_TYPE_COMPLEX(ptr) (((int*) (ptr))[-1] = TYPE_COMPLEX)
#define SET_NB_COL(ptr,m) (((int*) (ptr))[-2] = (m))
#define SET_NB_ROW(ptr,n) (((int*) (ptr))[-3] = (n))

typedef struct {
	double re;
	double im;
} complex16;

/**
 * <long-description>
 *
 * @param name
 * @param m
 * @param n
 * @param type
//@}

 * @param lp
 * @return <ReturnValue>
 */
int F2C(mycmatptr)(char *name,int *m, int *n, int *type, int *lp);

/**
 * <long-description>
 *
 * @param n
 * @param ip
 * @param op
 */
void C2F(ccomplexf)(int *n, double **ip, double *op);

#endif /* __SCI_TOOLS */
