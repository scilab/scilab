c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2006 - INRIA - Allan CORNET
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine writebufspa(buffer,line)
      
      character*(*) buffer
      character*(*) line
      
      write(buffer,'(''Error while reading line '',a5)') line
      
      end
c     ====================================      
      subroutine writebufspb(buffer,typrow,line)
      
      character*(*) buffer
      character*(*) typrow
      character*(*) line
      
      write(buffer,'(''Unknown row type '',a2,'' at line '',a5)')
     $ typrow,line
     
      end
c     ====================================      
      subroutine writebufspc(buffer,finval)
      
      character*(*) buffer
      integer finval
      
      write(buffer(1:4),'(i4)') finval
      
      end
c     ====================================      
