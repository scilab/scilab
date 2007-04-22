c     ====================================
c     INRIA 2006
c     Allan CORNET
c     ====================================
      subroutine writelunitstring(lunit,string)
      integer lunit
      character*(*) string
      write(lunit,'(a)') string      
      end
c     ====================================           
