c     ====================================
c     INRIA 2006
c     Allan CORNET
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