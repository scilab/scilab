c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine intones(id)
      INCLUDE 'stack.h'
      integer id(nsiz)
      logical  getmat

      integer tops, u1, v1
      double precision s
      integer iadr,sadr, mattyp1, areadr1, aimadr1
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs.gt.2) then
         call setfunnam(ids(1,pt+1),'%hm_ones',8)
         fun=-1
c         call error(42)
         return
      endif

      tops=top
c

      if(rhs.le.0) then
c     ones sans argument
         top=top+1
         m=1
         n=1
      elseif(rhs.eq.1) then
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).gt.10) then
            call funnam(ids(1,pt+1),'ones',il)
            fun=-1
            return
         endif
         if(istk(il).lt.0) il=iadr(istk(il+1))
         m=istk(il+1)
         n=istk(il+2)
         if(m.eq.-1.and.n.eq.-1) then !To avoid ones(:)
            call error(21)
            return
         endif
c     ones(matrice)
      elseif(rhs.eq.2) then
c     ones(m,n)
         call getdimfromvar(top,2,n)
         if(err.gt.0.or.err1.gt.0) return
         il=iadr(lstk(top))
         if(istk(il).eq.1.or.istk(il).eq.-1) then
            if (.not.getmat('ones', tops, top, 
     +            mattyp1, u1, v1, areadr1, aimadr1)) return !To have the dimensions of argument #2 
            if(u1.eq.-1.or.v1.eq.-1) then !detect ':' or eye
               call error(21)
               return
            endif
         endif
         
         top=top-1
         call getdimfromvar(top,1,m)
         if(err.gt.0.or.err1.gt.0) return
         il=iadr(lstk(top))
         if(istk(il).eq.1.or.istk(il).eq.-1) then
            if (.not.getmat('ones', tops, top, 
     +            mattyp1, u1, v1, areadr1, aimadr1)) return !To have the dimensions of argument #1 
            if(u1.eq.-1.or.v1.eq.-1) then !detect ':' or eye
               call error(21)
               return
            endif
         endif
      endif
c
      mn=m*n
      if(m.eq.0) n=0
      if(n.eq.0) m=0

      il=iadr(lstk(top))
      l=sadr(il+4)

c     to avoid integer overflow
      s=l+dble(m)*dble(n)- lstk(bot)
      if(s.gt.0.0d0) then
         err=s
         call error(17)
         return
      endif
      istk(il)=1
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=0
      lstk(top+1)=l+mn
      if(mn.eq.0) return
      call dset(mn,1.0d+0,stk(l),1)
      return
      end
c     -------------------------------
