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
      subroutine scildiv(fname)
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
   05 if(rhs.ne.3.or.lhs.ne.1) then
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
   
   99 return
      end
c     ----------------------------
