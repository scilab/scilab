c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine intpclean(id)
      INCLUDE 'stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vol
      logical ref
      double precision epsa,epsr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if (rhs.lt.1.or.rhs.gt.3) then
         call error(42)
         return
      endif
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).ne.2) then
         fun=-1
         call funnam(ids(1,pt+1),'clean',il1)
         return
      endif
      ref=il1.ne.ilr

c     get relative and absolute tolerances
      epsr=1.0d-10
      epsa=1.0d-10

      if (rhs.eq.3) then
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1) then
            err=3
            call error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=3
            call error(60)
            return
         endif
         if(istk(il+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         epsr=stk(sadr(il+4))
         top=top-1
      endif

      if (rhs.ge.2) then
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1) then
            err=3
            call error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=3
            call error(60)
            return
         endif
         if(istk(il+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         epsa=stk(sadr(il+4))
         top=top-1
      endif

      if(ref) then
         k=istk(ilr+2)
         vol=lstk(k+1)-lstk(k)
         err=lstk(top)+vol-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(vol,stk(lstk(k)),1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+vol
         il1=ilr
      endif
      it1=istk(il1+3)
      m1=istk(il1+1)
      n1=istk(il1+2)
      mn1=m1*n1
      id1=il1+8
      l1=sadr(id1+mn1+1)
      vol=istk(id1+mn1)-1


      id2=iadr(lstk(top+1))
      err=sadr(id2+mn1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(it1.eq.0) then
         call dmpcle(stk(l1),istk(id1),m1,n1,istk(id2),epsr,epsa)
      else
         call wmpcle(stk(l1),stk(l1+vol),istk(id1),m1,n1,
     &        istk(id2),epsr,epsa)
      endif
      lstk(top+1)=l1+(istk(id1+mn1)-1)*(it1+1)
      return
      end
c			=======================================
