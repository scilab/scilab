c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1988-2008 - INRIA - Serge STEER
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
C/MEMBR ADD NAME=MPDEGR,SSI=0
c     Copyright INRIA
      subroutine mpdegr(d,nl,deg,m,n)
c!but
c mpdegr calcule le degre maximum des elements d'une matrice de polynomes
c!liste d'appel
c     integer d(nl*n+1),nl,m,n,deg
c     d : tableau entier de taille nl*n+1,  si k=i+(j-1)*nl alors
c          d(k)) contient  l'adresse dans pm du coeff de degre 0
c          du polynome mp(i,j). Le degre du polynome mp(i,j) vaut:
c          d(k+1)-d(k) -1
c     nl : entier definissant le rangement dans d
c     m : nombre de ligne de la matrice polynomiale
c     n : nombre de colonnes de la matrice polynomiale
c
c!
      integer d(*),nl,m,n,deg
c
c determination du degre max de mp
      deg=0
      ip=-nl
      do 10 k=1,n
      ip=ip+nl
      do 10 i=1,m
      deg=max(deg,d(ip+i+1)-d(ip+i)-1)
 10   continue
      return
      end
