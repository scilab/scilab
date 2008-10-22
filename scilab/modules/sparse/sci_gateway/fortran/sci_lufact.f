c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intlufact(id)
      include 'stack.h'
      double precision abstol,reltol
      integer id(nsiz),top0,tops,rhss,fmatindex
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs
      tops=top
      rhss=rhs

      if (rhs .ne. 1.and.rhs .ne. 2) then
         call error(39)
         return
      endif
      if (lhs .gt. 2) then
         call error(41)
         return
      endif
      abstol=stk(leps)
      reltol=0.001d0
      if(rhs.eq.2) then
c     checking variable tol (number 2)
         il=iadr(lstk(top))
         if (istk(il) .ne. 1) then
            err = 1
            call error(53)
            return
         endif
         l=sadr(il+4)
         if (istk(il+2)*istk(il+1).eq.1) then
            abstol=stk(l)
         elseif (istk(il+2)*istk(il+1).eq.2) then
            abstol=stk(l)
            reltol=stk(l+1)
         else
            err = 1
            call error(89)
            return
         endif
         top=top-1
         rhs=rhs-1
      endif
      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
c     
c     checking variable sp (number 1)
c     
      il1 = iadr(lstk(top))
      if (istk(il1) .ne. 5) then
         call funnam(ids(1,pt+1),'lufact',iadr(lstk(top)))
         fun=-1
         top=tops
         rhs=rhss
         return
      endif
      m=istk(il1+1)
      n=istk(il1+2)
      if(m.ne.n) then
         err=1
         call error(20)
         return
      endif
      it=istk(il1+3)
      nel=istk(il1+4)
      l=sadr(il1+5+m+nel)
      if(it.ne.0) then
         err=1
         call error(52)
         return
      endif

     
      mx=max(m,n)
      call lufact1(stk(l),istk(il1+5),istk(il1+5+m),mx,nel,
     $     fmatindex,abstol,reltol,nrank,ierr)
      if(ierr.gt.0) then
         buf='not enough memory'
         call error(9999)
         return
      endif        
c     
      top=top-rhs
c     
c     output variable: fmat
c     
      top=top+1
      il=iadr(lstk(top))
      istk(il)=128
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=it
      l=sadr(il+4)
      stk(l)=fmatindex
      lstk(top+1)=l+1
c     
      if(lhs .eq.2) then
c     
c     output variable: rank
c     
         top=top+1
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         l=sadr(il+4)
         stk(l)=nrank
         lstk(top+1)=l+1
      endif
      return
      end

c			======================================
