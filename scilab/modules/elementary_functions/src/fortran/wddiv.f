c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA - Serge STEER
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine wddiv(ar,ai,br,cr,ci,ierr)
c!but
c
c     This subroutine wddiv computes c=a/b where a is a complex number
c      and b a real number
c
c!Calling sequence
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
