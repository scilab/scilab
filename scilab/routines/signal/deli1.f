C/MEMBR ADD NAME=DELI1,SSI=0
      subroutine deli1(n,resv,xv,ck)
c!purpose
c      res=integral(zero to x) of
c            1/sqrt((1+t*t)(1+ck*ck*t*t))
c!calling sequence
c     subroutine deli1(n,resv,xv,ck)
c     double precision resv(n),xv(n),ck
c!
      implicit double precision (a-h,o-z)
      dimension resv(n),xv(n)
c
      dpi=4.0d+0*atan(1.0d+00)
      domi=2.0d+0*dlamch('p')
c
      do 200 k=1,n
      x=xv(k)
      if (x .eq. 0) then
         goto 10
      else
         goto 20
      endif
  10  res = 0.0d+0
      goto 150
c
  20  if (ck .eq. 0) then
         goto 30
      else
         goto 40
      endif
  30  res = log(abs(x)+sqrt(1.0d+0+x*x))
      go to 130
c
  40  angle = abs(1.0d+0/x)
      geo = abs(ck)
      ari = 1.0d+0
      pim = 0.0d+0
  50  sqgeo = ari*geo
      aari = ari
      ari = geo + ari
      angle = -sqgeo/angle + angle
      sqgeo = sqrt(sqgeo)
      if (angle .eq. 0) then
         goto 60
      else
         goto 70
      endif
c
c replace 0 by a small value, test
c
  60  angle = sqgeo*domi
  70  test = aari*domi*1.0d+05
      CRES=abs(aari-geo)-test
      if (CRES .le. 0) then
         goto 100
      else
         goto 80
      endif
  80  geo = sqgeo + sqgeo
      pim = pim + pim
      if (angle .lt. 0) then
         goto 90
      else
         goto 50
      endif
  90  pim = pim + dpi
      go to 50
 100  if (angle .lt. 0) then
         goto 110
      else
         goto 120
      endif
 110  pim = pim + dpi
 120  res = (atan(ari/angle)+pim)/ari
 130  if (x .lt. 0) then
         goto 140
      else
         goto 150
      endif
 140  res = -res
 150  continue
      resv(k)=res
 200  continue
      return
      end
