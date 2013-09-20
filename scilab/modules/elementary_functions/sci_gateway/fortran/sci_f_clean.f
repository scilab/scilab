c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine intclean(id)
      INCLUDE 'stack.h'
      integer id(nsiz)

      double precision epsa,epsr,norm,eps
      double precision dasum,wasum
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs .lt. 1 .or. rhs .gt. 3) then
         call error(42)
         return
      endif

      ilp=iadr(lstk(top+1-rhs))
      if(istk(ilp).lt.0) ilp=iadr(istk(ilp+1))
      if(istk(ilp).eq.2) then
         fin=17
         fun=16
c        *call* polelm
         return
      elseif(istk(ilp).eq.5) then
         call ref2val
         fin=8
         fun=27
c        *call* spelm
         return
      elseif(istk(ilp).ne.1) then
         call funnam(ids(1,pt+1),'clean',ilp)
         fun=-1
         return
      endif

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

      il=iadr(lstk(top))
      
      if(istk(il).lt.0) then
c     argument is passed by reference
         ilr=il
         il=iadr(istk(il+1))
         mn=istk(il+1)*istk(il+2)
         it=istk(il+3)
         l=sadr(il+4)
         lr=sadr(ilr+4)
         err=lr+mn*(it+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(4,istk(il),1,istk(ilr),1)
         call unsfdcopy(mn*(it+1),stk(l),1,stk(lr),1)
         lstk(top+1)=lr+mn*(it+1)
      else
         mn=istk(il+1)*istk(il+2)
         it=istk(il+3)
         l=sadr(il+4)
         lr=l
         ilr=il
      endif
      
      if(mn.eq.0) return

      if(it.eq.0) then
         norm=dasum(mn,stk(lr),1)
      else
         norm=wasum(mn,stk(lr),stk(lr+mn),1)
      endif
      eps=max(epsa,epsr*norm)
      do 10 kk=0,mn*(it+1)
         if (abs(stk(lr+kk)).le.eps) stk(lr+kk)=0.d0
 10   continue

      return
      end
c     -------------------------------
