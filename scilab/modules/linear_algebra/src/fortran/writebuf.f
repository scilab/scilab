
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

c     ====================================
c     requires by f2c :(
c     ====================================
      subroutine writebufdgesv3(buffer,r)
      
      character*(*) buffer
      double precision r
      
      write(buffer(1:13),'(1pd13.4)') r

      end
c     ====================================      
      subroutine writebufdgesv4(buffer,r)
      
      character*(*) buffer
      double precision r
      
      write(buffer(1:13),'(1pd13.4)') r
      
      end
c     ====================================      
      subroutine writebufdgetri(buffer,r)
      
      character*(*) buffer
      double precision r
      
      write(buffer(1:13),'(1pd13.4)') r
      
      end
c     ====================================      
      subroutine writebufzgesv3(buffer,r)
      
      character*(*) buffer
      double precision r
      
      write(buffer(1:13),'(1pd13.4)') r
      
      end
c     ====================================      
      subroutine writebufzgesv4(buffer,r)
      
      character*(*) buffer
      double precision r
      
      write(buffer(1:13),'(1pd13.4)') r
      
      end
c     ====================================      
      subroutine writebufzgetri(buffer,r)
      
      character*(*) buffer
      double precision r
      
      write(buffer(1:13),'(1pd13.4)') r
      
      end
c     ====================================      



