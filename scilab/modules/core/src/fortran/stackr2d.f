c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


      subroutine stackr2d(n,sl1,dl1) 
c     =============================
c     same as stacki2d but for reals 
c     to double 
c     ===============================
      include 'stack.h'
      integer n,sl1,dl1 
      integer iadr
      iadr(l)=l+l-1
      if((sl1+n-1).ge.iadr(dl1+n-1)) then
         call rea2db(n,istk(sl1),1,stk(dl1),1)
      elseif(sl1.le.iadr(dl1)) then
         call rea2db(n,istk(sl1),-1,stk(dl1),-1)
      else
         imont=sl1-iadr(dl1)
         ides=n-imont
         call rea2db(imont,istk(sl1),1,stk(dl1),1)
         call rea2db(ides,istk(sl1+imont),-1,stk(dl1+imont),-1)
      endif
      return
      end
c     ===============================

