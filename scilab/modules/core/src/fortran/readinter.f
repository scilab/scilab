c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      function readinter(lunit,fmt)
c     interface for "file" gateway
        integer lunit, read_inter
        character*(*) fmt
c
        read(lunit, fmt, err=20, end=30)
c
        read_inter = 0
        return
        read_inter = 2
   20   return
        read_inter = 1
   30   return
      end
