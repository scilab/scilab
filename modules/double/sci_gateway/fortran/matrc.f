
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine matrc
c     
c     [a b]

      include 'stack.h'
c     
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      lw=lstk(top+1)+1
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

      itr=max(it1,it2)

      if(m1.lt.0.or.m2.lt.0) then
         call error(14)
         return
      endif
      if(m2.eq.0) then
         return
      elseif(m1.eq.0)then
         call unsfdcopy(lstk(top+2)-lstk(top+1),stk(lstk(top+1))
     $        ,1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(top+2)-lstk(top+1)
         return
      elseif(m1.ne.m2) then
         call error(5)
         return
      endif
c     
      if(itr.eq.0) then
         call unsfdcopy(mn2,stk(l2),1,stk(l1+mn1),1)
      else
         lw=l1+(itr+1)*(mn1+mn2)
         if(lw.gt.l2) then
            err=lw+mn2*(it2+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(mn2*(it2+1),stk(l2),-1,stk(lw),-1)
            l2=lw
         endif
         if(it1.eq.1) call unsfdcopy(mn1,stk(l1+mn1),-1,stk(l1
     $        +mn1+mn2),-1)
         call unsfdcopy(mn2,stk(l2),1,stk(l1+mn1),1)
         if(it1.eq.0) then
            call dset(mn1,0.0d+0,stk(l1+mn1+mn2),1)
            call unsfdcopy(mn2,stk(l2+mn2),1,stk(l1+2*mn1+mn2),1)
         endif
         if(it2.eq.0) then
            call dset(mn2,0.0d+0,stk(l1+2*mn1+mn2),1)
         else
            call unsfdcopy(mn2,stk(l2+mn2),1,stk(l1+2*mn1+mn2),1)
         endif
      endif
      n=n1+n2
      istk(il1+1)=m1
      istk(il1+2)=n
      istk(il1+3)=itr
      lstk(top+1)=sadr(il1+4)+m1*n*(itr+1)
      return
      end
c			================================================
