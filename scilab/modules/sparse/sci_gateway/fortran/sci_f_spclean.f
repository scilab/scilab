c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
			subroutine intspclean(id)
      include 'stack.h'
      double precision abstol,reltol
      integer id(nsiz),top0,tops,rhss
      integer iadr, sadr

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs = max(0,rhs)
      top0=top+1-rhs
      tops=top
      rhss=rhs
c
      abstol=1.0d-10
      reltol=1.0d-10
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
         call funnam(ids(1,pt+1),'clean',iadr(lstk(top)))
         fun=-1
         top=tops
         rhs=rhss
         return
      endif
      m=istk(il1+1)
      n=istk(il1+2)
      it=istk(il1+3)
      nel=istk(il1+4)
      l=sadr(il1+5+m+nel)
      ilind=iadr(lw)
      lw=sadr(ilind+m+nel)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(it.eq.0) then
         call dspcle(m,n,stk(l),nel,istk(il1+5),stk(l),nelr,
     $        istk(ilind),abstol,reltol)
      else
         call  wspcle(m,n,stk(l),stk(l+nel),nel,istk(il1+5),stk(l),
     $        stk(l+nel),nelr,istk(ilind),abstol,reltol)
      endif
      if(nelr.eq.nel) return
      call icopy(m+nelr,istk(ilind),1,istk(il1+5),1)
      l1=sadr(il1+5+m+nelr)
      call unsfdcopy(nelr,stk(l),1,stk(l1),1)
      if(it.eq.1) call unsfdcopy(nelr,stk(l+nel),1,stk(l1+nelr),1)
      istk(il1+4)=nelr
      lstk(top+1)=l1+nelr*(it+1)
      return
      end
c			======================================
  
