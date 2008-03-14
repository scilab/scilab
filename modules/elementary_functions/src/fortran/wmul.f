c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      subroutine wmul(ar,ai,br,bi,cr,ci)
c
      double precision ar,ai,br,bi,cr,ci,t
c     c = a*b
      t = ar*bi + ai*br
      cr = ar*br - ai*bi
      ci = t
      return
      end
