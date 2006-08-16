      subroutine compel(dk, dellk)
c!purpose
c  calculate complete elliptic integral of first kind
c!calling sequence
c     subroutine compel(dk, dellk)
c     double precision dk,dellk
c!
      double precision dpi, domi,dellk
      double precision de, dgeo, dk, dri, dari, dtest
      external slamch, dlamch
      double precision dlamch,flma
      real slamch
c
      data de /1.0d+0/
      flma=2.0d+0**(int(slamch('l'))-2)
      dpi=4.0d+0*atan(1.0d+0)
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
      if((dari-dgeo-dtest).le.0.0d+0) goto 50
      dgeo = sqrt(dari*dgeo)
      dri = 0.50d+0*dri
      go to 30
  50  dellk = dpi/dri
      return
      end
