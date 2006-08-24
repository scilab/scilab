      subroutine dwdiv(ar,br,bi,cr,ci,ierr)
c!but
c
c     This subroutine dwdiv computes c=a/b where a is a real number and
c      b a complex number
c
c!Calling sequence
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
c!
c     Copyright INRIA
      double precision ar,br,bi,cr,ci
c     c = a/b
      double precision s,d,ars,brs,bis,dlamch
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
