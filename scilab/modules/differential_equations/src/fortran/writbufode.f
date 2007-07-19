c     ====================================
c     INRIA 2007
c     Allan CORNET
c     ====================================
      subroutine writebufodea(buf,itask,meth,jactyp,ml,mu,iopt)
      character*(*) buf
      integer itask
      integer meth
      integer jactyp
      integer ml
      integer mu
      integer iopt
      
      write(buf,'(''itask = '',i3,'' meth = '',i3,'' jactyp = '','//
     $     'i3,'' ml = '',i3,'' mu = '',i3,'' iopt = '',i3)') 
     $           itask,meth,jactyp,ml,mu,iopt
     
      
      end
c     ====================================           
      subroutine writebufodeb(buf,tcrit,ho,hmax,hmin)
      character*(*) buf      
      double precision tcrit
      double precision ho
      double precision hmax
      double precision hmin
      write(buf,'(''tcrit= '',e9.4,'' h0= '',e9.4, '' hmax= '','//
     $       'e9.4,'' hmin = '',e9.4)')
     $    tcrit,ho,hmax,hmin
      end
c     ====================================           
