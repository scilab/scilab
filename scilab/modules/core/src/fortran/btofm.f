c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine btofm(x,ldx,nx,mx)
c    scilab to fortran matrix transfer
c
      include 'stack.h'
      integer iadr,sadr
      double precision x
      dimension x(*)
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      il=iadr(lstk(top))
      if(istk(il).ne.1) then
         err=1
         call error(53)
         return
      endif
      if(istk(il+3).eq.1) then
         err=1
         call error(82)
         return
      endif
      if(istk(il+1).ne.nx.or.istk(il+2).ne.mx) then
         err=1
         call error(98)
         return
      endif
      lx=sadr(il+4)
      call dmcopy(stk(lx),istk(il+1),x,ldx,nx,mx)
      top=top-1
      return
      end
