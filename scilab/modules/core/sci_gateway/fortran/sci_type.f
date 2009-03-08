c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine inttype
      include 'stack.h'
      logical checkrhs,checklhs,cremat
      integer iadr
c    
      iadr(l)=l+l-1
c
      if(.not.checklhs('type',1,1)) return
      if(.not.checkrhs('type',1,1)) return


      il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      k=istk(il)
      if(.not.cremat('type',top,0,1,1,l,lc)) return
      stk(l)=dble(k)
      return
      end
      
