c     ====================================
      subroutine rkqc(y,dydx,n,x,htry,eps,yscal,hdid,hnext,derivs)
C      implicit undefined (a-z)
      integer nmax,n,i
      double precision fcor,one,safety,errcon
      parameter (nmax=10,fcor=.0666666667,
     $     one=1.0,safety=0.9,errcon=6.e-4)
      double precision x,htry,eps,hdid,hnext,pgrow,pshrnk,xsav,hh
      double precision errmax,h,dysav(nmax)
      double precision y(n),dydx(n),yscal(n),ytemp(nmax),ysav(nmax)
     
      external derivs
      integer iero
      common/ierode/iero
      iero=0
      pgrow=-0.20
      pshrnk=-0.25
      xsav=x
      do 11 i=1,n
        ysav(i)=y(i)
        dysav(i)=dydx(i)
11    continue
      h=htry
1     hh=0.5*h
      call rk4(ysav,dysav,n,xsav,hh,ytemp,derivs)
      x=xsav+hh
      call derivs(n,x,ytemp,dydx)
      if (iero.gt.0) return 
      call rk4(ytemp,dydx,n,x,hh,y,derivs)
      x=xsav+h
      if(x.eq.xsav) then 
         iero=1
         return
      endif
      call rk4(ysav,dysav,n,xsav,h,ytemp,derivs)
      errmax=0.
      do 12 i=1,n
        ytemp(i)=y(i)-ytemp(i)
        errmax=max(errmax,abs(ytemp(i)/(yscal(i)*eps)))
12    continue
      if(errmax.gt.one) then
        h=safety*h*(errmax**pshrnk)
        goto 1
      else
        hdid=h
        if(errmax.gt.errcon)then
          hnext=safety*h*(errmax**pgrow)
        else
          hnext=4.*h
        endif
      endif
      do 13 i=1,n
        y(i)=y(i)+ytemp(i)*fcor
13    continue
      return
      end
c     ====================================
