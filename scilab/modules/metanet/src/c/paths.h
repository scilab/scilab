
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __PATHS_H__
#define __PATHS_H__

#include "machine.h"


/** NodesToPath converts a vector with node numbers into a path:
 * NODES = [Nn,...,N2,N1] =>
 * PATH = [A1,A2,...,An-1] with Ai = (Ni,Ni+1)
 * n = *psize + 1 
 * @param nodes
 * @param p
 * @param psize
 * @param la
 * @param lp
 * @param ls
*/
void NodesToPath(int *nodes,int **p,int *psize,int *la,int *lp,int *ls);

/** prevn2p_ computes a path from node i to node j from a
 * vector describing the previous nodes in path:
 * node i belongs to path if pln[i] > 0 
 * pln[i] is then the previous node in the sequence 
*/
void C2F(prevn2p)(int *i,int *j,int *m,int *n,int *la,int *lp,int *ls,int *direct,int *pln,int **p,int *psize);


/** ns2p_ converts a node set into a path:
 * NODES = [N1,N2,...,Nn] =>
 * PATH = [A1,A2,...,An-1] with Ai = (Ni,Ni+1)
 * *psize = *nsize - 1 
*/
void C2F(ns2p)(int *nodes,int *nsize,int **p,int *psize,int *la,int *lp,int *ls,int *n);


/** p2ns_ converts a path into a node set:
 * PATH = [A1,A2,...,An] =>
 * NODES = [N1,N2,...,Nn+1] => with Ai = (Ni,Ni+1)
 * with *nsize = *psize + 1 
*/
void C2F(p2ns)(int *p,int *psize,int **nodes,int *nsize,int *la,int *lp,int *ls,int *direct,int *m,int *n);



/** edge2st_ computes a spanning tree (root = node 1) from 
 * an array alpha of connecting edge numbers 
 */
void C2F(edge2st)(int *n,int *alpha,int **tree,int *treesize);


/** prevn2st_ computes a spanning tree (root = node i0) from
 * a vector nodes describing the previous nodes in tree 
 */

void C2F(prevn2st)(int *n,int *nodes,int **tree,int *treesize,int *la,int *lp,int *ls);

#endif /* __PATHS_H__ */
