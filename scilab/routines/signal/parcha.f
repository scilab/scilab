C/MEMBR ADD NAME=PARCHA,SSI=0
      subroutine parcha(iapro,adeg,adelp,adels,vsn,gd1,gd2,acap12)
c!purpose
c computation of the parameters of the characteristic function
c         Nomenclature Rabiner-Gold (page 241)
c         Si adelp > 0 et adelps > 0
c         acap12=1/k1
c!calling sequence
c      subroutine parcha (iapro,adeg,adelp,adels,vsn,gd1,gd2,acap12)
c      implicit double precision (a-h,o-z)
c!
c
      implicit double precision (a-h,o-z)
c
      gd1 = 0.0d+0
      gd2 = -1.0d+0
      if (adelp.gt.0.0d+0) gd1 = sqrt((2.0d+0-adelp)*adelp)
     +                         / (1.0d+0-adelp)
      if (adels.gt.0.0d+0) gd2 = sqrt(1.0d+0-adels*adels)/adels
      acap12 = gd1/gd2
      if (acap12.gt.0.0d+0) go to 60
      go to (10, 20, 20, 30), iapro
  10  acap12 = vsn**(-adeg)
        go to 40
  20  q = arcosh(vsn)*adeg
      acap12 = 1.0d+0/cosh(q)
      go to 40
  30  call bounn (adeg,acap12,vsn)
  40  if (gd2.eq.(-1.0d+0)) go to 50
      gd1 = acap12*gd2
      adelp = 1.0d+0 - 1.0d+0/sqrt(1.0d+0+gd1*gd1)
      go to 60
  50  gd2 = gd1/acap12
      adels = 1.0d+0/sqrt(1.0d+0+gd2*gd2)
  60  return
      end
