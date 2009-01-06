c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine setlnb()
c     =======================================================
c     compilation info for line counter 
c     =======================================================
      include 'stack.h'
      logical ilog,compil
      if(err1.gt.0) return
      call whatln(lpt(1),lpt(2),lpt(6),nct,idebut,ifin)
      ilog=compil(16,lct(8)-nct-1,0,0,0)
      return
      end


