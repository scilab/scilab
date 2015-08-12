c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      integer function uppertype(ita,itb)
      if(ita.eq.0.or.itb.eq.0) then
         it=0
      else
         it=min(int(ita/10),int(itb/10))*10+max(mod(ita,10),mod(itb,10))
      endif
      uppertype=it
      return
      end
