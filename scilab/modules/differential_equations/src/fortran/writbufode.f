c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2007 - INRIA - Allan CORNET
c ...
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
