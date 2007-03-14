c     ====================================
c     INRIA 2007
c     Allan CORNET
c     requires by f2c :(
c     ====================================
      subroutine writebufio(buffer,line)
      
      character*(*) buffer
      character*(*) line
      
      write(buffer(1:4),'(i4)') line
      
      end
c     ====================================  
      subroutine writebufw(chaine,line)
      
      character*(*) chaine
      character*(*) line
      
      write(chaine,'(''('',i2,''(1x,1pd17.10))'')') line
      
      end
c     ====================================  
      subroutine writeiotobuf(line)
      INCLUDE 'stack.h'

      character*(*) line
      write(buf,line)
     
      end
c     ====================================  
