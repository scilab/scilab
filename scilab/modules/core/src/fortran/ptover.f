c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      logical function ptover(inc,icheck)
      integer inc,icheck
C     increment pt with inc and 
C     checks overflow in pt counter 
C     returns true if overflow
      include 'stack.h'
      pt=pt+inc 
      if(pt.gt.icheck) then
         call error(26)
         ptover=.true.
      else
         ptover=.false.
      endif
      return
      end


      logical function eptover(inc,icheck)
      integer inc,icheck
C     like ptover but returns true 
C     if both pt overflows and err is present 
      include 'stack.h'
      pt=pt+inc 
      if(pt.gt.icheck) then
         call error(26)
         if ( err.gt.0) eptover=.true.
      else
         eptover=.false.
      endif
      return
      end
