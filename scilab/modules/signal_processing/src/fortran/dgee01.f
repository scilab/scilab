C/MEMBR ADD NAME=DGEE01,SSI=0
      double precision function dgee01(k,n,m,x)
c!purpose
c   function to calculate the lagrange interpolation
c   coefficients for use in the function gee.
c!
      dimension x(*)
      double precision dev,x
      double precision q
      double precision pi2
      common /rem001/ pi2,dev,nfcns,ngrid
c
      d=1.0d+0
      q=x(k)
      do 3 l=1,m
      do 2 j=l,n,m
      if ((j-k) .eq. 0) then
         goto 2
      else
         goto 1
      endif
    1 d=2.0d+0*d*(q-x(j))
    2 continue
    3 continue
      dgee01=1.0d+0/d
      return
      end
