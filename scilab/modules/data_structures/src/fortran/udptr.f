c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine updptr(ilt,ind,nind,dvol)
c!purpose
c     update new data structure pointers recursively for nested lists
c!parameters
c     ilt : pointer to the beginning of the list
c     ind : vector of indexes giving modified entry path.
c     nind: size of the path
c     dvol: difference of the old and new sizes of the modified entry
c!
      integer ind(*),dvol
      integer iadr,sadr
      include 'stack.h'

c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      il=ilt
      if(nind.le.0) return
c
      do 20 k=1,nind
c     .  m : sub-list size
         m=istk(il+1)
         mi=ind(k)
c     .  update pointers to entries following the mi sub list entry
         do 10 i=mi,m
            istk(il+i+2)=istk(il+i+2)+dvol
 10      continue
c     .  il pointer to ind(k) sub-list entry
         il=iadr(sadr(il+3+m)+istk(il+1+mi)-1)
 20   continue
      end

      integer function subptr(ilt,ind,nind)
c     !purpose
c     get pointer to an entry of a  nested list
c     !parameters
c     ilt : pointer to the beginning of the list
c     ind : vector of indexes giving modified entry path.
c     nind: size of the path
c     !
      integer ind(nind)
      integer iadr,sadr
      include 'stack.h'
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      il=ilt
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(nind.le.0) goto 30
c
      do 20 k=1,nind
c     .  m : sub-list size
         m=istk(il+1)
c     .  il pointer to ind(k) sub-list entry
         il=iadr(sadr(il+3+m)+istk(il+1+ind(k))-1)
 20   continue
 30   subptr=il
      end

