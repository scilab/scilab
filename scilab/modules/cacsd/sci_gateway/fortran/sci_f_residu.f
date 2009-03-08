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
      subroutine sciresidu(fname)
c     ----------------------------
      character*(*) fname
      include 'stack.h'
      integer iadr,sadr
c
      integer vol1,vol2,vol3
      double precision v,eps,gnrm
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
      
   99 return
      end
c     ----------------------------
