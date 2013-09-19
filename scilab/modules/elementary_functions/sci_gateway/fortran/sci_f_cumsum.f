c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine intcumsum(id)
c     WARNING : argument of this interface may be passed by reference
      INCLUDE 'stack.h'
      integer id(nsiz)
      logical ref
      integer mtlbsel
      integer sel,tops,type
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(rhs.gt.3) then
         call error(42)
         return
      endif
      if(lhs.ne.1) then
         call error(41)
         return
      endif
c
      sel=0
      tops=top
c
      il0=iadr(lstk(tops-rhs+1))
      ilr=il0
      if(istk(il0).lt.0) il0=iadr(istk(il0+1))
      ref=ilr.ne.il0
c
      if(istk(il0).ne.1) then
         top=tops
         call funnam(ids(1,pt+1),'cumsum',il0)
         fun=-1
         return
      endif

      if(rhs.eq.3) then
        top=tops-1
        rhs=rhs-1
      endif

c     standard matrix case
      call  orientandtype(sel,type)
      if (err.gt.0.or.err1.gt.0) return
      if(sel.eq.-1) sel=mtlbsel(istk(il0+1),2)

      m=istk(il0+1)
      n=istk(il0+2)
      it=istk(il0+3)
      l=sadr(il0+4)
      l1=sadr(ilr+4)
      mn=m*n

c
      if(ref) then
         err=l1+mn*(it+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(4,istk(il0),1,istk(ilr),1)
         call unsfdcopy(mn*(it+1),stk(l),1,stk(l1),1)
      endif
      lstk(top+1)=l1+mn*(it+1)
      if(mn.eq.0) return


      if(sel.eq.0) then
c     op(a) <=> op(a,'*')
         call cusum(mn,stk(l1))
         if(it.eq.1) call cusum(mn,stk(l1+mn))
      elseif(sel.eq.1) then
c     op(a,'r')  <=>  op(a,1)
         do 10 k=0,n-1
            call cusum(m,stk(l1+k*m))
 10      continue
         if(it.eq.1) then
            do 11 k=0,n-1
               call cusum(m,stk(l1+k*m+mn))
 11         continue
         endif
      elseif(sel.eq.2) then
c     op(a,'c')   <=>  op(a,2)
         kk=0
         do 20 k=1,n-1
	    call dadd(m,stk(l1+kk),1,stk(l1+kk+m),1)
            kk=kk+m
 20      continue
         if(it.eq.1) then
            kk=0
            do 21 k=1,n-1
               call dadd(m,stk(l1+mn+kk),1,stk(l1+kk+m+mn),1)
               kk=kk+m
 21         continue
         endif
      endif
      end
c     -------------------------------
