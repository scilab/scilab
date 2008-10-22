c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
   		subroutine inttriu(id)
      INCLUDE 'stack.h'
      integer id(nsiz)

c
      integer tops
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs.gt.2) then
         call error(42)
         return
      endif

      tops=top
c
      k = 0
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
         k = int(stk(sadr(il+4)))
         top = top-1
      endif


      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).eq.1) then
c     standard matrix case
         m=istk(il+1)
         n=istk(il+2)
         mn=m*n
         it=istk(il+3)
         l=sadr(il+4)

         if(il.ne.ilr) then
            lr=sadr(ilr+4)
            err=lr+mn*(it+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(4,istk(il),1,istk(ilr),1)
            call unsfdcopy(mn*(it+1),stk(l),1,stk(lr),1)
            lstk(top+1)=lr+mn*(it+1)
            l=lr
         endif
c
         if(k.le.0) goto 65
         call dset(m*k,0.0d+0,stk(l),1)
         if(it.eq.1) call dset(m*k,0.0d+0,stk(l+mn),1)
         l=l+m*k
         n=n-k
         k=0
 65      ls=l+1-k
         ll=m-1+k
         do 66 j=1,n
            if(ll.le.0) return
            call dset(ll,0.0d+0,stk(ls),1)
            if(it.eq.1) call dset(ll,0.0d+0,stk(ls+mn),1)
            ll=ll-1
            ls=ls+m+1
 66      continue
      elseif(istk(il).eq.2) then
c       *call* polelm
         top=tops
         fun=16
         fin=11
      else
         top=tops
         call funnam(ids(1,pt+1),'triu',il)
         fun=-1
      endif
      return
      end
c     -------------------------------
