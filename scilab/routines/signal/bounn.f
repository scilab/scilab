C/MEMBR ADD NAME=BOUNN,SSI=0
      subroutine bounn(adeg,acap12,vsn)
c!purpose
C  calculation of a bounn for vsn or acap12 for elliptic filters
c!
      implicit double precision (a-h,o-z)
c
      dimension dk(3), df(3)
      data de /1.0d+0/
      dff(dd) = (dellk(dd)*dkk/dellk(sqrt(de-dd*dd)))**ii - ddeg
      dpi=4.0d+0*atan(1.0d+00)
      if (acap12.le.0.0d+0) go to 10
      dcap12 = acap12
      deg = 1.0d+0/adeg
      ii = 1
      go to 20
  10  dcap12 = de/vsn
      deg = adeg
      ii = -1
  20  dcap14 = sqrt(de-dcap12*dcap12)
      dkk = dellk(dcap14)/dellk(dcap12)
      dq = exp(-dpi*dkk*deg)
      dk1 = 4.0d+0*sqrt(dq)
      if (dk1.lt.de) go to 30
      dq = 2.0d+0*dq
      dq = (de-dq)/(de+dq)
      dq = dq*dq
      dk1 = sqrt(de-dq*dq)
  30  dk(1) = dk1
      dk(2) = (de+dk(1))/2.0d+0
      ddeg = adeg
      df(1) = dff(dk(1))
      df(2) = dff(dk(2))
   40  dk(3) = dk(1) - df(1)*(dk(1)-dk(2))/(df(1)-df(2))
      df(3) = dff(dk(3))
      if (abs(df(3)).lt.1.0d-6) go to 60
      dmax = 0.0d+0
      do 50 j=1,3
        dab = abs(df(j))
        if (dmax.gt.dab) go to 50
        jj = j
        dmax = dab
  50  continue
      if (jj.eq.3) go to 40
      dk(jj) = dk(3)
      df(jj) = df(3)
      go to 40
  60  if (acap12.le.0.) go to 70
      dde = de/dk(3)
      vsn = dde
      return
  70  acap12 = dk(3)
      return
      end
