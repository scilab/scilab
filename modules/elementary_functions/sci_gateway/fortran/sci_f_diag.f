c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
      subroutine intdiag(id)
      INCLUDE 'stack.h'
      integer id(nsiz)

c
      integer tops
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
      if(rhs.gt.2.or.rhs.lt.1) then
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
c
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
         ref=il.ne.ilr
         lr=sadr(ilr+4)

         if (m .eq. 1 .or. n .eq. 1) go to 63
c     
c     .  diag(a,k) with a a matrix
         if (k.ge.0) then
            mn=max(0,min(m,n-k))
         else
            mn=max(0,min(m+k,n))
         endif
         if(ref) then
            err=lr+mn*(it+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
         endif
         if(mn.eq.0) then
            istk(ilr)=1
            istk(ilr+1)=0
            istk(ilr+2)=0
            istk(ilr+3)=0
            lstk(top+1)=sadr(ilr+4)+1
            return
         else
            istk(ilr)=1
            istk(ilr+1)=mn
            istk(ilr+2)=1
            istk(ilr+3)=it
            lstk(top+1)=lr+istk(ilr+1)*(it+1)
            if(k.ge.0) then
               call unsfdcopy(mn,stk(l+k*m),m+1,stk(lr),1)
            else
               call unsfdcopy(mn,stk(l-k),m+1,stk(lr),1)
            endif
            if(it.eq.0) return
            if(k.ge.0) then
               call unsfdcopy(mn,stk(l+m*n+k*m),m+1,stk(lr+mn),1)
            else
               call unsfdcopy(mn,stk(l+m*n-k),m+1,stk(lr+mn),1)
            endif
         endif
         return

c     .  diag(vector,k)
 63      nn = max(m,n)+abs(k)
         l1=lr+nn*nn*(it+1)
         err = l1 + mn*(it+1) -  lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(ilr)=1
         istk(ilr+1)=nn
         istk(ilr+2)=nn
         istk(ilr+3)=it
         lstk(top+1)=lr+nn*nn*(it+1)
         if(ref) then
            l1=l
         else
            call unsfdcopy(mn*(it+1),stk(l),-1,stk(l1),-1)
         endif
         call dset(nn*nn*(it+1),0.0d+0,stk(lr),1)
         if(k.ge.0) then
            call unsfdcopy(mn,stk(l1),1,stk(lr+nn*k),nn+1)
         else
            call unsfdcopy(mn,stk(l1),1,stk(lr-k),nn+1)
         endif
         if(it.eq.0) return
         if(k.ge.0) then
            call unsfdcopy(mn,stk(l1+mn),1,stk(lr+nn*nn+k*nn),nn+1)
         else
            call unsfdcopy(mn,stk(l1+mn),1,stk(lr+nn*nn-k),nn+1)
         endif
      elseif(istk(il).eq.2) then
c       *call* polelm
         fun=16
         fin=10
         top=tops
      else
         top=tops
         call funnam(ids(1,pt+1),'diag',il)
         fun=-1
      endif
      return
      end
c     -------------------------------
