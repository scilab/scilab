c     extraits de numerical recipies 
c     runge kutta d'ordre 4 adaptatif 
c     l'interface lsrgk a ete fait en s'inspirant de lsode 
c     voir lsode.f pour comprendre le sens des variables 
C---------------------------------------------------------------------
      subroutine lsrgk (f, neq, y, t, tout, itol, rtol, atol, itask,
     1            istate, iopt, rwork, lrw, iwork, liw, jac, mf)
      external f, jac,rkqc
      integer neq, itol, itask, istate, iopt, lrw, iwork, liw, mf
      double precision y, t, tout, rtol, atol, rwork
      integer nok,nbad
      dimension neq(*), y(*), rtol(*), atol(*), rwork(lrw), iwork(liw)
      integer iero
      common/ierode/iero
      iero=0
      call odeint(y,neq,t,tout,atol(1),1.0d-4,0.0d0,nok,nbad,f,rkqc)
      t=tout
      if (iero.gt.0) istate=-1
      return
      end
C---------
C     subroutine de Num Recipies modifiee pour avoir le meme test 
C     d'arret que lsode
      subroutine odeint(ystart,nvar,x1,x2,eps,h1,hmin,nok,nbad,derivs,rk
     *qc)
C      implicit undefined (a-z)
      external derivs,rkqc
      integer maxstp,nmax,kmax,kount,nvar,i,nok,nbad,nstp
      double precision two,zero,tiny,dxsav,xp(200),yp(10,200),x,h
      parameter (maxstp=10000,nmax=10,two=2.0,zero=0.0,tiny=1.e-30)
      double precision x1,x2,eps,h1,hmin,xsav,hdid,hnext
      double precision ystart(nvar),yscal(nmax),y(nmax),dydx(nmax)
      character*80 messag
      common /path/ kmax,kount,dxsav,xp,yp
      integer iero
      common/ierode/iero
      iero=0
      if ( abs(x2-x1).le.tiny) return
      x=x1
      h=sign(h1,x2-x1)
      nok=0
      nbad=0
      kount=0
      do 11 i=1,nvar
        y(i)=ystart(i)
11    continue
      xsav=x-dxsav*two
      do 16 nstp=1,maxstp
        call derivs(nvar,x,y,dydx)
        if (iero.gt.0) return 
        do 12 i=1,nvar
          yscal(i)=abs(y(i))+abs(h*dydx(i))+tiny
12      continue
        if(kmax.gt.0)then
          if(abs(x-xsav).gt.abs(dxsav)) then
            if(kount.lt.kmax-1)then
              kount=kount+1
              xp(kount)=x
              do 13 i=1,nvar
                yp(i,kount)=y(i)
13            continue
              xsav=x
            endif
          endif
        endif
        if((x+h-x2)*(x+h-x1).gt.zero) h=x2-x
        call rkqc(y,dydx,nvar,x,h,eps,yscal,hdid,hnext,derivs)
        if(iero.gt.0) return
        if(hdid.eq.h)then
          nok=nok+1
        else
          nbad=nbad+1
        endif
        if((x-x2)*(x2-x1).ge.zero)then
          do 14 i=1,nvar
            ystart(i)=y(i)
14        continue
          if(kmax.ne.0)then
            kount=kount+1
            xp(kount)=x
            do 15 i=1,nvar
              yp(i,kount)=y(i)
15          continue
          endif
          return
        endif
        if(abs(hnext).lt.hmin) then
           write(messag, 17) hnext,hmin
           hnext=hmin
        endif
        h=hnext
16    continue
      iero=-1
c      print *, 'Trop d''iterations a faire pour la precision demandee.'
      return
 17   format('stepsize ',e10.3,' smaller than minimum ',e10.3)
      end

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
c         print *, 'stepsize not significant in rkqc.'
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
