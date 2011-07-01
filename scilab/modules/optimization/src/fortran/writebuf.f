c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2007-2008 - INRIA - Allan CORNET
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     ====================================
c     required by f2c :(
c     ====================================
      subroutine writebufbjsqrsolv(buffer,r1,r2,r3)
      
      character*(*) buffer
      double precision r1
      double precision r2
      double precision r3
      
      write(buffer(1:13),'(3i4)') r1,r2,r3

      end
c     ====================================      
      subroutine writebufbjsolv(buffer,r1,r2,r3)
      
      character*(*) buffer
      double precision r1
      double precision r2
      double precision r3
      
      write(buffer(1:12),'(3i4)') r1,r2,r3

      end
c     ====================================
      subroutine writebufbsolv(buffer,r1,r2,r3)
      
      character*(*) buffer
      double precision r1
      double precision r2
      double precision r3
      
      write(buffer(1:12),'(3i4)') r1,r2,r3

      end
c     ====================================
      subroutine writebufblsqrsolv(buffer,r1,r2,r3)
      
      character*(*) buffer
      double precision r1
      double precision r2
      double precision r3
      
      write(buffer(1:12),'(3i4)') r1,r2,r3

      end
c     ====================================
      subroutine writebufscioptim(buffer,r1)
      
      character*(*) buffer
      double precision r1
      
c Initialize the buffer with empty blanks
c      write(buffer(:),'(a)') " "
c What is the p format edit descriptor ?
c      write(buffer(1:15),'(1pd15.7)') r1
      write(buffer(1:15),'(1d15.7)') r1

      end
c     ====================================
      
      subroutine writebufspa(buffer,fname,line)
      
      character*(*) buffer
      character*(*) fname
      integer line
      
      write(buffer,'(A,'': Error while reading line '',I5)') fname,line
      
      end
c     ====================================      
      subroutine writebufspb(buffer,fname,typrow,line)
      
      character*(*) buffer
      character*(*) fname
      character*(*) typrow
      integer line
      
      write(buffer,'(A,''Unknown row type '',a2,'' at line '',I5)')
     $ fname,typrow,line
     
      end
c     ====================================      
