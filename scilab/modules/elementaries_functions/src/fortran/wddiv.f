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
c!
c     Copyright INRIA
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
