C/MEMBR ADD NAME=TRBIPO,SSI=0
      subroutine trbipo(nmaxi,maxdeg,nj,sfa,sm,spr,spi,zfa,zpr,zpi)
c!purpose
c  bilinear transformation of the poles
c!
      implicit double precision (a-h,o-z)
      double precision sm(maxdeg,*)
      double precision spr(*),spi(*),zpr(*),zpi(*)
      flmi=2.0d+0*dlamch('p')
      zfa = sfa*sm(nmaxi-1,1)
      do 20 i=1,nj
        qr = spr(i)
        q = 1.0d+0 - qr
        qi = spi(i)
        if (abs(qi).lt.flmi) go to 10
        qqr = qr*qr
        qqi = qi*qi
        zfa = zfa/(q-qr+qqr+qqi)
        q = 1.0d+0/(q*q+qqi)
        zpr(i) = (1.0d+0-qqr-qqi)*q
        zpi(i) = 2.0d+0*qi*q
        go to 20
  10    zpr(i) = (1.0d+0+qr)/q
        zpi(i) = 0.0d+0
        zfa = zfa/q
  20  continue
      return
      end
