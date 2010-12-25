c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intcoeff(id)
      INCLUDE 'stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer v2,vol,top0
      logical ref
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1.and.rhs.ne.2) then
         call error(42)
         return
      endif
      top0=top
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr
      if(istk(il1).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'coeff',il1)
         return
      endif

      if(rhs.eq.2) then
         ilv=iadr(lstk(top))
         if(istk(ilv).lt.0) ilv=iadr(istk(ilv+1))
         if(istk(ilv).ne.1) then
            err=2
            call error(53)
            return
         endif
         if(istk(ilv+3).ne.0) then
            err=2
            call error(52)
            return
         endif
         nv=istk(ilv+1)*istk(ilv+2)
         if(nv.eq.0) then
            top=top-1
            istk(ilr)=1
            istk(ilr+1)=0
            istk(ilr+2)=0
            istk(ilr+3)=0
            lstk(top+1)=sadr(ilr+4)
            return
         endif
         if(istk(ilv+1).ne.1.and.istk(ilv+2).ne.1) then
            err=2
            call error(89)
            return
         endif
         lv=sadr(ilv+4)
         do 10 i=0,nv-1
            if(int(stk(lv+i)).lt.0) then
               err=2
               call error(116)
               return
            endif
 10      continue
         top=top-1
      endif
c
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      mn1=m1*n1
c
      if(ref) then
         lr=sadr(ilr+4)
      else
         lr=lstk(top+1)
      endif
c
      if(istk(il1).eq.2) then
c     coeff of a matrix of polynomials
         id1=il1+8
         l1=sadr(id1+mn1+1)
         vol=istk(id1+mn1)-1

         if(rhs.eq.1) then
            mx=0
            do 20 ij=1,mn1
               nij=istk(id1+ij)-istk(id1+ij-1)
               if(nij.gt.mx) mx=nij
 20         continue
            v2=mn1*mx
            err=lr+v2*(it1+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dset(v2*(it1+1),0.0d+0,stk(lr),1)
            do 30 ij=1,mn1
               nij=istk(id1+ij)-istk(id1+ij-1)
               lij=l1-1+istk(id1+ij-1)
               call unsfdcopy(nij,stk(lij),1,stk(lr-1+ij),mn1)
               if(it1.eq.1) then
                  call unsfdcopy(nij,stk(lij+vol),1,stk(lr+v2-1+ij),mn1)
               endif
 30         continue
            if(.not.ref) then
               l=sadr(ilr+4)
               call unsfdcopy(v2*(it1+1),stk(lr),1,stk(l),1)
               lr=l
            endif
            istk(ilr)=1
            istk(ilr+1)=m1
            istk(ilr+2)=n1*mx
            istk(ilr+3)=it1
            lstk(top+1)=lr+v2*(it1+1)
         else
            lw=max(lstk(top0+1),lr+mn1*nv*(it1+1))
            err=lw+nv-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(nv,stk(lv),1,stk(lw),1)
            lv=lw
            call dset(mn1*nv*(it1+1),0.0d+0,stk(lr),1)
            do 41 ij=1,mn1
               nij=istk(id1+ij)-istk(id1+ij-1)
               lij=l1-1+istk(id1+ij-1)
               do 40 k=1,nv
                  mv=int(stk(lv-1+k))
                  if(mv.ge.nij) goto 40
                  stk(lr+ij-1+(k-1)*mn1)=stk(lij+mv)
                  if(it1.ne.0) stk(lr+ij-1+(k-1+nv)*mn1)=stk(lij+mv+vol)
 40            continue
 41         continue
            l=sadr(ilr+4)
            if(.not.ref) call unsfdcopy(mn1*nv*(it1+1),stk(lr),1,
     $           stk(l),1)
            istk(ilr)=1
            istk(ilr+1)=m1
            istk(ilr+2)=n1*nv
            istk(ilr+3)=it1
            lstk(top+1)=l+mn1*nv*(it1+1)
         endif
      else
c     coeff of a matrix of numbers
         if(rhs.eq.1) then
c     .     return the input matrix
            if(ref) then
c     .      copy matrix on the top of the stack
               k=istk(ilr+2)
               err=lstk(top)+lstk(k+1)-lstk(k)-lstk(bot)
               if(err.gt.0) then
                  call error(17)
                  return
               endif
               call unsfdcopy(lstk(k+1)-lstk(k),stk(lstk(k)),1
     $              ,stk(lstk(top)),1)
               lstk(top+1)=lstk(top)+lstk(k+1)-lstk(k)
            endif
            return
         endif
         l1=sadr(il1+4)
         vol=mn1

         lw=lr+mn1*nv*(it1+1)
         err=lw+nv-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         if(lv.le.lw) then
            call unsfdcopy(nv,stk(lv),-1,stk(lw),-1)
            lv=lw
         endif
         call dset(mn1*nv*(it1+1),0.0d+0,stk(lr),1)
         do 50 k=1,nv
            if(stk(lv-1+k).eq.0.0d0) then
               call unsfdcopy(mn1,stk(l1),1,stk(lr+(k-1)*mn1),1)
               if(it1.eq.1) call unsfdcopy(mn1,stk(l1+mn1),1,
     $              stk(lr+(k-1)*mn1+mn1*nv),1)
            endif
 50      continue
         l=sadr(ilr+4)
         if(.not.ref) call unsfdcopy(mn1*nv*(it1+1),stk(lr),1,stk(l),1)
         istk(ilr)=1
         istk(ilr+1)=m1
         istk(ilr+2)=n1*nv
         istk(ilr+3)=it1
         lstk(top+1)=l+mn1*nv*(it1+1)
      endif

      return
      end

c			=======================================
