c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intsimp(id)
      INCLUDE 'stack.h'

Cc (DLL Import Intel Fortran)     
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /csimp/
cDEC$ ENDIF      
      integer id(nsiz)
      integer simpmd
      common/csimp/ simpmd
c
      if(simpmd.eq.0) then
         call ref2val()
         return
      endif
      if(rhs.eq.1) then
         call intrsimp(id)
      elseif(rhs.eq.2) then
         call intpsimp(id)
      else
         call error(39)
      endif
c
      return
      end
c			=======================================
