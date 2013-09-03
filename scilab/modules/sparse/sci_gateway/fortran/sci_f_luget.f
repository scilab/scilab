c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intluget(id)
      include 'stack.h'
      integer id(nsiz),top0,ptr
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs

      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 4) then
         call error(41)
         return
      endif
c     checking variable fmat (number 1)
c     
      il1 = iadr(lstk(top-rhs+1))
      if (istk(il1) .ne. 128) then
         err=1
         call error(218)
         return
      endif
      m=istk(il1+1)
      n=istk(il1+2)
      it1=istk(il1+3)
      l1 = sadr(il1+4)
      ptr=stk(l1)
      call lusiz1(ptr,nl,nu,ierr)
      if (ierr.ne.0) then
         err=1
         call error(247)
         return
      endif
      ilp=il1
      lp=sadr(ilp+5+m+m)
      lw=lp+m*(it1+1)
      lstk(top+1)=lw
c     
      top=top+1
      ill=iadr(lstk(top))
      ll=sadr(ill+5+m+nl)
      lw=ll+nl*(it1+1)
      lstk(top+1)=lw
c     
      top=top+1
      ilu=iadr(lstk(top))
      lu=sadr(ilu+5+n+nu)
      lw=lu+nu*(it1+1)
      lstk(top+1)=lw
c     
      top=top+1
      ilq=iadr(lstk(top))
      lq=sadr(ilq+5+n+n)
      lw=lq+n*(it1+1)
      lstk(top+1)=lw
c     
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(ilp)=5
      istk(ilp+1)=n
      istk(ilp+2)=n
      istk(ilp+3)=it1
      istk(ilp+4)=n
c     
      istk(ill)=5
      istk(ill+1)=n
      istk(ill+2)=n
      istk(ill+3)=it1
      istk(ill+4)=nl
c     
      istk(ilu)=5
      istk(ilu+1)=n
      istk(ilu+2)=n
      istk(ilu+3)=it1
      istk(ilu+4)=nu
c     
      istk(ilq)=5
      istk(ilq+1)=n
      istk(ilq+2)=n
      istk(ilq+3)=it1
      istk(ilq+4)=n
c     
      call luget1(ptr,istk(ilp+5),stk(lp),istk(ill+5),stk(ll),
     $     istk(ilu+5),stk(lu),istk(ilq+5),stk(lq),ierr)
      if (ierr.ne.0) then
         err=1
         call error(247)
         return
      endif
      return
      end
c			======================================
      
