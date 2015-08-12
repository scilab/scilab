c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine fcalc(str,a)
      character*(*) str
      integer a
c
      if (str.eq.'one') then
         a=1
      elseif (str.eq.'two') then
         a=2
      else
         a=-1
      endif
      end

