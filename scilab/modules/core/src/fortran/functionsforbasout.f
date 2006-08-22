c     ====================================
c     INRIA 2006
c     Allan CORNET
c     ====================================
      subroutine readrtechar(onechar)
      include 'stack.h'
      character onechar*1
      read(rte,'(a1)') onechar
      end
c     ====================================     
      subroutine writelunitstring(lunit,string)
      integer lunit
      character*(*) string
      write(lunit,'(a)') string      
      end
c     ====================================           
      subroutine writewtemore()
      include 'stack.h'
      write(wte, '('' more ?'',$)')
      end
c     ====================================           
