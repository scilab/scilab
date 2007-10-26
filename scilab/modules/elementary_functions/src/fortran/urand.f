C/MEMBR ADD NAME=URAND,SSI=0
      double precision function urand(iy)
      integer iy
      save
c!purpose
c
c      urand is a uniform random number generator based  on  theory  and
c  suggestions  given  in  d.e. knuth (1969),  vol  2.   the integer  iy
c  should be initialized to an arbitrary integer prior to the first call
c  to urand.  the calling program should  not  alter  the  value  of  iy
c  between  subsequent calls to urand.  values of urand will be returned
c  in the interval (0,1).
c
c!calling sequence
c     double precision function urand(iy)
c     integer iy
c!
cc symbolics version
c      double precision function urand(iy)
c      integer iy
c      lispfunction random 'cl-user::random' (integer) integer
c      urand=dble(real(random(2**31)))/(dble(real(2**31))-1.0d+0)
c      return
c      end
cc end
c
      integer ia,ic,itwo,m2,m,mic
      double precision halfm,s
      data m2/0/,itwo/2/
      if (m2 .ne. 0) go to 20
c
c  if first entry, compute machine integer word length
c
      m = 1
   10 m2 = m
      m = itwo*m2
      if (m .gt. m2) go to 10
      halfm = m2
c
c  compute multiplier and increment for linear congruential method
c
      ia = 8*nint(halfm*atan(1.0d+0)/8.0d+0) + 5
      ic = 2*nint(halfm*(0.50d+0-sqrt(3.0d+0)/6.0d+0)) + 1
      mic = (m2 - ic) + m2
c
c  s is the scale factor for converting to floating point
c
      s = 0.50d+0/halfm
c
c  compute next random number
c
   20 iy = iy*ia
c
c  the following statement is for computers which do not allow
c  integer overflow on addition
c
      if (iy .gt. mic) iy = (iy - m2) - m2
c
      iy = iy + ic
c
c  the following statement is for computers where the
c  word length for addition is greater than for multiplication
c
      if (iy/2 .gt. m2) iy = (iy - m2) - m2
c
c  the following statement is for computers where integer
c  overflow affects the sign bit
c
      if (iy .lt. 0) iy = (iy + m2) + m2
      urand = dble(iy)*s
      return
      end
