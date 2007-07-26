c     ====================================
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
c     ====================================
      
      