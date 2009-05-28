c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intsfact(id)
      INCLUDE 'stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vol
      logical ref

c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      maxit=100
      lw=lstk(top+1)

      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1) then
         call error(42)
         return
      endif

      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1
      if(istk(il1).ne.2) then
         fun=-1
         call funnam(ids(1,pt+1),'sfact',il1)
         return
      endif
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      if(it1.ne.0) then
         err=1
         call error(52)
         return
      endif
      mn1=m1*n1
      id1=il1+8
      l1=sadr(id1+mn1+1)
      lr=l1
      vol=istk(id1+mn1)-1

      if(mn1.eq.1) then
c
c     sfact of a polynomial
         if(ref) then
c     . sfact1 modifies its input argument
            lr=sadr(ilr+8+mn1+1)
            lstk(top+1)=lr+vol
            err=lstk(top+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(8+mn1+1,istk(il1),1,istk(ilr),1)
            call unsfdcopy(vol,stk(l1),1,stk(lr),1)
            lstk(top+1)=lr+vol
         endif
c
c     .  check symmetry
         n1=istk(id1+1)-2
         if (2*int(n1/2).ne.n1) then
            call error(88)
            return
         endif
         n=1+n1/2
         do 81 i=0,n-1
            if(stk(lr+i).ne.stk(lr+n1-i)) then
               call error(88)
               return
            endif
 81      continue
c
         lw=lstk(top+1)
         err=lw+6*n-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call sfact1(stk(lr),n-1,stk(lw),maxit,ierr)
         if(ierr.eq.2) then
            call writebufstfacta(buf,n-1)
            call error(999)
            return
         else if(ierr.eq.1) then
            call error(24)
            return
         else if(ierr.lt.0) then
c     convergence incomplete
            call writebufstfacta(buf,ierr)
            call msgs(22,0)
         endif
         lstk(top+1)=lr+n
         istk(ilr+8+1)=n+1
      else
c     multivariable case
         if(m1.ne.n1) then
            err=1
            call error(20)
            return
         endif
c     .  convert matrix of polynomials to a polynomial matrix
         n1=0
         do 87 i=1,mn1
            n1=max(n1,istk(id1+i)-istk(id1+i-1))
 87      continue
c
         n1=1+(n1-1)/2
         l2=lstk(top+1)
         if(ref) l2=l2+mn1+9+mn1*n1
         lw=l2+mn1*n1
         err=lw+sadr((n1+1)*m1*((n1+1)*m1)+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
c
         call dset(mn1*n1,0.0d+0,stk(l2),1)
         do 88 i1=0,mn1-1
            lij=l1-1+istk(id1+i1)
            mij=2+istk(id1+i1+1)-istk(id1+i1)-1-n1
            if(mij.gt.0) call unsfdcopy(mij,stk(lij+n1-1),1,
     $           stk(l2+i1),mn1)
 88      continue
c
         maxit=maxit+n1
         call sfact2(stk(l2),m1,n1-1,stk(lw),maxit,ierr)
         if(ierr.lt.0) then
            call error(24)
            return
         endif
         if(ierr.gt.0) then
            call error(88)
            return
         endif
c     .  convert polynomial matrix to matrix of polynomials
         id1=ilr+8
         lr=sadr(id1+mn1+1)
         l1=lr
         do 89 i=0,mn1-1
            call unsfdcopy(n1,stk(l2+i),mn1,stk(l1),1)
            l1=l1+n1
 89      continue
         do 90 i=0,mn1
            istk(id1+i)=1+n1*i
 90      continue
         if(ref) call icopy(8,istk(il1),1,istk(ilr),1)

         lstk(top+1)=l1
      endif
      return
      end
c			=======================================
