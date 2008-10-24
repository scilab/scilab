c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
      subroutine intround(id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE 'stack.h'
      integer id(nsiz)
      logical ref
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (lhs .ne. 1) then
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

      if(istk(il).eq.8) then
         return
      elseif(istk(il).eq.1) then
         mn=m*n
         l=sadr(il+4)
         lr=sadr(ilr+4)
         if (ref) then
            err=lr+mn*(it+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(4,istk(il),1,istk(ilr),1)
         endif
      elseif(istk(il).eq.2) then
         l=sadr(il+9+m*n)
         lr=sadr(ilr+9+m*n)
         mn=istk(il+8+m*n)-1
         if (ref) then
            err=lr+mn*(it+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(9+m*n,istk(il),1,istk(ilr),1)
         endif
      else
         call funnam(ids(1,pt+1),'round',il)
         fun=-1
         return
      endif

      if(it.eq.1) mn=2*mn
      do 10 i=1,mn
       i1=i-1
       stk(lr+i1)=anint(stk(l+i1))
 10   continue
      lstk(top+1)=lr+mn
      return
      end
c     -------------------------------
      
