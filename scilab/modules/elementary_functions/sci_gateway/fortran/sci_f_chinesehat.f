c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine intchinesehat(id)
      INCLUDE 'stack.h'
      integer id(nsiz)

      integer iadr
c
      iadr(l)=l+l-1
      
      fun=-1
      il=iadr(lstk(top-rhs+1))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      call funnam(ids(1,pt+1),'pow',il)
      
      return
      end
c     -------------------------------
