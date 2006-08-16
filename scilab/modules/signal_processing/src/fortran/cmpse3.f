C/MEMBR ADD NAME=CMPSE3,SSI=0
      subroutine cmpse3(m,n,mode,x,y,xr,xi,zr,zi,ierr,ichaud,nbx)
c!
c     calcul de  correlations.
c
c     d'apres rabiner
c ieee trans on audio and elect, vol 18, no 4, pp 439-442, 1970.
c
c input:        m is the section size(must be a power of 2)
c               n is the number of samples to be used is the analysis
c               mode is the data format type
c                   mode = 0   auto correlation
c                   mode = 1   cross correlation
c
c     modifications scilab :
c
c        en sortie xa contient m/2+1 coeff de correlation
c        xr,xi sont des tableaux de travail
c        xr,xi dimensionnes a m=puissance de 2
c        zr,zi dimensionnes a m/2+1 en sortie zr et zi sont updates
c        x et y contiennent les deux series a correler
c!
      double precision x(n),y(n)
      double precision xr(m),xi(m),zr(*),zi(*)
      double precision xir,xii,yir,yii,yirbis
c
c
      lshft=m/2
      mhlf1=lshft+1
      nsect=(dble(n)+dble(lshft)-1.0d+0)/dble(lshft)
      iss=1
      nrdy=m
      nrdx=lshft
c     demarrage a chaud
      if(ichaud.eq.1) then
      call dset(nbx,0.0d+0,xi,1)
      if (mode.eq.1) then
      call dcopy(m-nbx,y,1,xi(nbx+1),1)
      endif
      if (mode.eq.0) then
      call dcopy(m-nbx,x,1,xi(nbx+1),1)
      endif
      call fft842(0,m,xr,xi,ierr)
      do 181 i=2,lshft
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
 181  continue
      zr(1)=zr(1)+xr(1)*xi(1)
      zr(mhlf1)=zr(mhlf1)+xr(mhlf1)*xi(mhlf1)
      endif
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
 110  continue
      call dcopy(nrdy,x(iss),1,xr,1)
      if (mode.eq.0) then
      call dcopy(nrdy,x(iss),1,xi,1)
      endif
      if(mode.eq.1) then
      call dcopy(nrdy,y(iss),1,xi,1)
      endif
      nrdx1=nrdx+1
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
      zr(j)=zr(i)
      zi(j)=-zi(i)
 200  continue
      nbx=nrdy
      return
      end
