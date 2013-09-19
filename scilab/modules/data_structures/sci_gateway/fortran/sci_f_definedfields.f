c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine definedfields
c     returns index of defined fields of a list
      include 'stack.h'
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c
      lw=lstk(top+1)
      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).lt.15.or.istk(il).gt.17) then
         err=1
         call error(44)
         return
      endif
      n=istk(il+1)

      err=lw+n-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      il=il+2
      l=lw
      do 10 i=1,n
         if(istk(il+1)-istk(il).gt.0) then
            stk(l)=i
            l=l+1
         endif
         il=il+1
 10   continue
      n=l-lw
      l=sadr(ilr+4)
      if(n.gt.0) then
         if(l.lt.lw) then
            call unsfdcopy(n,stk(lw),1,stk(l),1)
         else
            call unsfdcopy(n,stk(lw),-1,stk(l),-1)
         endif
      endif
      istk(ilr)=1
      istk(ilr+1)=min(1,n)
      istk(ilr+2)=n
      istk(ilr+3)=0
      lstk(top+1)=l+n
      return
      end

