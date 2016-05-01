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
      subroutine wddiv(ar,ai,br,cr,ci,ierr)
c!but
c
c     This subroutine wddiv computes c=a/b where a is a complex number
c      and b a real number
c
c!Syntax
c
c     subroutine wddiv(ar,ai,br,bi,cr,ci,ierr)
c
c     ar, ai: double precision, a real and complex parts.
c
c     br, bi: double precision, b real and complex parts.
c
c     cr, ci: double precision, c real and complex parts.
c
c!author
c
c     Serge Steer
c
      double precision ar,ai,br,cr,ci
c
      ierr=0

      if (br .eq. 0.0d+0) then
         ierr=1
c         return
      endif
      cr = ar/br
      ci = ai/br
      return
      end
