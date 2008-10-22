c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intnnz(id)
      include 'stack.h'
      integer id(nsiz),top0
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs

      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
c     
c     checking variable sp (number 1)
c     
      il1 = iadr(lstk(top))
      if (istk(il1) .eq. 5) then
         m=istk(il1+1)
         n=istk(il1+2)
         it=istk(il1+3)
         nel=istk(il1+4)
      elseif (istk(il1) .eq. 1) then
         m=istk(il1+1)
         n=istk(il1+2)
         it=istk(il1+3)
         l=sadr(il1+4)
         nel=0
         if(it.eq.0) then
            do 50 i=0,m*n-1
               if(stk(l+i).ne.0.0d0) nel=nel+1
 50         continue
         else
            li=l+m*n
            do 51 i=0,m*n-1
               if(abs(stk(l+i))+abs(stk(li+i)).ne.0.0d0) nel=nel+1
 51         continue
         endif
      else
         call funnam(ids(1,pt+1),'nnz',iadr(lstk(top)))
         fun=-1
         return
      endif
      istk(il1)=1
      istk(il1+1)=1
      istk(il1+2)=1
      istk(il1+3)=0
      l=sadr(il1+4)
      stk(l)=nel
      lstk(top+1)=l+1
      return
      end
c			======================================
  
  
