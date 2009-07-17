c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intinttype
      include 'stack.h'
      logical checkrhs,checklhs
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(.not.checkrhs('inttype',1,1)) return
      if(.not.checklhs('inttype',1,1)) return

      il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).eq.8) then
         is=istk(il+3)
      elseif(istk(il).eq.1) then
         is=0
      else
         err=rhs
         call error(44)
         return
      endif
      istk(il)=1
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      l=sadr(il+4)
      stk(l)=is
      lstk(top+1)=l+1
      end
