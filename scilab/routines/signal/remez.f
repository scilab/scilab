C/MEMBR ADD NAME=REMEZ,SSI=0
      subroutine remez(ngr,nfc,iext,ad,x,y,des,grid,wt,a,p,q,alpha)
c!purpose
c   this subroutine implements the remez exchange algorithm
c   for the weighted chebyshev approximation of a continuous
c   function with a sum of cosines.  inputs to the subroutine
c   are a dense grid which replaces the frequency axis, the
c   desired function on this grid, the weight function on the
c   grid, the number of cosines, and an initial guess of the
c   extremal frequencies.  the program minimizes the chebyshev
c   error by determining the best location of the extremal
c   frequencies (points of maximum error) and then calculates
c   the coefficients of the best approximation.
c!calling sequence
c     subroutine remez(ngr,nfc,iext,ad,x,y,des,grid,wt,a,p,q,alpha)
c   dimensioning of arrays in calling program is as follows:
c         working vectors   ad,x,y,a,p,q:   nfc+2
c         input vectors     des,grid,wt:    ngr
c         input vector      iext:           nfc+2
c         output vector     alpha:          nfc+2
c!
      dimension iext(*),ad(*),alpha(*),x(*),y(*)
      dimension des(*),grid(*),wt(*)
      dimension a(*),p(*),q(*)
      double precision pi2,dnum,dden,dtemp,a,p,q
      double precision dk,dak
      double precision ad,dev,x,y
      double precision gee,dgee01
      double precision alpha
      common /rem001/ pi2,dev,nfcns,ngrid
      common /oops/niter,iout
c
c  the program allows a maximum number of iterations of 25
c
      itrmax=25
      ngrid=ngr
      nfcns=nfc
      pi=4.0d+0*atan(1.0d+0)
      pi2=2.0d+0*pi
      devl=-1.0d+0
      nz=nfcns+1
      nzz=nfcns+2
      niter=0
  100 continue
      iext(nzz)=ngrid+1
      niter=niter+1
      if(niter.gt.itrmax) go to 400
      do 110 j=1,nz
      jxt=iext(j)
      dtemp=grid(jxt)
      dtemp=cos(dtemp*pi2)
  110 x(j)=dtemp
      jet=(nfcns-1)/15+1
      do 120 j=1,nz
  120 ad(j)=dgee01(j,nz,jet,x)
      dnum=0.0d+0
      dden=0.0d+0
      k=1
      do 130 j=1,nz
      l=iext(j)
      dtemp=ad(j)*des(l)
      dnum=dnum+dtemp
      dtemp=dble(k)*ad(j)/wt(l)
      dden=dden+dtemp
  130 k=-k
      dev=dnum/dden
c      write(6,131) dev
c  131 format(1x,12hdeviation = ,f12.9)
      nu=1
      if(dev.gt.0.0d+0) nu=-1
      dev=-dble(nu)*dev
      k=nu
      do 140 j=1,nz
      l=iext(j)
      dtemp=dble(k)*dev/wt(l)
      y(j)=des(l)+dtemp
  140 k=-k
      if(dev.gt.devl) go to 150
      call ouch
      go to 400
  150 devl=dev
      jchnge=0
      k1=iext(1)
      knz=iext(nz)
      klow=0
      nut=-nu
      j=1
c
c  search for the extremal frequencies of the best
c  approximation
c
  200 if(j.eq.nzz) ynz=comp
      if(j.ge.nzz) go to 300
      kup=iext(j+1)
      l=iext(j)+1
      nut=-nut
      if(j.eq.2) y1=comp
      comp=dev
      if(l.ge.kup) go to 220
      err=gee(l,nz,ad,x,y,grid)
      err=(err-des(l))*wt(l)
      dtemp=dble(nut)*err-comp
      if(dtemp.le.0.0d+0) go to 220
      comp=dble(nut)*err
  210 l=l+1
      if(l.ge.kup) go to 215
      err=gee(l,nz,ad,x,y,grid)
      err=(err-des(l))*wt(l)
      dtemp=dble(nut)*err-comp
      if(dtemp.le.0.0d+0) go to 215
      comp=dble(nut)*err
      go to 210
  215 iext(j)=l-1
      j=j+1
      klow=l-1
      jchnge=jchnge+1
      go to 200
  220 l=l-1
  225 l=l-1
      if(l.le.klow) go to 250
      err=gee(l,nz,ad,x,y,grid)
      err=(err-des(l))*wt(l)
      dtemp=dble(nut)*err-comp
      if(dtemp.gt.0.0d+0) go to 230
      if(jchnge.le.0) go to 225
      go to 260
  230 comp=dble(nut)*err
  235 l=l-1
      if(l.le.klow) go to 240
      err=gee(l,nz,ad,x,y,grid)
      err=(err-des(l))*wt(l)
      dtemp=dble(nut)*err-comp
      if(dtemp.le.0.0d+0) go to 240
      comp=dble(nut)*err
      go to 235
  240 klow=iext(j)
      iext(j)=l+1
      j=j+1
      jchnge=jchnge+1
      go to 200
  250 l=iext(j)+1
      if(jchnge.gt.0) go to 215
  255 l=l+1
      if(l.ge.kup) go to 260
      err=gee(l,nz,ad,x,y,grid)
      err=(err-des(l))*wt(l)
      dtemp=dble(nut)*err-comp
      if(dtemp.le.0.0d+0) go to 255
      comp=dble(nut)*err
      go to 210
  260 klow=iext(j)
      j=j+1
      go to 200
  300 if(j.gt.nzz) go to 320
      if(k1.gt.iext(1)) k1=iext(1)
      if(knz.lt.iext(nz)) knz=iext(nz)
      nut1=nut
      nut=-nu
      l=0
      kup=k1
      comp=ynz*(1.000010d+0)
      luck=1
  310 l=l+1
      if(l.ge.kup) go to 315
      err=gee(l,nz,ad,x,y,grid)
      err=(err-des(l))*wt(l)
      dtemp=dble(nut)*err-comp
      if(dtemp.le.0.0d+0) go to 310
      comp=dble(nut)*err
      j=nzz
      go to 210
  315 luck=6
      go to 325
  320 if(luck.gt.9) go to 350
      if(comp.gt.y1) y1=comp
      k1=iext(nzz)
  325 l=ngrid+1
      klow=knz
      nut=-nut1
      comp=y1*(1.000010d+0)
  330 l=l-1
      if(l.le.klow) go to 340
      err=gee(l,nz,ad,x,y,grid)
      err=(err-des(l))*wt(l)
      dtemp=dble(nut)*err-comp
      if(dtemp.le.0.0d+0) go to 330
      j=nzz
      comp=dble(nut)*err
      luck=luck+10
      go to 235
  340 if(luck.eq.6) go to 370
      do 345 j=1,nfcns
      nzzmj=nzz-j
      nzmj=nz-j
  345 iext(nzzmj)=iext(nzmj)
      iext(1)=k1
      go to 100
  350 kn=iext(nzz)
      do 360 j=1,nfcns
  360 iext(j)=iext(j+1)
      iext(nz)=kn
      go to 100
  370 if(jchnge.gt.0) go to 100
c
c  calculation of the coefficients of the best approximation
c  using the inverse discrete fourier transform
c
  400 continue
      nm1=nfcns-1
      fsh=1.0e-06
      gtemp=grid(1)
      x(nzz)=-2.0d+0
      cn=2*nfcns-1
      delf=1.0d+0/cn
      l=1
      kkk=0
      if(grid(1).lt.0.010d+0.and.grid(ngrid).gt.0.490d+0) kkk=1
      if(nfcns.le.3) kkk=1
      if(kkk.eq.1) go to 405
      dtemp=cos(pi2*grid(1))
      dnum=cos(pi2*grid(ngrid))
      aa=2.0d+0/(dtemp-dnum)
      bb=-(dtemp+dnum)/(dtemp-dnum)
  405 continue
      do 430 j=1,nfcns
      ft=j-1
      ft=ft*delf
      xt=cos(pi2*ft)
      if(kkk.eq.1) go to 410
      xt=(xt-bb)/aa
      xt1=sqrt(1.0d+0-xt*xt)
      ft=atan2(xt1,xt)/pi2
  410 xe=x(l)
      if(xt.gt.xe) go to 420
      if((xe-xt).lt.fsh) go to 415
      l=l+1
      go to 410
  415 a(j)=y(l)
      go to 425
  420 if((xt-xe).lt.fsh) go to 415
      grid(1)=ft
      a(j)=gee(1,nz,ad,x,y,grid)
  425 continue
      if(l.gt.1) l=l-1
  430 continue
      grid(1)=gtemp
      dden=pi2/cn
      do 510 j=1,nfcns
      dtemp=0.0d+0
      dnum=j-1
      dnum=dnum*dden
      if(nm1.lt.1) go to 505
      do 500 k=1,nm1
      dak=a(k+1)
      dk=k
  500 dtemp=dtemp+dak*cos(dnum*dk)
  505 dtemp=2.0d+0*dtemp+a(1)
  510 alpha(j)=dtemp
      do 550 j=2,nfcns
  550 alpha(j)=2.0d+0*alpha(j)/cn
      alpha(1)=alpha(1)/cn
      if(kkk.eq.1) go to 545
      p(1)=2.0d+0*alpha(nfcns)*bb+alpha(nm1)
      p(2)=2.0d+0*aa*alpha(nfcns)
      q(1)=alpha(nfcns-2)-alpha(nfcns)
      do 540 j=2,nm1
      if(j.lt.nm1) go to 515
      aa=0.5*aa
      bb=0.5*bb
  515 continue
      p(j+1)=0.0d+0
      do 520 k=1,j
      a(k)=p(k)
  520 p(k)=2.0d+0*bb*a(k)
      p(2)=p(2)+a(1)*2.0d+0*aa
      jm1=j-1
      do 525 k=1,jm1
  525 p(k)=p(k)+q(k)+aa*a(k+1)
      jp1=j+1
      do 530 k=3,jp1
  530 p(k)=p(k)+aa*a(k-1)
      if(j.eq.nm1) go to 540
      do 535 k=1,j
  535 q(k)=-a(k)
      nf1j=nfcns-1-j
      q(1)=q(1)+alpha(nf1j)
  540 continue
      do 543 j=1,nfcns
  543 alpha(j)=p(j)
  545 continue
      if(nfcns.gt.3)return
      alpha(nfcns+1)=0.0d+0
      alpha(nfcns+2)=0.0d+0
      return
      end
