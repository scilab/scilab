c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine introots(id)
      INCLUDE 'stack.h'
      integer iadr, sadr
      integer id(nsiz)
      double precision t,sr,si
      integer vol
      integer fail
      logical ref,eigen
      integer eig
      data eig/14/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw=lstk(top+1)
      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1.and.rhs.ne.2) then
         call error(42)
         return
      endif
c
      eigen=.false.
      if (rhs.eq.2) then
         ilopt=iadr(lstk(top))
         if(istk(ilopt).lt.0) ilopt=iadr(istk(ilopt+1))
         if(istk(ilopt).ne.10) then
            err=2
            call error(55)
            return
         endif
         if(istk(ilopt+1).ne.1.or.istk(ilopt+2).ne.1) then
            err=2
            call error(89)
            return
         endif
         if(istk(ilopt+5).ne.2.or.istk(ilopt+6).ne.eig) then
            err=2
            call error(116)
            return
         endif
         eigen=.true.
         rhs=rhs-1
         top=top-1
      endif

      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr
      if(istk(il1).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'roots',il1)
         return
      endif
c
      m1=istk(il1+1)
      n1=istk(il1+2)
      mn1=m1*n1
      if(m1*n1.eq.0) return

      it1=istk(il1+3)

      if(istk(il1).eq.1) then
c     for Matlab compatibility root of the vector of coefficients
         vol=mn1
         l1=sadr(il1+4)

         n=mn1
         if(.not.ref) then
            call dtild(n*(it1+1),stk(l1),1)
            lc=l1
         else
            lc=lw
            lw=lc+n*(it1+1)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dcopy(n,stk(l1),1,stk(lc),-1)
            if(it1.eq.1)  call dcopy(n,stk(l1+n),1,stk(lc+n),-1)
         endif
      else
         if(mn1.ne.1) then
            err=1
            call error(43)
            return
         endif

         if(istk(il1).lt.2) goto 24

         id1=il1+8
         lc=sadr(id1+mn1+1)
         vol=istk(id1+mn1)-1
         call icopy(4,istk(il1+4),1,id,1)
         n=vol
      endif
      l1=sadr(ilr+4)
 21   n=n-1
      if(n.lt.0) goto 24
      t=abs(stk(lc+n))
      if(it1.eq.1) t=t+abs(stk(lc+n+vol))
      if(t.eq.0.0d+0) goto 21

      if (eigen) goto 22
c
c     real polynomial: rpoly algorithm
c     this alg is much more speedy, but it may happens that it gives
C     erroneous results without messages : example
C     roots(%s^31-8*%s^30+9*%s^29+0.995) should have two real roots near
C     1.355 and 6.65 and the other ones inside a circle centered in 0
C     with radius 1
C

      if(it1.eq.0.and.n.le.100) then
         lp=max(lw,l1+2*n)
         err=lp+n+1-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(n+1,stk(lc),1,stk(lp),1)
         call dtild(n+1,stk(lp),1)
         call rpoly(stk(lp),n,stk(l1),stk(l1+n),fail)
         if(fail.eq.1) then
            call error(24)
            return
         elseif(fail.eq.2) then
            call error(74)
            return
         elseif(fail.eq.3) then
            call error(75)
            return

         endif
         istk(ilr)=1
         istk(ilr+1)=n
         istk(ilr+2)=1
         if(n.eq.0) istk(ilr+2)=0
         istk(ilr+3)=1
         lstk(top+1)=l1+2*n
         goto 999
      endif

 22   continue
c
c     Companion matrix method
      lw=lw+n*n*(it1+1)
      err=lw+n*(it1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      sr=stk(lc+n)
      call unsfdcopy(n,stk(lc),-1,stk(lw),1)
      if(it1.eq.0) then
         call dscal(n,-1.0d+0/sr,stk(lw),1)
      else
         si=stk(lc+vol+n)
         t=sr*sr+si*si
         sr=-sr/t
         si=si/t
         call unsfdcopy(n,stk(lc+vol),-1,stk(lw+n),1)
         call wscal(n,sr,si,stk(lw),stk(lw+n),1)
      endif
      call dset(n*n*(it1+1),0.0d+0,stk(l1),1)
      call dset(n-1,1.0d+0,stk(l1+n),n+1)
      call unsfdcopy(n,stk(lw),1,stk(l1),1)
      if(it1.eq.1) call unsfdcopy(n,stk(lw+n),1,stk(l1+n*n),1)
      lstk(top+1)=l1+n*n*(it1+1)
      istk(ilr)=1
      istk(ilr+1)=n
      istk(ilr+2)=n
      istk(ilr+3)=it1
      fin=3
      fun=2
c     *call* matds(r c)
      goto 999
c     polynome de degre 0
 24   istk(ilr)=1
      istk(ilr+1)=0
      istk(ilr+2)=0
      istk(ilr+3)=0
      lstk(top+1)=sadr(ilr+4)
      goto 999
 999  return
      end
c			=======================================
