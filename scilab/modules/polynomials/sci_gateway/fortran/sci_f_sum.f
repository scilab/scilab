c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intpsum(id)
      INCLUDE 'stack.h'
      integer iadr, sadr
      integer id(nsiz)

      integer vol
      logical ref
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1) then
         if(rhs.eq.2) then
            call funnam(ids(1,pt+1),'sum',iadr(lstk(top-rhs+1)))
            fun=-1
            return
         endif
         call error(42)
         return
      endif
c
      lw=lstk(top+1)
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr
      if(istk(il1).ne.2) then
         fun=-1
         call funnam(ids(1,pt+1),'sum',il1)
         return
      endif
c
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=istk(il1+1)*istk(il1+2)
      id1=il1+8
      l1=sadr(id1+mn1+1)
      vol=istk(id1+mn1)-1
c
      maxd=0
      do 56 i=1,mn1
         m=istk(id1+i)-istk(id1-1+i)
         if(m.gt.maxd) maxd=m
 56   continue

      if(ref) then
         lr=sadr(ilr+10)
      else
         lr=lw
      endif

      err=lw+maxd*(it1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call dset(maxd*(it1+1),0.0d+0,stk(lr),1)
      do 57 i=1,mn1
         li=l1-1+istk(id1-1+i)
         n=istk(id1+i)-istk(id1-1+i)
         call dadd(n,stk(li),1,stk(lr),1)
         if(it1.eq.1) call dadd(n,stk(li+vol),1,stk(lr+maxd),1)
 57   continue
      if(ref) then
         call icopy(9,istk(il1),1,istk(ilr),1)
         istk(ilr)=2
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+9)=1+maxd
      else
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+9)=1+maxd
         l1=sadr(il1+10)
         call unsfdcopy(maxd*(it1+1),stk(lr),1,stk(l1),1)
         lr=l1
      endif
      lstk(top+1)=lr+maxd*(it1+1)
      return
      end
c			=======================================
