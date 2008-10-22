c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intdegree(id)
      INCLUDE 'stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer volr,vol
      logical ref
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1) then
         call error(42)
         return
      endif
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr
      if(istk(il1).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'degree',il1)
         return
      endif

      m1=istk(il1+1)
      n1=istk(il1+2)
      mn1=m1*n1

      if(mn1.eq.0) then
         if (ref) then
            l=sadr(ilr+4)
            err=l-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            istk(ilr)=1
            istk(ilr+1)=0
            istk(ilr+2)=0
            istk(ilr+3)=0
            lstk(top+1)=l
         endif
         return
      endif
c
      if(istk(il1).eq.2) then
         id1=il1+8
         if(ref) then
            l1=sadr(ilr+4)
            l=l1
         else
            l1=sadr(id1+mn1+1)
            l=sadr(ilr+4)
         endif
         do 31 k=1,mn1
            stk(l1-1+k)=dble(istk(id1+k)-istk(id1+k-1)-1)
 31      continue
         if(ref) then
            istk(ilr)=1
            istk(ilr+1)=m1
            istk(ilr+2)=n1
            istk(ilr+3)=0
         else
            istk(ilr)=1
            istk(ilr+3)=0
            call unsfdcopy(mn1,stk(l1),1,stk(l),1)
         endif
         lstk(top+1)=l+mn1
      elseif(istk(il1).eq.1) then
         istk(ilr)=1
         istk(ilr+1)=m1
         istk(ilr+2)=n1
         istk(ilr+3)=0
         l=sadr(ilr+4)
         call dset(mn1,0.0d+0,stk(l),1)
         lstk(top+1)=l+mn1
      endif
      return
      end
c			=======================================
