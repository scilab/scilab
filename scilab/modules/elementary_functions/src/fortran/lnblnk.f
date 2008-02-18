c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      integer function lnblnk(str)
c
      character*(*) str
      n=len(str)+1
 10   continue
      n=n-1
      if (n.eq.0) then
         lnblnk=0
         return
      else
         if (str(n:n).ne.' ') then
            lnblnk=n
            return
         endif
      endif
      goto 10
      end
