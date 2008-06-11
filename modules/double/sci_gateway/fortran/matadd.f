
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

	    subroutine matadd
c     
c     matrix addition

      include 'stack.h'
      common /mtlbc/ mmode
c     
      double precision cstr,csti,sr,si
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
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
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c
      itr=max(it1,it2)
c
      if (mn1.eq.0) then
         if (mmode.eq.1) then
c     .     Matlab like []+a=[]
         else
c     .     []+a=a
            call icopy(4,istk(il2),1,istk(il1),1)
            call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
            lstk(top+1)=l1+mn2*(it2+1)
         endif
      elseif (mn2.eq.0) then
         if (mmode.eq.1) then
c     .     Matlab like a+[]=[]
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
         else
c     .     a+[]=a
         endif
      elseif (m1 .lt. 0) then
c     .  eye+vector
         go to 40
      elseif (m2 .lt. 0) then
c     .  vector+eye
         go to 41
      elseif (mn2.eq.1) then
c     .   vector+const
         err=l1+mn1*(itr+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dadd(mn1,stk(l2),0,stk(l1),1)
         if(it2+2*it1.eq.1) call unsfdcopy(mn1,stk(l2+mn2),0,
     $        stk(l1+mn1),1)
         if(it1*it2.eq.1) call dadd(mn1,stk(l2+mn2),0,stk(l1+mn1),1)
         lstk(top+1)=l1+mn1*(itr+1)
         istk(il1+3)=itr
      elseif (mn1.eq.1) then
c     .  cst+vector
         err=l1+mn2*(itr+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         cstr=stk(l1)
         csti=stk(l1+1)
         call unsfdcopy((it2+1)*mn2,stk(l2),1,stk(l1),1)
         if(it1.eq.1.and.it2.eq.0) call dset(mn2,0.d0,stk(l1+mn2),1)
         call dadd(mn2,cstr,0,stk(l1),1)
         if(it1.eq.1) call dadd(mn2,csti,0,stk(l1+mn2),1)
         lstk(top+1)=l1+mn2*(itr+1)
         istk(il1+1)=m2
         istk(il1+2)=n2
         istk(il1+3)=itr
      else
c     .  vector+vector
         if (m1 .ne. m2.or.n1 .ne. n2) then
            call error(8)
            return
         endif
         err=l1+mn1*(itr+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dadd(mn1,stk(l2),1,stk(l1),1)
         if(it2+2*it1.eq.1) then
            call unsfdcopy(mn1,stk(l2+mn1),1,stk(l1+mn1),1)
         endif
         if(it1*it2.eq.1) then
            call dadd(mn1,stk(l2+mn1),1,stk(l1+mn1),1)
         endif
         lstk(top+1)=l1+mn1*(itr+1)
         istk(il1+3)=itr
      endif
      return
c     addition et soustraction d'un scalaire fois l'identite
 40   sr=stk(l1)
      si=0.0d+0
      if(it1.eq.1) si=stk(l1+1)
      call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
      m1=m2
      n1=n2
      m2=it2
      it2=it1
      it1=m2
      mn1=mn2
      goto 46
c     
 41   sr=stk(l2)
      si=0.0d0
      if(it2.eq.1) si = stk(l2+1)
      goto 46
c     
 46   err=l1+mn1*(itr+1) - lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      lstk(top+1)=l1+mn1*(itr+1)
      istk(il1+1)=m1
      istk(il1+2)=n1
      istk(il1+3)=itr
c     
      if(itr.eq.1.and.it1.eq.0) call dset(mn1,0.0d+0,stk(l1+mn1),1)
      m1=abs(m1)
      n1=abs(n1)
      do 47 i = 1, min(n1,m1)
         ll = l1 + (i-1)*(m1+1)
         stk(ll) = stk(ll)+sr
         if(itr.ne.0) stk(ll+mn1) = stk(ll+mn1)+si
 47   continue
      return 
      end
c			================================================
