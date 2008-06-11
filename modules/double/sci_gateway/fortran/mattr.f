
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine mattr

      include 'stack.h'
c     
      integer vol
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c
      if(mn1 .eq. 0.or.istk(il1).eq.0) then
         return
      elseif(abs(m1).eq.1.or.abs(n1).eq.1) then
         goto 10
      else
         vol=mn1*(it1+1)
         ll = l1+vol
         err = ll+vol - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         call unsfdcopy(vol,stk(l1),1,stk(ll),1)
         call mtran(stk(ll),m1,stk(l1),n1,m1,n1)
         if(it1.eq.1) then
            call mtran(stk(ll+mn1),m1,stk(l1+mn1),n1,m1,n1)
         endif
      endif
 10   istk(il1+1)=n1
      istk(il1+2)=m1
      return
      end
c			================================================
