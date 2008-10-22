      subroutine blnumz(nzm,nzero,zzr,zzi,b2,b1,b0,nze)
c!but
c build numerator blocks of second order
c!
      implicit double precision (a-h,o-z)
      dimension nzm(*),nzero(*),zzr(*),zzi(*)
      double precision b0(*),b1(*),b2(*)
      dimension nze(*)
c
      n = 0
      me = nzm(4)
       do 10 i=1,me
        nze(i) = nzero(i)
  10  continue
c
      do 70 i=1,me
        qr = zzr(i)
        nz = nze(i)
c
  20    if (nz.eq.0) go to 70
        n = n + 1
        b2(n) = 1.0d+0
        if (nz.eq.1) go to 30
        b1(n) = -2.0d+0*qr
        b0(n) = 1.0d+0
        nz = nz - 2
        if (nz.gt.0) go to 20
        go to 70
c
  30    if (i.eq.me) go to 60
        ma = i + 1
         do 40 ii=ma,me
          if (zzi(ii).eq.0.0d+0) go to 50
  40    continue
        go to 60
c
  50    qrr = zzr(ii)
        b1(n) = -qr - qrr
        b0(n) = qr*qrr
        nze(ii) = nze(ii) - 1
        go to 70
c
  60    b1(n) = -qr
        b0(n) = 0.0d+0
c
  70  continue
      return
      end
