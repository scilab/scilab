C/MEMBR ADD NAME=CMPSE2,SSI=0
      subroutine cmpse2(m,n,mode,dgetx,dgety,xa,xr,xi,zr,zi,ierr)
c!
c     calcul de  correlations.
c
c authors:      l r rabiner
c               bell laboratories, murray hill, new jersey 07974
c               r w schafer and d dlugos
c
c ieee trans on audio and elect, vol 18, no 4, pp 439-442, 1970.
c
c input:        m is the section size(must be a power of 2)
c               n is the number of samples to be used is the analysis
c               mode is the data format type
c                   mode = 0   auto correlation
c                   mode = 1   cross correlation
c                   mode = 2   auto covariance
c                   mode = 3   cross covariance
c               fs is the sampling frequency in hz
c               iwin is the window type
c                   iwin = 1   rectangular window
c                   iwin = 2   hamming window
c
c     modifications scilab :
c
c        en sortie xa contient m/2+1 coeff de correlation
c        xr,xi,zr,zi sont des tableaux de travail
c        xa,xr,xi dimensionnes a m=puissance de 2
c        zr,zi dimensionnes a m/2+1
c        zr(1),zr(2) contient en sortie xmean et ymean
c!
      double precision xa(m),xr(m),xi(m),zr(*),zi(*)
      double precision xmean,ymean,xsum,ysum,xmnr,xmni
      double precision xir,xii,yir,yii,yirbis,fn
      external dgetx,dgety
      common/  iercorr /iero
c
c
      lshft=m/2
      mhlf1=lshft+1
      nsect=(dble(n)+dble(lshft)-1.0d+0)/dble(lshft)
      iss=1
      nrd=lshft
      xsum=0.0d+0
      ysum=0.0d+0
      do 70 k=1,nsect
      if(k.eq.nsect) nrd=n-(k-1)*nrd
      call dgetx(xa,nrd,iss)
      if (iero.gt.0) return 
      do 40 i=1,nrd
      xsum=xsum+xa(i)
 40   continue
      if(mode.eq.2) goto 60
      call dgety(xa,nrd,iss)
      if (iero.gt.0) return 
      do 50 i=1,nrd
      ysum=ysum+xa(i)
  50  continue
  60  iss=iss+nrd
  70  continue
      xmean=xsum/dble(n)
      ymean=ysum/dble(n)
      if(mode.eq.2) ymean=xmean
      xmnr=xmean
      xmni=ymean
      iss=1
      nrdy=m
      nrdx=lshft
      do 90 i=1,mhlf1
      zr(i)=0.0d+0
      zi(i)=0.0d+0
  90  continue
      do 190 k=1,nsect
      nsect1=nsect-1
      if(k.lt.nsect1) goto 110
      nrdy=n-(k-1)*lshft
      if(k.eq.nsect) nrdx=nrdy
      if(nrdy.eq.m) goto 110
      nrdy1=nrdy+1
      do 100 i=nrdy1,m
      xr(i)=0.0d+0
      xi(i)=0.0d+0
 100  continue
 110  call dgetx(xa,nrdy,iss)
      if (iero.gt.0) return 
      do 120 i=1,nrdy
      xr(i)=xa(i)
      xi(i)=xa(i)
 120  continue
      if(mode.eq.0..or.mode.eq.2) goto 140
      call dgety(xa,nrdy,iss)
      if (iero.gt.0) return 
      do 130 i=1,nrdy
      xi(i)=xa(i)
 130  continue
 140  if(mode.lt.2) goto 160
      do 150 i=1,nrdy
      xr(i)=xr(i)-xmnr
      xi(i)=xi(i)-xmni
 150  continue
 160  nrdx1=nrdx+1
      do 170 i=nrdx1,m
      xr(i)=0.0d+0
 170  continue
      call fft842(0,m,xr,xi,ierr)
      if(ierr.gt.0) return
      do 180 i=2,lshft
      j=m+2-i
      xir=0.50d+0*(xr(i)+xr(j))
      xii=0.50d+0*(xi(i)-xi(j))
      yir=0.50d+0*(xr(j)-xr(i))
      yii=0.50d+0*(xi(i)+xi(j))
      yirbis=yir
      yir=yii
      yii=yirbis
      zr(i)=zr(i)+xir*yir+xii*yii
      zi(i)=zi(i)+xir*yii-xii*yir
 180  continue
      zr(1)=zr(1)+xr(1)*xi(1)
      zr(mhlf1)=zr(mhlf1)+xr(mhlf1)*xi(mhlf1)
      iss=iss+lshft
 190  continue
      do 200 i=2,lshft
      j=m+2-i
      xr(i)=zr(i)
      xi(i)=zi(i)
      xr(j)=zr(i)
      xi(j)=-zi(i)
 200  continue
      xr(1)=zr(1)
      xi(1)=zi(1)
      xr(mhlf1)=zr(mhlf1)
      xi(mhlf1)=zi(mhlf1)
      call fft842(1,m,xr,xi,ierr)
      if(ierr.gt.0) return
      fn=dble(n)
      do 210 i=1,mhlf1
      xa(i)=xr(i)/fn
 210  continue
      xr(1)=xmean
      xr(2)=ymean
      return
      end
