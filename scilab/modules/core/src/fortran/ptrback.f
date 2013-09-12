c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine mvptr(topk,il)
C     just for dealing with lists 
      include 'stack.h'
      integer topk ,il
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /ptkeep/
cDEC$ ENDIF            
      common / ptkeep / lwk
      lwk= lstk(topk)
      lstk(topk) = il 
      return 
      end 

      subroutine ptrback(topk)
      integer topk 
      include 'stack.h'
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /ptkeep/
cDEC$ ENDIF            
      common / ptkeep / lwk
      lstk(topk)=lwk
      return 
      end 
