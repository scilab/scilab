C/MEMBR ADD NAME=DESI01,SSI=0
      subroutine desi01(maxdeg,iapro,edeg,ndeg,adelp,adels,
     *vsn,adeg,gd1,gd2,acap12,ierr)
c!purpose
c design of butterworth, chebyshev (passband or stopband), and
c elliptic filters
c!
      implicit double precision (a-h,o-z)
c
      call parcha (iapro,adeg,adelp,adels,vsn,gd1,gd2,acap12)
      call degree (iapro,vsn,acap12,adeg)
      q = adeg*(1.0d+0+edeg) + 0.50d+0
      n = int(q)
      m = int(adeg)
      if (real(m).lt.adeg) m = m + 1
      n = max0(m,n)
      if (ndeg.eq.0) go to 10
      if (ndeg.ge.n) go to 20
      ierr=15
c
  10  ndeg = n
c
  20  if (ndeg.le.maxdeg) return
      ierr=25
      return
      end
