      integer function idmin(n,dx,incx)
c
c     finds the index of element having min. value.
c     derived from idamax (blas)
c
      double precision dx(*),dmin
      integer i,incx,ix,n
c
      idmin = 0
      if( n.lt.1 .or. incx.le.0 ) return
      idmin = 1
      if(n.eq.1)return
      if(incx.eq.1)go to 20
c
c        code for increment not equal to 1
c
      ix = 1
      dmin = dx(1)
      ix = ix + incx
      do 10 i = 2,n
         if(dx(ix).ge.dmin) go to 5
         idmin = i
         dmin = dx(ix)
    5    ix = ix + incx
   10 continue
      return
c
c        code for increment equal to 1
c
   20 dmin = dx(1)
      do 30 i = 2,n
         if(dx(i).ge.dmin) go to 30
         idmin = i
         dmin = dx(i)
   30 continue
      return
      end
