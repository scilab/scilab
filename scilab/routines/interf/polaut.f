      subroutine polaut
c ====================================================================
c      polynomial stuff
c ====================================================================
c
c     Copyright INRIA
      include '../stack.h'
      integer iadr,sadr
c
      integer vol1,vol2,vol3,var(4)
      double precision v,eps,errl2,phi,gnrm
      logical all
      common/no2f/gnrm
      common/arl2c/info,ierr
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) write(wte,1000) fin
 1000 format(1x,'polaut',i4)
c
c     functions/fin
c     1    2   3    4    5
c   arl2 resi ldiv     
c
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
   05 goto (80,20,40,99,99) fin
c
c residu
  20  continue
      if(rhs.ne.3.or.lhs.ne.1) then
         call error(39)
         return
      endif
c
      il2=iadr(lstk(top+2-rhs))
      if(istk(il2).gt.2) then
         err=1
         call error(54)
         return
      endif
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      mn2=m2*n2
      if(istk(il2).eq.1) goto 21
      id2=il2+8
      vol2=istk(id2+mn2)-1
      l2r=sadr(id2+mn2+1)
      l2i=l2r+vol2
      goto 25
   21 id2=iadr(lw)
      l2r=sadr(il2+4)
      l2i=l2r+mn2
      lw=sadr(id2+mn2+1)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      do 22 i=1,mn2
   22 istk(id2+i-1)=i
      istk(id2+mn2)=mn2+1
      vol2=mn2
c
   25 il3=iadr(lstk(top+3-rhs))
      if(istk(il3).gt.2) then
         err=3
         call error(54)
         return
      endif
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      mn3=m3*n3
      if(istk(il3).eq.1) goto 26
      id3=il3+8
      vol3=istk(id3+mn3)-1
      l3r=sadr(id3+mn3+1)
      l3i=l3r+vol3
      goto 30
   26 id3=iadr(lw)
      l3r=sadr(il3+4)
      l3i=l3r+mn3
      lw=sadr(id3+mn3+1)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      do 27 i=1,mn3
   27 istk(id3+i-1)=i
      istk(id3+mn3)=mn3+1
      vol3=mn3
c
   30 if(m1.ne.m2.or.m1.ne.m3.or.n1.ne.n2.or.n1.ne.n3) then
         call error(60)
         return
      endif
      top=top+1-rhs
      if(it1+it2+it3.ne.0) goto 35
      lr=l1r
      do 31 k=1,mn1
      nd1=istk(id1+k)-istk(id1+k-1)
      nd2=istk(id2+k)-istk(id2+k-1)
      nd3=istk(id3+k)-istk(id3+k-1)
      nd11=nd1-1
      nd21=nd2-1
      nd31=nd3-1
      call residu(stk(l1r),nd11,stk(l2r),nd21,stk(l3r),nd31,v,
     1             eps,ierr)
      if(ierr.gt.0) then
         call error(27)
         return
      endif
      stk(lr)=v
      l1r=l1r+nd1
      l2r=l2r+nd2
      l3r=l3r+nd3
      lr=lr+1
   31 continue
      l1=sadr(il1+4)
      call unsfdcopy(mn1,stk(lr-mn1),1,stk(l1),1)
      istk(il1)=1
      lstk(top+1)=l1+mn1
      goto 99
c
   35 if(it1.eq.0) then
                       l1i=lw
                       lw=l1i+vol1
                       err=lw-lstk(bot)
                       if(err.gt.0) then
                          call error(17)
                          return
                       endif
                       call dset(vol1,0.0d+0,stk(l1i),1)
      endif
      if(it2.eq.0) then
                       l2i=lw
                       lw=l2i+vol2
                       err=lw-lstk(bot)
                       if(err.gt.0) then
                          call error(17)
                          return
                       endif
                       call dset(vol2,0.0d+0,stk(l2i),1)
      endif
      if(it3.eq.0) then
                       l3i=lw
                       lw=l3i+vol3
                       err=lw-lstk(bot)
                       if(err.gt.0) then
                          call error(17)
                          return
                       endif
                       call dset(vol3,0.0d+0,stk(l3i),1)
      endif
      lr=lw
      lw=lr+mn1*2
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      do 36 k=1,mn1
      nd1=istk(id1+k)-istk(id1+k-1)
      nd2=istk(id2+k)-istk(id2+k-1)
      nd3=istk(id3+k)-istk(id3+k-1)
      nd11=nd1-1
      nd21=nd2-1
      nd31=nd3-1
      call wesidu(stk(l1r),stk(l1i),nd11,stk(l2r),stk(l2i),nd21,
     &         stk(l3r),stk(l3i),nd31,stk(lr+k-1),stk(lr+mn1+k-1),
     &         eps,ierr)
      if(ierr.gt.0) then
         call error(27)
         return
      endif
      l1r=l1r+nd1
      l1i=l1i+nd1
      l2r=l2r+nd2
      l2i=l2i+nd2
      l3r=l3r+nd3
      l3i=l3i+nd3
   36 continue
      l1=sadr(il1+4)
      call unsfdcopy(mn1*2,stk(lr),1,stk(l1),1)
      istk(il1)=1
      istk(il1+1)=m1
      istk(il1+2)=n1
      istk(il1+3)=1
      lstk(top+1)=l1+mn1*2
      goto 99
c
c
c ldiv
  40  continue
      if(rhs.ne.3.or.lhs.ne.1) then
         call error(39)
         return
      endif
c
      il2=iadr(lstk(top+2-rhs))
      if(istk(il2).gt.2) then
         err=2
         call error(54)
         return
      endif
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      if(it2.ne.0) then
         err=2
         call error(52)
         return
      endif
      mn2=m2*n2
      if(istk(il2).eq.1) goto 41
      id2=il2+8
      vol2=istk(id2+mn2)-1
      l2r=sadr(id2+mn2+1)
      l2i=l2r+vol2
      goto 45
   41 id2=iadr(lw)
      l2r=sadr(il2+4)
      l2i=l2r+mn2
      lw=sadr(id2+mn2+1)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      do 42 i=1,mn2
   42 istk(id2+i-1)=i
      istk(id2+mn2)=mn2+1
      vol2=mn2
c
      if(m2.ne.m1.or.n2.ne.n1) then
         call error(60)
         return
      endif
      if(it1.ne.0) then
         err=1
         call error(52)
         return
      endif
c
   45 il3=iadr(lstk(top+3-rhs))
      if(istk(il3).ne.1) then
         err=3
         call error(53)
         return
      endif
      l3=sadr(il3+4)
      nmax=stk(l3)
c
      lr=lw
      err=lw+nmax*mn1-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      do 46 k=1,mn1
      nd1=istk(id1+k)-istk(id1+k-1)
      nd2=istk(id2+k)-istk(id2+k-1)
      call dtild(nd1,stk(l1r),1)
      call dtild(nd2,stk(l2r),1)
      call expan(stk(l2r),nd2,stk(l1r),nd1,stk(lr),nmax)
      l2r=l2r+nd2
      l1r=l1r+nd1
      lr=lr+nmax
   46 continue
      l1=sadr(il1+4)
      call unsfdcopy(nmax*mn1,stk(lw),1,stk(l1),1)
      istk(il1)=1
      istk(il1+1)=m1*nmax
      istk(il1+2)=n1
      top=top-rhs+lhs
      lstk(top+1)=l1+nmax*mn1
      goto 99
c
c arl2
   80 continue
      all=.false.
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
