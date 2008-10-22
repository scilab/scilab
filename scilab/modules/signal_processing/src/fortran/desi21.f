      subroutine desi21(ndeg,adelp,adels,adelta,pren,pimn,
     *ugc,ogc,nj,acx,ac,rdelp,rdels,sfa,spr,spi)
c!purpose
c butterworth filter
c computation of the poles
c!
c
      implicit double precision (a-h,o-z)
      double precision spr(*), spi(*)
      double precision pren(*), pimn(*)
      flmi=2.0d+0*dlamch('p')
c
c computation of constant c and reduced tolerance scheme
c
      if (acx.lt.999.0d+0) go to 20
      if ((ogc-ugc).lt.flmi) go to 10
      ac = (2.0d+0*adelp/(adelta*adels))**(1.0d+0/3.0d+0)
      acx = log10(ac/ugc)/log10(ogc/ugc)
      if (acx.ge.0.0d+0 .and. acx.le.1.0d+0) go to 30
  10  acx = 0.50d+0
  20  ac = ugc*(ogc/ugc)**acx
  30  rdelp = 1.0d+0 - sqrt(1.0d+0/(1.0d+0+ac*ac))
      q = ac*adelta
      rdels = sqrt(1.0d+0/(1.0d+0+q*q))
c
c computation of factor sfa and poles
c
      sfa = 1.0d+0/ac
      q = ac**(-1.0d+0/dble(ndeg))
c
      do 40 i=1,nj
        spr(i) = -q*pren(i)
        spi(i) = q*pimn(i)
  40  continue
      return
      end
