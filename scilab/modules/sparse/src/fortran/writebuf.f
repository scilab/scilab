c     ====================================
c     INRIA 2006
c     Allan CORNET
c     ====================================
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
