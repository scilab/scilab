c     ====================================
c     INRIA 2007
c     Allan CORNET
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



