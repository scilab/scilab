c     ====================================
      subroutine rk4(y,dydx,n,x,h,yout,derivs)
C      implicit undefined (a-z)
      external derivs
      integer n,i,nmax
      parameter (nmax=10)
      double precision x,h,hh,xh,h6
      double precision y(n),dydx(n),yout(n),yt(nmax),dyt(nmax),dym(nmax)
      integer iero
      common/ierode/iero
      iero=0
      hh=h*0.5
      h6=h/6.
      xh=x+hh
      do 11 i=1,n
        yt(i)=y(i)+hh*dydx(i)
11    continue
      call derivs(n,xh,yt,dyt)
      if (iero.gt.0) return 
      do 12 i=1,n
        yt(i)=y(i)+hh*dyt(i)
12    continue
      call derivs(n,xh,yt,dym)
      if (iero.gt.0) return 
      do 13 i=1,n
        yt(i)=y(i)+h*dym(i)
        dym(i)=dyt(i)+dym(i)
13    continue
      call derivs(n,x+h,yt,dyt)
      if (iero.gt.0) return 
      do 14 i=1,n
        yout(i)=y(i)+h6*(dydx(i)+dyt(i)+2.*dym(i))
14    continue
      return
      end
c     ====================================
