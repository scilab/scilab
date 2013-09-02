c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intprint
      INCLUDE 'stack.h'
c
      integer top2,tops
      integer mode(2)
      logical opened
      integer iadr
c
      save opened,lunit
c
c
      iadr(l)=l+l-1
c
      if(rhs.le.1) then
         call error(42)
         return
      endif
      if(lhs.gt.1) then
         call error(41)
         return
      endif

c     opening file
      top2 = top
      top = top-rhs+1
      il=iadr(lstk(top))
      mode(1)=0
      mode(2)=0
      call v2unit(top,mode,lunit,opened,ierr)
      if(ierr.gt.0) return
c
c     try to write on stdin
      if(lunit.eq.5) then
         call error(49)
         return
      endif
c
      l = lct(2)
      if(lunit.ne.wte) then
         lct(2) = 0
      endif
      top=top2
      do 26 i=2,rhs
         tops=top
         call print(idstk(1,top),tops,lunit)
         top=top-1
 26   continue
      lct(2) = l
      istk(il)=0
      if(.not.opened) then
         mode(1)=0
         mode(2)=0
         call clunit(-lunit,buf,mode)
      endif
      go to 999
 999  return
      end
