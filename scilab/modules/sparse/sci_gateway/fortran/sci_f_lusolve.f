c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine intlusolve(id)
      include 'stack.h'
      double precision abstol,reltol
      integer id(nsiz),top0,hand
      integer iadr, sadr
      logical fact

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs

      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs .ne. 2) then
         call error(39)
         return
      endif
      if (lhs .gt. 1) then
         call error(41)
         return
      endif
c     checking variable fmat (number 1)
c     
      il1 = iadr(lstk(top-rhs+1))
      if (istk(il1) .eq. 128) then
         m1=istk(il1+1)
         n1=istk(il1+2)
         l1 = sadr(il1+4)
         hand=stk(l1)
         fact=.true.
      elseif(istk(il1) .eq. 5) then
         m1=istk(il1+1)
         n1=istk(il1+2)
         if(m1.ne.n1) then
            err=1
            call error(20)
            return
         endif
         it1=istk(il1+3)
         nel1=istk(il1+4)
         l1=sadr(il1+5+m1+nel1)
         if(it1.ne.0) then
            call error(220)
            return
         endif
c     
         abstol=stk(leps)
         reltol=0.001d0
         mx=max(m1,n1)
         call lufact1(stk(l1),istk(il1+5),istk(il1+5+m1),mx,nel1,
     $        hand,abstol,reltol,nrank,ierr)
         if(ierr.gt.0) then
            buf='not enough memory'
            call error(9999)
            return
         endif  
c         if(nrank.ne.m1) then
c            call ludel1(hand)
c            call error(19)
c            return
c         endif
         fact=.false.
      else
         call funnam(ids(1,pt+1),'lusolve',iadr(lstk(top-rhs+1)))
         fun=-1
         return
      endif
c     checking variable b (number 2)
c     
      il2 = iadr(lstk(top-rhs+2))
      if (istk(il2) .ne. 1.and.istk(il2) .ne. 5) then
         call funnam(ids(1,pt+1),'lusolve',iadr(lstk(top-rhs+2)))
         fun=-1
      endif
      m2 = istk(il2+1)
      n2 = istk(il2+2)
      it2 = istk(il2+3)
      if(m2.ne.m1) then
         call error(60)
         return
      endif
      l2 = sadr(il2+4)
      l2i=l2+m2*n2
c     
      if(istk(il2).eq.1) then
c     b is full
         lw3=lw
         lw3i=lw3+m2*n2
         lw=lw+m2*n2*(it2+1)
         err=lw-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
c        
         do 40 j=0,n2-1
            call lusolve1(hand,stk(l2+j*m2),stk(lw3+j*m2),ierr)
            if(it2.eq.1) then
               call lusolve1(hand,stk(l2i+j*m2),stk(lw3i+j*m2),ierr)
            endif
            if (ierr.ne.0) then
               err=1
               call error(247)
               return
            endif
            if (err .gt. 0) return
 40      continue

         if(.not.fact) call ludel1(hand,ierr)
c     
         top=top-rhs
         lw0=lw
         mv=lw0-l0
c     
c     output variable: x
c     
         top=top+1
         il=iadr(lstk(top))
         l=sadr(il+4)
         err=l+m2*(it2+1)-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(il)=1
         istk(il+1)=m2
         istk(il+2)=n2
         istk(il+3)=it2
         call unsfdcopy(m2*n2*(it2+1),stk(lw3),1,stk(l),1)
         lstk(top+1)=l+m2*n2*(it2+1)
      else
c     b is sparse
         call error(222)
         return
      endif
      return
      end
c			======================================
