      subroutine wwdiv(ar,ai,br,bi,cr,ci,ierr)
c!but
c
c     This subroutine wwdiv computes c=a/b where a and b are complex numbers
c
c!Calling sequence
c
c     subroutine wwdiv(ar,ai,br,bi,cr,ci,ierr)
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
      double precision ar,ai,br,bi,cr,ci
c     c = a/b
      double precision s,d,ars,ais,brs,bis
c
      ierr=0
      if(bi.eq.0.0d0) then
         cr=ar/br
         ci=ai/br
      elseif(br.eq.0.0d0) then
         cr=ai/bi
         ci=-ar/bi
      else
         s = abs(br) + abs(bi)
         if (s .eq. 0.0d+0) then
            ierr=1
            cr=ar/s
            ci=ai/s
            return
         endif
         ars = ar/s
         ais = ai/s
         brs = br/s
         bis = bi/s
         d = brs**2 + bis**2
         cr = (ars*brs + ais*bis)/d
         ci = (ais*brs - ars*bis)/d
      endif
      return
      end
