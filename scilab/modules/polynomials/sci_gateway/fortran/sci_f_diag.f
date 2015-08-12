c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine intpdiag(id)
      INCLUDE 'stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer volr,vol,tops
      logical ref

c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw=lstk(top+1)
      tops=top
c
      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1.and.rhs.ne.2) then
         call error(42)
         return
      endif
c
      kdiag=0
      if (rhs .eq. 2) then
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1) then
            err=1
            call  error(53)
            return
         endif
         if(istk(il+3).ne.0) then
            err=1
            call  error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=1
            call error(89)
            return
         endif
         kdiag = int(stk(sadr(il+4)))
         top = top-1
      endif

      lw=lstk(top+1)
c
      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr
      if(istk(il1).ne.2) then
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'diag',il1)
         return
      endif
c
      id1=il1+8
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      mn1=m1*n1
      l1=sadr(id1+mn1+1)
      vol=istk(id1+mn1)-1

c
      if(ref) then
         idr=ilr+8
      else
         idr=iadr(lw)
      endif
      if(m1.eq.1.or.n1.eq.1) then
         m1=mn1
         n1=0
         err=sadr(idr+mn1*(mn1+abs(kdiag)))-lstk(bot)
      else
         err=sadr(idr+min(n1,m1))-lstk(bot)
      endif
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      call mpdiag(istk(id1),m1,n1,kdiag,istk(idr),mr,nr)
      if(nr.le.0.or.mr.le.0) then
         istk(ilr)=1
         istk(ilr+1)=0
         istk(ilr+2)=0
         istk(ilr+3)=0
         lstk(top+1)=sadr(ilr+4)
         goto 999
      endif
      lr=sadr(idr+mr*nr+1)
      volr=istk(idr)
      err=lr+volr*(it1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if (it1.eq.0) then
         call dmpins(stk(l1),istk(id1),mn1,1,stk,1,0,0,
     1        stk(lr),istk(idr),mr,nr)
      else
         call wmpins(stk(l1),stk(l1+vol),istk(id1),mn1,1,
     1     stk,stk,1,0,0,stk(lr),stk(lr+volr),istk(idr),mr,nr)
      endif
      if(ref) then
         call icopy(8,istk(il1),1,istk(ilr),1)
         l1=sadr(idr+mr*nr+1)
      else
         call icopy(mr*nr+1,istk(idr),1,istk(id1),1)
         l1=sadr(id1+mr*nr+1)
         call unsfdcopy(volr*(it1+1),stk(lr),1,stk(l1),1)
      endif
      istk(ilr+1)=mr
      istk(ilr+2)=nr
      lstk(top+1)=l1+volr*(it1+1)
      goto 999
 999  return
      end
c			=======================================
