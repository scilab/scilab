
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine vecmul
c     
c     vector multiplication .*
c     
c     Copyright INRIA
      include 'stack.h'
c     
      double precision sr,si
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
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c
      itr=max(it1,it2)
c
      i1=1
      i2=1
      if(mn1.eq.0.or.mn2.eq.0) then
c     [].*a     a.*[]  -->[]
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         return
      endif
      if(n1.lt.0.and.mn2.ne.1.or.n2.lt.0.and.mn1.ne.1) then
         call error(14)
         return
      endif
      if(mn1.ne.1.and.mn2.ne.1) then
c       check dimensions
         if (m1.ne.m2 .or. n1.ne.n2) then
            buf='inconsistent element-wise operation'
            call error(9999)
            return
         endif
      endif
c
      lstk(top+1)=l1+max(mn1,mn2)*(itr+1)
      err=lstk(top+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(il1+3)=itr


      if(mn1.eq.1) then
c     .  cst.*a
         sr = stk(l1)
         si=0.0d+0
         if(it1.eq.1) si = stk(l1+1)
         if (m1.lt.0) then
            if(mn2.eq.1) then
c     .     eye.*cst
               istk(il1+1)=m1
               istk(il1+2)=n1
               istk(il1+3)=itr
            else
               call error(14)
               return
            endif
         else
            istk(il1+1)=m2
            istk(il1+2)=n2
            istk(il1+3)=itr
         endif
         call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
         it21=it2+2*it1
         if(it21.eq.0) then
c     .     vector and cst are real
            call dscal(mn2,sr,stk(l1),1)
         elseif(it21.eq.1) then
c     .     complex vector, real cst
            call dscal(mn2,sr,stk(l1),1)
            call dscal(mn2,sr,stk(l1+mn2),1)
         elseif(it21.eq.2) then
c     .     real vector, complex cst
            lstk(top+1)=l1+mn2*(itr+1)
            err=lstk(top+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(mn2,stk(l1),1,stk(l1+mn2),1)
            call dscal(mn2,sr,stk(l1),1)
            call dscal(mn2,si,stk(l1+mn2),1)
         elseif(it21.eq.3) then
c     .     lvector and cst are complex
            call wscal(mn2,sr,si,stk(l1),stk(l1+mn2),1)
         endif
         lstk(top+1)=l1+mn2*(itr+1)
      elseif (mn2 .eq. 1) then
c     .  a.*cst
         if(m2.lt.0) then
            call error(14)
            return
         endif
         it21=it2+2*it1
         if(it21.eq.0) then
c     .     vector and cst are real
            call dscal(mn1,stk(l2),stk(l1),1)
         elseif(it21.eq.1) then
c     .     real vector, complex cst
            sr = stk(l2)
            si = stk(l2+1)
            lstk(top+1)=l1+mn1*(itr+1)
            err=lstk(top+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(mn1,stk(l1),1,stk(l1+mn1),1)
            call dscal(mn1,si,stk(l1+mn1),1)
            call dscal(mn1,sr,stk(l1),1)
            istk(il1+3)=itr
         elseif(it21.eq.2) then
c     .     complex vector, real cst
            sr = stk(l2)
            call dscal(mn1,sr,stk(l1),1)
            call dscal(mn1,sr,stk(l1+mn1),1)
         elseif(it21.eq.3) then
            sr = stk(l2)
            si = stk(l2+1)
c     .     vector and cst are complex
            call wscal(mn1,sr,si,stk(l1),stk(l1+mn1),1)
         endif
      else
c     .  vector.*vector
         if(it1*it2.ne.1) then
            if(it1.eq.1) call dvmul(mn1,stk(l2),i2,stk(l1
     $           +mn1),i1)
            if(it2.eq.1) call dvmul(mn1,stk(l1),i1,stk(l2
     $           +mn2),i2)
            call dvmul(mn1,stk(l2),i2,stk(l1),i1)
            if(it2.eq.1) call unsfdcopy(mn1,stk(l2+mn2),i2
     $           ,stk(l1+mn1),i1)
         else
            call wvmul(mn1,stk(l2),stk(l2+mn2),i2,stk(l1)
     $           ,stk(l1+mn1),i1)
         endif
      endif
      return
      end
c			================================================
