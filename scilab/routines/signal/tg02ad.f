C/MEMBR ADD NAME=TG02AD,SSI=0
      subroutine tg02ad(ix,n,u,s,d,x,v)
c!but
c      tg02ad - a routine to compute function values and 1st, 2nd
c      and 3rd derivative values of a cubic spline.  the spline
c      is defined by its values and 1st derivative values at the
c      knots.
c
c      the values of the spline and its derivatives are set to be
c      zero if x should fall outside the knot range plus or minus
c      a rounding error.
c!calling sequence
c      ix    set by the user to specify the method of estimating
c            the knot interval containing x.  if ix<0 the
c            initial guess is calculated on the assumption that the
c            knots are equally spaced.  if ix>=0 it assumes that
c            the current x is very near the x of the previous
c            entry and that it is the same spline.
c      n,u(i),s(i),d(i) i=1,n; define the spline and are set by
c            the user to the number of knots, the knots, the spline
c            values at the knots and the 1st derivative values
c            at the knots.  the knots u(i) i=1,n must be ordered.
c      x     the point at which the spline value is required.
c      v(i) i=1,4; set by the routine to the values of the spline
c            and its first three derivatives at the point x.
c!
      implicit double precision(a-h,o-z)
      double precision u(*),s(*),d(*),v(*)
c
      common/tg02bd/ k
c
c      k     returned set to the knot interval that
c            contained x, i.e. u(k)<=x<=u(k+1).
c
c      allowable rounding error on points at extreams
c      of knot range is 2**ieps*max(|u(1)|,|u(n)|).
      integer iflg,ieps
      data iflg,ieps/0,-30/
c
c      test whether point in range.
      j=0
      k=0
      if(x.lt.u(1)) go to 990
      if(x.gt.u(n)) go to 991
c
c      jump if knot interval requires random search.
      if(ix.lt.0.or.iflg.eq.0) go to 12
c      test if knot interval same as last time.
      if(x.gt.u(j+1)) go to 1
      if(x.ge.u(j)) go to 18
      go to 2
c          loop till interval found.
    1 j=j+1
   11 if(x.gt.u(j+1)) go to 1
      go to 7
c
c          estimate knot interval by assuming equally spaced knots.
   12 j=abs(x-u(1))/(u(n)-u(1))*(n-1)+1
c          ensure case x=u(n) gives j=n-1.
      j=min(j,n-1)
c          indicate that knot interval inside range has been used.
      iflg=1
c          search for knot interval containing x.
      if(x.ge.u(j)) go to 11
    2 j=j-1
      if(x.lt.u(j)) go to 2
c
c          calculate spline parameters for jth interval.
c
c      knot interval
    7 k=j
      h=u(j+1)-u(j)
      hr=1.0d+0/h
      hrr=(hr+hr)*hr
c
      s0=s(j)
      s1=s(j+1)
      d0=d(j)
      d1=d(j+1)
      a=s1-s0
      b=a-h*d1
      a=a-h*d0
      c=a+b
      c3=c*3.0d+0
c      transform the variable
   18 theta=(x-u(j))*hr
      phi=1.0d+0-theta
      t=theta*phi
      gama=theta*b-phi*a
c      spline value
      v(1)=theta*s1+phi*s0+t*gama
c      1st derivative value
      v(2)=theta*d1+phi*d0+t*c3*hr
c      2nd derivative value
      v(3)=(c*(phi-theta)-gama)*hrr
c      3rd derivative value
      v(4)=-c3*hrr*hr
      return
c          test if x within rounding error of u(1).
  990 if(x.le.u(1)-2.0d+0**ieps*max(abs(u(1)),abs(u(n)))) go to 99
      j=1
      go to 7
c          test if x within rounding error of u(n).
  991 if(x.ge.u(n)+2.0d+0**ieps*max(abs(u(1)),abs(u(n)))) go to 995
      j=n-1
      go to 7
  995 k=n
   99 iflg=0
c          values set to zero for points outside the range
    5 do 6 i=1,4
    6 v(i)=0d+0
      return
      end
