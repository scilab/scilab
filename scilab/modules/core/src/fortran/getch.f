c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine fortrangetch
c
c     get next character

      include 'stack.h'
c
      integer eol
      data eol/99/
c
      l = lpt(4)
      char1 = lin(l)
      if (char1 .ne. eol) lpt(4) = l + 1
      return
      end
