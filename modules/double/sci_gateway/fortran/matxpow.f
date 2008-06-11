
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine matxpow
c     
c     matrix/vector entrywize power
c     
      include 'stack.h'
c     
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      lw=lstk(top+1)+1

      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=sadr(il2+4)
      mn2=m2*n2
      top=top-1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c

      if (mn1.eq.0) then
         return
      endif
      if (mn2.eq.0) then
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         return
      endif
      if (mn2 .gt. 1) then
         m=m2
         n=n2
         inc2=1
         if (mn1 .eq.1) then
            inc1=0
         elseif(m1.eq.m2.and.n1.eq.n2) then
            inc1=1
         else
            call error(30)
            return
         endif
      else
         inc2=0
         inc1=1
         m=m1
         n=n1
      endif
      mn=m*n
      err=lw+mn*2-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      itr=max(it1,it2)
      if(it2.eq.0) then
         if(it1.eq.0) then
            call ddpow1(mn,stk(l1),inc1,stk(l2),inc2,
     $           stk(lw),stk(lw+mn),1,ierr,itr)
         else
            call wdpow1(mn,stk(l1),stk(l1+mn1),inc1,stk(l2),inc2,
     $           stk(lw),stk(lw+mn),1,ierr)
         endif
      else
         if(it1.eq.0) then
            call dwpow1(mn,stk(l1),inc1,stk(l2),stk(l2+mn2),inc2,
     &           stk(lw),stk(lw+mn),1,ierr)
         else
            call wwpow1(mn,stk(l1),stk(l1+mn1),inc1,stk(l2),stk(l2+mn2),
     &           inc2,stk(lw),stk(lw+mn),1,ierr)
         endif
      endif
      if(ierr.eq.1) then
         call error(30)
         return
      endif
      if(ierr.eq.2) then
         if(ieee.eq.0) then
            call error(27)
            return
         elseif(ieee.eq.1) then
            call msgs(63)
         endif
         err=0
      endif
      istk(il1+1)=m
      istk(il1+2)=n
      istk(il1+3)=itr
      call unsfdcopy(mn*(itr+1),stk(lw),1,stk(l1),1)
      lstk(top+1)=l1+mn*(itr+1)
      return
      end
c			================================================
