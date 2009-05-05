c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2009 - DIGITEO - Allan CORNET
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine basouttofile(lunit,string)
      integer lunit
      character*(*) string
      write(lunit,'(a)') string      
      end
c     ====================================           
