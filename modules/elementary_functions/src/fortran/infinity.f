c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      double precision function infinity(x)
c Purpose
c     Return infinity value	
c Calling sequence
c     a = infinity(0.0d)
c
      double precision x
      
      infinity = 1.0d0/x
      
      return 
      end      
