      subroutine desi24(ndeg,adelp,adels,adelta,dsk,ugc,ogc,ack,
     *nj,nh,dk,dks,dcap02,dcap04,acx,ac,rdelp,rdels,sfa,spr,spi)
c!purpose
C  elliptic filter
C computation of the reduced tolerance scheme, the factor sfa, and
C  the poles
c!
      implicit double precision (a-h,o-z)
      double precision dsk(*)
      double precision dellk
      double precision spr(*),spi(*)
      data de /1.0d+0/
      flmi=2.0d+0*dlamch('p')
      dpi=4.00d+0*atan(1.0d+00)
C  if acx not defined, compute a symmetrical usage of the tolerance
C  scheme
      if (acx.lt.999.0d+0) go to 20
      if ((ogc-ugc).lt.flmi) go to 10
      ac = (2.0d+0*adelp/(adelta*adels))**(1.0d+0/3.0d+0)
      acx = log10(ac/ugc)/log10(ogc/ugc)
      if (acx.ge.0.0d+0 .and. acx.le.1.0d+0) go to 30
  10  acx = 0.50d+0
  20  ac = ugc*(ogc/ugc)**acx
C  computation of the reduced tolerance scheme
   30  q = ac*adelta
      du = de/q
      rdelp = 1.0d+0 - sqrt(1.0d+0/(1.0d+0+q*q))
      q = 1.0d+0 + ac*ac/(adelta*adelta)
      rdels = sqrt(1.0d+0/q)
C  computation of the factor sfa and the poles
      q = ac*ack
      if (nh.eq.nj) q = sqrt(1.0d+0+q*q)
      sfa = 1.0d+0/q
      dr = adelta
      dr = dr*dr
      dq = q
      call deli11(du, dr, dq)
      du = dq
      dq = sqrt(de-dr*dr)
      dq = dellk(dr)
      du = dk*du/(dq*dble(ndeg))
      dq = exp(-dpi*dk/dks)
      du = -dsn2(du,dks,dq)
      dq = du*du
      dud = de - dcap04*dcap04*dq
      dud = sqrt(dud)
      duc = sqrt(de-dq)
      do 40 i=1,nj
        dr = dsk(i)
        drc = dr*dr
        drd = de - dcap02*dcap02*drc
        drc = sqrt(de-drc)
        dm = de - dq*drd
        drd = sqrt(drd)
        drd = drd*du*duc*drc/dm
        spr(i) = drd
        dr = dr*dud/dm
        spi(i) = dr
  40  continue
      return
      end
