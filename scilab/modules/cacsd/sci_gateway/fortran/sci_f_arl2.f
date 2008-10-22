c     --------------------------

c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     --------------------------
c
      subroutine sciarl2(fname)
c     ----------------------------
      character*(*) fname
      include 'stack.h'
      integer iadr,sadr
c
      integer vol1,vol2,vol3,var(4)
      double precision v,eps,errl2,phi,gnrm
      logical all
      common/no2f/gnrm
      common/arl2c/info,ierr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      
            if(top+lhs-rhs.ge.bot) then
         call error(18)
         return
      endif
      if(rhs.le.0) then
         call error(39)
         return
      endif
c
      eps=stk(leps)
c
      lw=lstk(top+1)
c
      il1=iadr(lstk(top+1-rhs))
      if(istk(il1).gt.2) then
         err=1
         call error(44)
         return
      endif
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      mn1=m1*n1
      if(istk(il1).eq.1) goto 01
      id1=il1+8
      vol1=istk(id1+mn1)-1
      l1r=sadr(id1+mn1+1)
      l1i=l1r+vol1
      goto 05
   01 id1=iadr(lw)
      l1r=sadr(il1+4)
      l1i=l1r+mn1
      lw=sadr(id1+mn1+1)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      do 02 i=1,mn1
   02 istk(id1+i-1)=i
      istk(id1+mn1)=mn1+1
      vol1=mn1
c
   05       all=.false.
      il=iadr(lstk(top))
      if(istk(il).eq.10) then
         all=.true.
         top=top-1
         rhs=rhs-1
      endif

      if(rhs.lt.3.or.rhs.gt.4) then
         call error(39)
         return
      endif
c
      info=0
      if(rhs.eq.4) then
c     info parameter

           il=iadr(lstk(top))
           if(istk(il).ne.1) then
              err=4
              call error(53)
              return
           endif
           l=sadr(il+4)
           if(stk(l).lt.0.0d+0) then
              err=4
              call error(36)
              return
           endif
           info=int(stk(l))
           top=top-1
      endif
c
c     Final degre parameter
      il=iadr(lstk(top))
      if(istk(il).ne.1) then
         err=3
         call error(53)
         return
      endif
      l=sadr(il+4)
      if(stk(l).lt.1.0d+0) then
         err=3
         call error(36)
         return
      endif
      itmax=int(stk(l))
c
c     Initial point parameter 
c     (for arl2a it only gives le formal variable name)
      top=top-1
      lw=lstk(top+1)
      ild=iadr(lstk(top))
      if(istk(ild).ne.2) then
         err=2
         call error(54)
         return
      endif
      if(istk(ild+3).ne.0) then
         err=2
         call error(52)
         return
      endif
      if(istk(ild+1)*istk(ild+2).ne.1) then
         err=2
         call error(36)
         return
      endif
      call icopy(4,istk(ild+4),1,var,1)
      nd=istk(ild+9)-2
      ld=sadr(ild+10)
      call idegre(stk(ld),istk(ild+9)-2,nd)
      call dscal(nd+1,1.0d+0/stk(ld+nd),stk(ld),1)
c
c     fourier coefficients parameters
      if(it1.ne.0) then
         err=1
         call error(52)
         return
      endif
      nf=mn1
      if(istk(il1).eq.2) then
         if(mn1.ne.1) then
            err=1
            call error(43)
            return
         endif
         nf=istk(id1+mn1)-1
      endif
      lf=l1r
      ilf=il1
c
      if(all) goto 82
c
c     look for a solution
      ln=ld+itmax+1
      lw=ln+itmax+1
      ng=nf-1
      lww=lw+32+32*itmax+7*ng+itmax*ng+itmax**2*(ng+2)
      ilw=iadr(lww)
      lww=sadr(ilw+ 29+itmax*itmax+4*itmax)
      err=lww-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      call arl2(stk(lf),nf,stk(ln),stk(ld),nd,itmax,errl2,stk(lw),
     1       istk(ilw),info,ierr,wte)
      if(ierr.ne.0) then
         if(ierr.eq.3) then
            call msgs(50,0)
         else if(ierr.eq.4) then
c     Impossible to reach required order
            call msgs(51,0)
         else if(ierr.eq.5) then
c     failure when looking for the intersection with domains boundaries
            call msgs(52,0)
         else if(ierr.eq.7) then
c     too many solutions found
            buf='arl2 : too many solutions found'
            call error(999)
            return
         endif
      endif
      call icopy(4,var,1,istk(ilf+4),1)
      istk(ilf+8)=1
      istk(ilf+9)=1+itmax+1
      l=sadr(ilf+10)
      call unsfdcopy(itmax+1,stk(ld),1,stk(l),1)
      istk(ilf)=2
      istk(ilf+1)=1
      istk(ilf+2)=1
      l=l+itmax+1
      lstk(top)=l+1
      if(lhs.eq.1) goto 99
      il=iadr(lstk(top))
      istk(il)=2
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      call icopy(4,var,1,istk(il+4),1)
      istk(il+8)=1
      istk(il+9)=1+itmax
      l=sadr(il+10)
      call unsfdcopy(itmax,stk(ln),1,stk(l),1)
      lstk(top+1)=l+itmax
      if(lhs.eq.2) goto 99
c
      top=top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      l=sadr(il+4)
      stk(l)=errl2
      lstk(top+1)=l+1
      goto 99
c
 82   continue
c     look for "all" solutions
      top=top-1
      mxsol=20
      ld=lf+nf
      ng=nf-1
      lw=ld+mxsol*(itmax+1)
      lww=lw+34+34*itmax+7*ng+itmax*ng+itmax**2*(ng+2)+
     $     4*(itmax+1)*mxsol
      ilw=iadr(lww)
      lww=sadr(ilw+29+itmax**2+4*itmax+2*mxsol)
      err=lww-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      call arl2a(stk(lf),nf,stk(ld),mxsol,nsol,itmax,info,ierr,wte,
     $     stk(lw),istk(ilw))
      if(ierr.ne.0) then
         if(ierr.eq.3) then
            buf='arl2a : Loop on two orders detected'
         else if(ierr.eq.4) then
            buf='arl2a : Impossible to reach required order'
         else if(ierr.eq.5) then
            call basout(io,wte,'arl2: Failure when looking for'//
     &           'the intersection with domains bounds')
         else if(ierr.eq.7) then
            buf='arl2a : Too many solutions found'
         endif
         call error(999)
      endif
c
      l0=lstk(top)
      lw0=lww
      mv=lw0-l0
c
c     denominators
      ild=iadr(lww)
      istk(ild)=2
      istk(ild+1)=nsol
      istk(ild+2)=1
      istk(ild+3)=0
      call icopy(4,var,1,istk(ild+4),1)
      istk(ild+8)=1
      do 83 is=1,nsol
         istk(ild+8+is)=istk(ild+7+is)+itmax+1
 83   continue
      l=sadr(ild+9+nsol)
      ld0=l
      do 84 is=1,nsol
      call unsfdcopy(itmax,stk(ld-1+is),mxsol,stk(l),1)
      stk(l+itmax)=1.0d0
      l=l+itmax+1
 84   continue

      lww=l+1
      lstk(top+1)=lww-mv
      if(lhs.eq.1) goto 88
c
c     numerators
      top=top+1
      il=iadr(lww)
      istk(il)=2
      istk(il+1)=nsol
      istk(il+2)=1
      istk(il+3)=0
      call icopy(4,var,1,istk(il+4),1)
      istk(il+8)=1
      do 85 is=1,nsol
         istk(il+8+is)=istk(il+7+is)+itmax
 85   continue
      l=sadr(il+9+nsol)
      gnrm=sqrt(gnrm)
      l1=ld0
      do 86 is=1,nsol
         call lq(itmax,stk(l1),stk(l),stk(lf),ng)
         call dscal(itmax,gnrm,stk(l),1)
         l1=l1+itmax+1
         l=l+itmax
 86   continue
      lww=l
      lstk(top+1)=lww-mv
c
      if(lhs.eq.2) goto 88
c     errors
      top=top+1
      il=iadr(lww)
      istk(il)=1
      istk(il+1)=nsol
      istk(il+2)=1
      istk(il+3)=0
      l=sadr(il+4)
      l1=ld0
      do 87 i=1,nsol
         stk(l)=sqrt(phi(stk(l1),itmax,stk(lf),ng,stk(l+nsol)))*gnrm
         l1=l1+itmax+1
         l=l+1
 87   continue
      lww=l
      lstk(top+1)=lww-mv
      goto 88
 88   continue
c     Putting in order the stack
      call unsfdcopy(lww-lw0,stk(lw0),1,stk(l0),1)
c
      
   99 return
      end
c     ----------------------------
      