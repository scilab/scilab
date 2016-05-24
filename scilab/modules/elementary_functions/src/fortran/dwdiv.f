c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA - Serge STEER
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      subroutine dwdiv(ar,br,bi,cr,ci,ierr)
c!but
c
c     This subroutine dwdiv computes c=a/b where a is a real number and
c      b a complex number
c
c!Syntax
c
c     subroutine dwdiv(ar,br,bi,cr,ci,ierr)
c
c     ar    : double precision.
c
c     br, bi: double precision, b real and complex parts.
c
c     cr, ci: double precision, c real and complex parts.
c
c!author
c
c     Serge Steer INRIA
c
      double precision ar,br,bi,cr,ci
c     c = a/b
      double precision s,d,ars,brs,bis
c
      ierr=0
      if(bi.eq.0.0d0) then
         cr=ar/br
         ci=0.0d0
      elseif(br.eq.0.0d0) then
         ci=-ar/bi        
         cr=0.0d0
      else
         s = abs(br) + abs(bi)
         if (s .eq. 0.0d+0) then
            ierr=1
            cr=ar/s
            ci=0.0d0
            return
         endif
         ars = ar/s
         brs = br/s
         bis = bi/s
         d = brs**2 + bis**2
         cr = (ars*brs)/d
         ci = (-ars*bis)/d
      endif
      return
      end
