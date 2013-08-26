c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine seteol
c     =====================================
c     insert eol code in compiled structure 
c     ======================================
      include 'stack.h'
      logical compil,ilog
      if(err1.le.0) then
         ilog=compil(15,0,0,0,0)
         if(comp(3).eq.2) then
c     .     set profile info
            ilog=compil(25,0,0,0,0)
         endif
      endif
      return
      end
