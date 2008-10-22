      subroutine deli2(nn,resv,xxv,ck)
c!purpose
c      calculates an approximate value for the elliptic integral of
c      the first kind, rf(x,y,z), as defined by b.c.carlson.
c      -special functions of applied maths-academic press(1977).
c      res=integral(zero to x) of
c            1/sqrt((1-t*t)(1-ck*ck*t*t))
c      for       0<=t<=1
c
c!calling sequence
c     subroutine deli2(nn,resv,xxv,ck)
c     double precision ck, xxv(nn),resv(nn)
c!
c     .. scalar arguments ..
      double precision x, y, z
      double precision xxv(nn),resv(nn)
      double precision res,xx,ck
c     ..
c     .. local scalars ..
      double precision acc, c1, c2, c3, cxn, cyn, czn, e2, e3, lamda,
     * lolim,mu, rscale, rtx, rty, rtz, uplim, xn, yn, zn
      integer ind
c     .. function references ..
      double precision dlamch
      data acc/8.50d-4/
c
c     order x,y,z into xn,yn,zn  st. xn.le.yn.le.zn
c      write(6,*) nn,resv(1),xxv(1),ck
      do 201 kk=1,nn
      xx=xxv(kk)
      x=1.0d+0-xx*xx
      y=1.0d+0-ck*ck*xx*xx
      if (x.le.y) go to 20
      xn = y
      yn = x
      go to 40
   20 xn = x
      yn = y
      z=1.0d+0
   40 zn = z
      if (yn.le.zn) go to 60
      zn = yn
      yn = z
      if (xn.le.yn) go to 60
      yn = xn
      xn = z
c
c     test for valid arguments
   60 ind = 1
      if (xn.lt.0.0d+0) then
         xn=0.d0
      endif
      ind = 2
      if (yn.le.0.0d+0) go to 180
c
c     valid call
      rscale = 1.0d+0
      lolim=dlamch('u')*16.0d+0
      uplim=0.06250d+0*dlamch('o')
c
c     for extreme arguments scale to avoid under and overflows
      if (zn.le.uplim) go to 120
      rscale = 0.250d+0
      zn = zn*0.06250d+0
      if (yn.le.lolim) go to 80
      yn = yn*0.06250d+0
      if (xn.le.lolim) go to 100
      xn = xn*0.06250d+0
      go to 140
   80 lamda = (sqrt(xn)+sqrt(yn))*(sqrt(zn)*0.250d+0)
      xn = lamda*0.250d+0
      yn = xn
      zn = (zn+lamda)*0.250d+0
      go to 140
  100 rtz = sqrt(zn)
      rty = sqrt(yn)
      lamda = rtz*rty + ((rtz+rty)*0.250d+0)*sqrt(xn)
      xn = lamda*0.250d+0
      yn = (yn+lamda)*0.250d+0
      zn = (zn+lamda)*0.250d+0
      go to 140
  120 if (zn.gt.lolim) go to 140
      rscale = 4.0d+0
      xn = xn*16.0d+0
      yn = yn*16.0d+0
      zn = zn*16.0d+0
c
c     main recursion
  140 mu = (xn+yn+zn)/3.0d+0
      czn = 2.0d+0 - (zn+mu)/mu
      cxn = 2.0d+0 - (xn+mu)/mu
      if (max(cxn,-czn).le.acc) go to 160
      rtx = sqrt(xn)
      rty = sqrt(yn)
      rtz = sqrt(zn)
      lamda = rtz*(rtx+rty) + rtx*rty
      xn = (xn+lamda)*0.250d+0
      yn = (yn+lamda)*0.250d+0
      zn = (zn+lamda)*0.250d+0
      go to 140
c
c     power series expansion
  160 c1 = 1.0d+0/24.0d+0
      c2 = 3.0d+0/44.0d+0
      c3 = 1.0d+0/14.0d+0
      cyn = -cxn - czn
      e2 = cxn*cyn - czn*czn
      e3 = cxn*czn*cyn
      res = rscale*(1.0d+0+(c1*e2-0.1d0-c2*e3)*e2+c3*e3)/sqrt(mu)
      res=res*xx
      go to 200
c
c     failure exits
  180 continue
      res = 0.0d+0
  200 continue
      resv(kk)=res
  201 continue
      return
      end
