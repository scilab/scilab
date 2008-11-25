c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
      subroutine intfrexp(id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE 'stack.h'
      integer id(nsiz)
      logical ref
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (lhs .ne. 2) then
         call error(41)
         return
      endif
      if(rhs.ne.1) then
         call error(39)
         return
      endif
      
      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) then
         il=iadr(istk(il+1))
         ref=.true.
      else
         ref=.false.
      endif
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      if(it.eq.1) then
         buf='frexp: real data only'
         call error(9999)
         return
      endif
      if(istk(il).eq.1) then
         mn=m*n
         l=sadr(il+4)
         lr=sadr(ilr+4)
         err=lr+4+m*n-lstk(bot)
         if (ref) then
            err=err+4*m*n
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(4,istk(il),1,istk(ilr),1)
         endif
      else
         call funnam(ids(1,pt+1),'frexp',il)
         fun=-1
         return
      endif
      lstk(top+1)=lr+mn
      ilr1=iadr(lstk(top+1))
      istk(ilr1)=1
      istk(ilr1+1)=m
      istk(ilr1+2)=n
      istk(ilr1+3)=it
      lr1=sadr(ilr1+4)
      top=top+1
      lstk(top+1)=lr1+m*n
      call vfrexp(mn,stk(l),1,stk(lr),1,stk(lr1),1)
      return
      end
c     -------------------------------
