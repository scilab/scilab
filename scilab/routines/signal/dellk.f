      double precision function dellk(dk)
c!purpose
C  calculate complete elliptic integral of first kind
c!
      implicit double precision (a-h,o-z)
      external slamch, dlamch
      real slamch
c
      data de /1.0d+0/
      flma=2.0d+0**(int(slamch('l'))-2)
      dpi=4.0d+0*atan(1.0d+00)
      domi=2.0d+0*dlamch('p')
      dgeo = de - dk*dk
      if (dgeo .le. 0) then
         goto 10
      else
         goto 20
      endif
  10  dellk = flma
      return
  20  dgeo = sqrt(dgeo)
      dri = de
  30  dari = dri
      dtest = dari*domi
      dri = dgeo + dri
      if (dari-dgeo-dtest .le. 0) then
         goto 50
      else
         goto 40
      endif
  40  dgeo = sqrt(dari*dgeo)
      dri = 0.50d+0*dri
      go to 30
  50  dellk = dpi/dri
      return
      end
