c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      logical function chkvar(v1,v2)
c     compare polynomials formal names
      integer v1(4),v2(4)
      logical test
      
      if(v1(1).eq.0.or.v2(1).eq.0) then
         test=.true.
      else
         test=.false.
         do 10 i=1,4
            if(v1(i).ne.v2(i)) goto 20
 10      continue
         test=.true.
      endif
 20   chkvar=test
      return
      end
      
     
      
