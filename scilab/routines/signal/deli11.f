C/MEMBR ADD NAME=DELI11,SSI=0
      subroutine deli11(x, ck,res)
c!purpose
c  elliptic function
c!calling sequence
c     subroutine deli11(x, ck,res)
c     double precision x,ck,res
c!
      implicit double precision (a-h,o-z)
      dpi=4.0d+0*atan(1.0d+00)
      domi=2.0d+0*dlamch('p')
      if (x) 20, 10, 20
  10  res = 0.0d+0
      return
  20  if (ck) 40, 30, 40
  30  res = log(abs(x)+sqrt(1.0d+0+x*x))
      go to 130
  40  angle = abs(1.0d+0/x)
      geo = abs(ck)
      ari = 1.0d+0
      pim = 0.0d+0
   50  sqgeo = ari*geo
      aari = ari
      ari = geo + ari
      angle = -sqgeo/angle + angle
      sqgeo = sqrt(sqgeo)
      if (angle) 70, 60, 70
C  replace 0 by a small value, test
  60  angle = sqgeo*domi
  70  test = aari*domi*1.0d+05
      if (abs(aari-geo)-test) 100, 100, 80
  80  geo = sqgeo + sqgeo
      pim = pim + pim
      if (angle) 90, 50, 50
  90  pim = pim + dpi
      go to 50
 100  if (angle) 110, 120, 120
 110  pim = pim + dpi
 120  res = (atan(ari/angle)+pim)/ari
 130  if (x) 140, 150, 150
 140  res = -res
 150  return
      end
