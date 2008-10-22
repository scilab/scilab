
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

		  subroutine matcc
c     
c     [a;b]

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
c
      itr=max(it1,it2)
c
      if(n1.lt.0.or.n2.lt.0) then
         call error(14)
         return
      elseif(n2.eq.0) then
c     .  [a;[]]
         return
      elseif(n1.eq.0)then
c     .  [[];b]
         call unsfdcopy(lstk(top+2)-lstk(top+1),stk(lstk(top+1))
     $        ,1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(top+2)-lstk(top+1)
         return
      elseif(n1.ne.n2) then
         call error(6)
         return
      endif
      m=m1+m2
      mn=m*n1
      if(n1.eq.1.and.itr.eq.0) then
         call unsfdcopy(mn2,stk(l2),1,stk(l1+mn1),1)
         istk(il1+1)=m
         istk(il1+3)=itr
         lstk(top+1)=l1+mn*(itr+1)
         return
      endif
c      lw1=l1+(itr+1)*mn
      lw1=max(lw,l1+(itr+1)*mn)
      lw2=lw1+mn1*(it1+1)
      err=lw2+mn2*(it2+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(lw2),1)
      call unsfdcopy(mn1*(it1+1),stk(l1),1,stk(lw1),1)
c     
      if(itr.eq.1) call dset(mn,0.0d+0,stk(l1+(mn1+mn2)),1)
      call dmcopy(stk(lw1),m1,stk(l1),m,m1,n1)
      if(it1.eq.1) call dmcopy(stk(lw1+mn1),m1,stk(l1+mn),m
     $     ,m1,n1)
      call dmcopy(stk(lw2),m2,stk(l1+m1),m,m2,n1)
      if(it2.eq.1) call dmcopy(stk(lw2+mn2),m2,stk(l1+mn+m1)
     $     ,m,m2,n1)
      istk(il1+1)=m
      istk(il1+2)=n1
      istk(il1+3)=itr
      lstk(top+1)=sadr(il1+4)+mn*(itr+1)
      return
      end
c			================================================
