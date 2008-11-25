c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
      subroutine inteye(id)
      INCLUDE 'stack.h'
      integer id(nsiz)

      integer tops
      double precision s
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (lhs .gt. 1) then
         call error(41)
         return
      endif
      if(rhs.gt.2) then
         call error(42)
         return
      endif

      tops=top
c
      if(rhs.le.0) then
c     eye sans argument
         top=top+1
         m=-1
         n=-1
      elseif(rhs.eq.1) then
         il=iadr(lstk(top))
         
         if(abs(istk(il)).gt.10.or.abs(istk(il)).eq.5.
     1    or.abs(istk(il)).eq.6) then
            call funnam(ids(1,pt+1),'eye',il)
            fun=-1
            return
         endif
         if(istk(il).lt.0) il=iadr(istk(il+1))
         m=istk(il+1)
         n=istk(il+2)
c     eye(matrice)
      elseif(rhs.eq.2) then
c     eye(m,n)
         call getdimfromvar(top,2,n)
         if(err.gt.0.or.err1.gt.0) return
         top=top-1
         call getdimfromvar(top,1,m)
         if(err.gt.0.or.err1.gt.0) return
      endif
c
      mn=m*n
      if(m.eq.0) n=0
      if(n.eq.0) m=0

      il=iadr(lstk(top))
      l=sadr(il+4)

c     to avoid integer overflow
      s=l+dble(m)*dble(n)- lstk(bot)
      if(s.gt.0.0d0) then
         err=s
         call error(17)
         return
      endif
      istk(il)=1
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=0
      lstk(top+1)=l+mn
      if(mn.ne.0) then
         m=abs(m)
         call dset(mn,0.0d+0,stk(l),1)
         call dset(min(m,abs(n)),1.0d+0,stk(l),m+1)
      endif
      return
      end
c     -------------------------------

