      subroutine tranpo(nmaxi,maxdeg,ityp,vd,sm,a,ndeg,sfa,spr,spi,nj)
c!purpose
c  reactance transformation of the poles
c!
      implicit double precision (a-h,o-z)
      double precision sm (maxdeg,*)
      double precision spr(*),spi(*)
      external slamch, dlamch
      real slamch
c
      flmi=2.0d+0*dlamch('p')
      flma=2.0d+0**(int(slamch('l'))-2)
      if (ityp.eq.1) go to 90
       if (ityp.eq.3) go to 40
      do 30 i=1,nj
        qr = spr(i)
        qi = spi(i)
        qh = qr*qr + qi*qi
        if (abs(qi).gt.flmi) go to 10
        sfa = -sfa/qr
        go to 20
  10    sfa = sfa/qh
  20    qi = qi/qh
        if (abs(qi).lt.flmi) qi = 0.0d+0
        spi(i) = qi
        spr(i) = qr/qh
  30  continue
      if (ityp.eq.2) go to 90
  40  qa = 2.0d+0*a
      nn = nj
      nj = ndeg
       ndeg = 2*ndeg
      me = nj
      do 80 i=1,nn
        qr = spr(i)/qa
        qi = spi(i)/qa
        if (abs(qi).ge.flma) go to 70
        dr = qr
        di = qi
        dq = di*di
        di = di*dr*2.0d+0
        dr = dr*dr - dq - 1.0d+0
        call dsqrtc(dr, di, dr, di)
        qz = dr
        qn = di
        if (abs(qn).gt.flmi) go to 60
        jj = nj + me
        do 50 ii=me,nj
          j = jj - ii
          spr(j+1) = spr(j)
           spi(j+1) = spi(j)
  50    continue
        nj = nj + 1
        me = me + 1
  60    spr(i) = qr + qz
        spi(i) = qi + qn
        spr(me) = qr - qz
        spi(me) = qn - qi
        me = me - 1
        go to 80
  70    spr(i) = qr
        spi(i) = flma
        nj = nj + 1
        spr(nj) = qr
        spi(nj) = 0.0d+0
  80  continue
  90  do 100 i=1,nj
        spr(i) = spr(i)*vd
         spi(i) = spi(i)*vd
 100  continue
      sfa = sfa*sm(nmaxi-1,4)
      return
      end
