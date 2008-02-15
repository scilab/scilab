
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     ====================================
c     requires by f2c :(
c     ====================================
      subroutine writebufdgesv3(buffer,line)
      
      character*(*) buffer
      character*(*) line
      
      write(buffer(1:4),'(1pd13.4)') line
      
      end
c     ====================================      
      subroutine writebufdgesv4(buffer,line)
      
      character*(*) buffer
      character*(*) line
      
      write(buffer(1:13),'(1pd13.4)') line
      
      end
c     ====================================      
      subroutine writebufdgetri(buffer,rcond)
      
      character*(*) buffer
      double precision rcond
      
      write(buffer(1:13),'(1pd13.4)') rcond
      
      end
c     ====================================      
      subroutine writebufzgesv3(buffer,line)
      
      character*(*) buffer
      character*(*) line
      
      write(buffer(1:13),'(1pd13.4)') line
      
      end
c     ====================================      
      subroutine writebufzgesv4(buffer,line)
      
      character*(*) buffer
      character*(*) line
      
      write(buffer(1:13),'(1pd13.4)') line
      
      end
c     ====================================      
      subroutine writebufzgetri(buffer,line)
      
      character*(*) buffer
      character*(*) line
      
      write(buffer(1:13),'(1pd13.4)') line
      
      end
c     ====================================      



