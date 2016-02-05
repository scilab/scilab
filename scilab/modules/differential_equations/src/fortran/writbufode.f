c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2007 - INRIA - Allan CORNET
c ...
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
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
      write(buf,'(''tcrit= '',e11.4,'' h0= '',e11.4, '' hmax= '','//
     $       'e11.4,'' hmin = '',e11.4)')
     $    tcrit,ho,hmax,hmin
      end
c     ====================================           
