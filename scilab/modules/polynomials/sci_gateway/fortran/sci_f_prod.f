c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intpprod(id)
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
         if(rhs.ge.2) then
            call funnam(ids(1,pt+1),'prod',iadr(lstk(top-rhs+1)))
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
      if(istk(il1).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'prod',il1)
         return
      endif

c
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=istk(il1+1)*istk(il1+2)
      id1=il1+8
      l1=sadr(id1+mn1+1)
      vol=istk(id1+mn1)-1

      if(ref) then
         lr=sadr(ilr+10)
      else
         lr=lw
      endif

      maxd=istk(id1+mn1)-mn1
      err=lr+maxd*(it1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      stk(lr)=1.0d+0
      if(it1.eq.1) stk(lr+maxd)=0.0d+0
      m=0
      do 59 i=1,mn1
         n=istk(id1+i)-istk(id1+i-1)-1
         li=l1-1+istk(id1+i-1)
         if(it1.eq.0) call dpmul1(stk(li),n,stk(lr),m,stk(lr))
         if(it1.eq.1) call wpmul1(stk(li),stk(li+vol),n,stk(lr),
     1        stk(lr+maxd),m,stk(lr),stk(lr+maxd))
         m=m+n
 59   continue

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
