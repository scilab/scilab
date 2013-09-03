c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intlstsize
c     WARNING : argument of this interface may be passed by reference
      include 'stack.h'
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).lt.15.or.istk(il).gt.17) then
         err=1
         call error(56)
         return
      endif
      n=istk(il+1)

      if(lhs*rhs.ne.1) then
         err=1
         call error(39)
         return
      endif
      ilr=iadr(lstk(top))
      l=sadr(ilr+4)
      err=l+1-lstk(bot)
      if (err.gt.0) then
         call error(17)
         return
      endif
      istk(ilr)=1
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=0
      l=sadr(ilr+4)
      stk(l)=dble(n)
      lstk(top+1)=l+1
c
      return
      end
