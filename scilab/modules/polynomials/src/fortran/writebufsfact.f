c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2009 - DIGITEO - Allan CORNET
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c     ====================================
c     requires by f2c :(
c     ====================================
      subroutine writebufstfacta(buffer,line)
      
      character*(*) buffer
      integer  line
      write(buffer,82) line
 82         format('No real solution: degree ',i2,' entry is negative!')
      end
c     ====================================  
      subroutine writebufstfactb(buffer,line)
      
      character*(*) buffer
      integer  line
      write(buffer(1:4),'(i3)') line
      end
c     ====================================  
