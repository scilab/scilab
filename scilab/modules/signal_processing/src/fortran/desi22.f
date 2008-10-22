      subroutine desi22(iapro,ndeg,adelp,adels,adelta,vsn,
     *pren,pimn,ugc,ogc,ack,nj,nh,acx,ac,rdels,sfa,spr,spi)
c!purpose
c chebyshev filter (passband or stopband)
c computation of the poles
c!calling sequence
c     subroutine desi22 (iapro,ndeg,adelp,adels,adelta,vsn,
c    *pren,pimn,ugc,ogc,ack,nj,nh,acx,ac,rdels,sfa,spr,spi)
c
c     implicit double precision (a-h,o-z)
c     dimension spr(*),spi(*)
c     dimension pren(*),pimn(*)
c!
c
      implicit double precision (a-h,o-z)
      double precision spr(*),spi(*)
      double precision pren(*),pimn(*)
c
      flmi=2.0d+0*dlamch('p')
      if (acx.lt.999.0d+0) go to 20
      if ((ogc-ugc).lt.flmi) go to 10
      if (iapro.eq.2) q = 1.0d+0/adelta
      if (iapro.eq.3) q = adelta*adelta
      ac = (2.0d+0*adelp*q/adels)**(1.0d+0/3.0d+0)
      acx = log10(ac/ugc)/log10(ogc/ugc)
      if (acx.ge.0.0d+0 .and. acx.le.1.0d+0) go to 30
  10  acx = 0.50d+0
  20  ac = ugc*(ogc/ugc)**acx
c
c computation of the reduced tolerance scheme
c
  30  q = ac
      if (iapro.eq.3) q = q/adelta
      q = 1.0d+0 + q*q
      rdelp = 1.0d+0- sqrt(1.0d+0/q)
      q = ac
      if (iapro.eq.2) q = q*adelta
      q = 1.0d+0 + q*q
      rdels = sqrt(1.0d+0/q)
c
c computation of the factor sfa and the poles
      if (iapro.eq.3) go to 40
ccccccccccccccccccccccccccccccccccccccccccccc
c     The next line was missing
      sfa = 2.0d+0/(ac*2.0d+0 **ndeg)
ccccccccccccccccccccccccccccccccccccccccccccc
      q = -1.0d+0/ac
      go to 50
  40  sfa = ack
      q = ac
  50  q = arsinh(q)/dble(ndeg)
      qr = sinh(q)
      qi = cosh(q)
      if (iapro.eq.3) go to 70
      do 60 i=1,nj
        spr(i) = qr*pren(i)
        spi(i) = qi*pimn(i)
  60  continue
      return
   70  do 80 i=1,nh
        q = pimn(i)*qi
        qa = pren(i)*qr
        qq = q*q
        qqa = qa*qa
        sfa = sfa/(qq+qqa)
        spr(i) = -vsn/(qq/qa+qa)
        spi(i) = vsn/(q+qqa/q)
  80  continue
      if (nh.eq.nj) return
      spi(nj) = 0.0d+0
      q = vsn/qr
      sfa = sfa*q
      spr(nj) = -q
      return
      end

