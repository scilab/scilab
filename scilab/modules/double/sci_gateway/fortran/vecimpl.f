
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine vecimpl
c     
c     implicit vector
c     
c     Copyright INRIA
      include 'stack.h'
c     
      double precision e1,e2,st,e1r,inf,npt,zero
      double precision dlamch
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      inf=dlamch('o')
      zero=0.0d0
      if(rhs.eq.2) goto 02
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=sadr(il3+4)
      mn3=m3*n3
      top=top-1

 02   il2=iadr(lstk(top))
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
      if(mn1.ne.1) then
         err=1
         call cvname(ids(1,pt+1),''':''',0)
         call error(204)
         return
      endif
      e1 = stk(l1)
c
      if(mn2.ne.1) then
         err=2
         call cvname(ids(1,pt+1),''':''',0)
         call error(204)
         return
      endif
      e2 = stk(l2)
c
      if (rhs .eq. 3) then
         if(mn3.ne.1) then
            err=3
            call cvname(ids(1,pt+1),''':''',0)
            call error(204)
            return
         endif
         e2=stk(l3)
         st = stk(l2)
      else
         st = 1.0d+0
      endif

C     If one value is NaN return NaN
      if(isanan(e1).eq.1.or.isanan(e2).eq.1.or.isanan(st).eq.1) then
         stk(l1)=e1+st+e2
         istk(il1+1)=1
         istk(il1+2)=1
         istk(il1+3)=0
         lstk(top+1)=l1+1
         return
      endif

C     empty answer cases
      if ((st.eq.0.0d0).or.
     +     (st.gt.0.0d0.and.e1.gt.e2).or.
     +     (st.lt.0.0d0.and.e1.lt.e2)) then
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=l1
         return
      endif
      npt=(e2-e1)/st
      if(abs(e1).ge.inf.or.abs(e2).ge.inf.or.abs(st).ge.inf) then
         stk(l1)=zero/zero
         istk(il1+1)=1
         istk(il1+2)=1
         istk(il1+3)=0
         lstk(top+1)=l1+1
       return
      endif

c     check for clause
      if (rstk(pt-1) .eq. 801.or.rstk(pt).eq.611) go to 54
c      if(rstk(pt-1).eq.611.and.rstk(pt).eq.601) go to 54
      if(rstk(pt-1).eq.611.and.rstk(pt).eq.601) then
c     .  in compiled macro, check if vecimpl is the latest opcode of the
c     .  for expression
         if (ids(3,pt)+ids(4,pt).eq.pstk(pt))go to 54
      endif
c




c     floating point used to avoid integer overflow
      e1r=dble(l1) + max(3.0d0,npt) - dble(lstk(bot))
      if (e1r .gt. 0.0d0) then
         err=e1r
         call error(17)
         return
      endif
c
      e1r=2.0d0*max(abs(e1),abs(e2))*dlamch('p')
      n = 0
      l=l1
 52   if (st*(stk(l)-e2).gt.0.0d+0) then
         if (abs(stk(l)-e2).lt.e1r) n=n+1
         go to 53
      endif
      n = n+1
      l = l+1
      stk(l) = e1 + dble(n)*st
      go to 52
 53   continue
      istk(il1+1)=1
      if(n.eq.0) istk(il1+1)=0
      istk(il1+2)=n
      istk(il1+3)=0
      lstk(top+1)=l1+n
      return
c     
c     for clause
 54   stk(l1) = e1
      stk(l1+1) = st
      stk(l1+2) = e2
      istk(il1+1)=-3
      istk(il1+2)=-1
      lstk(top+1)=l1+3
      return
      end
c			================================================
