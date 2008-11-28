c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
			subroutine intcumprod(id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE 'stack.h'
      integer id(nsiz)
      integer mtlbsel
      integer sel,tops
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(rhs.gt.2) then
         call error(42)
         return
      endif
      if(lhs.ne.1) then
         call error(41)
         return
      endif
c
      tops=top
      sel=0
c
      il0=iadr(lstk(tops-rhs+1))
      ilr=il0
      if(istk(il0).lt.0) il0=iadr(istk(il0+1))
c
      if(istk(il0).ne.1) then
         top=tops
         call funnam(ids(1,pt+1),'cumprod',il0)
         fun=-1
         return
      endif
c     
c     standard matrix case
      if(rhs.eq.2) then
         call getorient(top,sel)
         if(err.gt.0) return
         top=top-1
         if(sel.eq.-1) sel=mtlbsel(istk(il0+1),2)
      endif

      m=istk(il0+1)
      n=istk(il0+2)
      it=istk(il0+3)
      l=sadr(il0+4)
      l1=l
      mn=m*n
c
      if(ilr.ne.il0) then
         err=sadr(ilr+4)+mn*(it+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(4,istk(il0),1,istk(ilr),1)
         l1=sadr(ilr+4)
         call unsfdcopy(mn*(it+1),stk(l),1,stk(l1),1)
         il0=ilr
         l=l1
      endif
      if(mn.eq.0) return

c

      lstk(top+1)=l1+mn*(it+1)
      if(sel.eq.0) then
c     op(a) <=> op(a,'*')
         if(it.eq.0) then
            call cupro(mn,stk(l1))
         else
            call cuproi(mn,stk(l1),stk(l1+mn))
         endif
      elseif(sel.eq.1) then
c     op(a,'r')  <=>  op(a,1)
         if(it.eq.0) then
            do 10 k=0,n-1
               call cupro(m,stk(l1+k*m))
 10         continue
         elseif(it.eq.1) then
            do 11 k=0,n-1
               call cuproi(m,stk(l1+k*m),stk(l1+k*m+mn))
 11         continue
         endif
      elseif(sel.eq.2) then
c     op(a,'c')   <=>  op(a,2)
         if(it.eq.0) then
            kk=0
            do 20 k=1,n-1
               call dvmul(m,stk(l1+kk),1,stk(l1+kk+m),1)
               kk=kk+m
 20         continue
         elseif(it.eq.1) then
            kk=0
            do 21 k=1,n-1
               call wvmul(m,stk(l1+kk),stk(l1+mn+kk),1,
     $              stk(l1+kk+m),stk(l1+kk+m+mn),1)
               kk=kk+m
 21         continue
         endif
      endif
      return
      end
c     -------------------------------
