c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intludel(id)
      include 'stack.h'
      integer id(nsiz),top0,hand
      integer iadr, sadr

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs

      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c     checking variable fmat (number 1)
c     
      il1 = iadr(lstk(top-rhs+1))

      if (istk(il1) .ne. 128) then
         err=1
         call error(218)
         return
      endif
      l1 = sadr(il1+4)
      hand=stk(l1)
      call ludel1(hand,ierr)
      if (ierr.ne.0) then
         err=1
         call error(247)
         return
      endif

      if (err .gt. 0) return
c     
      top=top-rhs
c     no output variable
      top=top+1
      il=iadr(lstk(top))
      istk(il)=0
      lstk(top+1)=lstk(top)+1
      return
      end
c			======================================
