      subroutine degree(iapro,vsn,acap12,adeg)
c!but
c computation of the minimum filter degree (adeg)
c         Nomenclature Rabiner-Gold (page 241)
c         acap12=1/k1
c         vsn=1/k
c!calling sequence
c     subroutine degree (iapro,vsn,acap12,adeg)
c     implicit double precision (a-h,o-z)
c!
c
      implicit double precision (a-h,o-z)
      double precision dellk
c
      go to (10, 20, 20, 30), iapro
c
  10  adeg = log(1.0d+0/acap12)/log(vsn)
      return
c
  20  adeg = arcosh(1.0d+0/acap12)/arcosh(vsn)
      return
c
  30  de = 1.0d+0
      dcap02 = de/vsn
      dcap04 = sqrt(de-dcap02*dcap02)
      dcap12 = acap12
      dcap14 = sqrt(de-dcap12*dcap12)
      dadeg = (dellk(dcap02)*dellk(dcap14))/(dellk(dcap04)*dellk(dcap12)
     &    )
      adeg = dadeg
      return
      end
