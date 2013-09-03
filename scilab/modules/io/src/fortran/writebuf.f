c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2007 - INRIA - Allan CORNET
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c     ====================================
c     requires by f2c :(
c     ====================================
      subroutine writebufio(buffer,line)
      
      character*(*) buffer
      integer  line
      write(buffer(1:4),'(i4)') line
      
      end
c     ====================================  
