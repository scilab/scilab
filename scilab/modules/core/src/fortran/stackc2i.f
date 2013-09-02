c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine stackc2i(n,sl1,il1) 
c     =============================
c     same as stacki2d but for characters
c     to integer and overlapping region 
c     ===============================
      include 'stack.h'
      integer n,sl1,il1
      integer cadr
      cadr(l)=l+l+l+l-3      
      if((sl1+n-1).ge.cadr(il1+n-1)) then
         call cvstr(n,istk(il1),cstk(sl1:sl1+n),0)
      elseif( sl1.le.cadr(il1)) then
         call cvstr1(n,istk(il1),cstk(sl1:sl1+n),0)
      else
         kh=(sl1-cadr(il1))/3
         kb=n-kh
         call cvstr(kh,istk(il1),cstk(sl1:sl1+kh),0)
         lsuite=sl1+kh
         call cvstr1(kb,istk(il1+kh),cstk(lsuite:lsuite+kb),0)
      endif
      return
      end
c     ===============================

