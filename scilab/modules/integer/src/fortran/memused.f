c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      integer function  memused(it,mn)
      integer mn,it
      if(it.ne.0) then
         memused=int((mn*mod(it,10))/4)+1
      else
         memused=mn*2
      endif
      end
