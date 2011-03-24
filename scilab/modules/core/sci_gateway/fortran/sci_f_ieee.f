c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intsieee(fname)
c     --------------------------
      character*(*) fname
      logical checkrhs,checklhs
      include 'stack.h'
      logical cremat, getscalar

      integer iadr
c
      iadr(l)=l+l-1
C
      nbvars = 0
      rhs = max(0,rhs)
      if(.not.checkrhs(fname,0,1)) return
      if(.not.checklhs(fname,1,1)) return
      if(rhs.le.0) then
         top=top+1
         if(.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr)=ieee
      else
         if(.not.getscalar(fname,top,top,lr)) return
         i=stk(lr)
         if(i.lt.0.or.i.gt.2) then
            err=1
            call error(116)
            return
         endif
         ieee=i
         il=iadr(lstk(top))
         istk(il)=0
      endif
      end
      
