      subroutine syredi(maxdeg,ityp,iapro,om,adelp,adels,ndeg,
     *         nb,fact,b2,b1,b0,c1,c0,zzr,zzi,zpr,zpi,ierr,
     *          spr,spi,pren,pimn,zm,sm,rom,nzero,nze)
c!purpose
c     computes butterworth, chebyshev, and elliptic approximation;
c!calling sequence
c      subroutine syredi(maxdeg,ityp,iapro,om,adelp,adels,ndeg,
c    *         nb,fact,b2,b1,b0,c1,c0,zzr,zzi,zpr,zpi,ierr,
c    *          spr,spi,pren,pimn,zm,sm,rom,nzero,nze)
c
c     variables d entree
c        integer maxdeg,ityp,iapro
c        double precision om(4),adelp,adels
c     variables de sortie
c        integer ndeg,ierr
c        double precision bo(nb),b1(nb),b2(nb),c0(nb),c1(nb),fact
c        double precision zzr(maxdeg),zzi(maxdeg),spr(maxdeg),spi(maxdeg)
c        nb=(maxdeg+1)/2
c!
c     variables de travail
c         pren,primn,nzero,zm,sm,rom,nzm,nze
      implicit double precision (a-h,o-z)
c
      double precision b2(*),b1(*),b0(*)
      double precision c1(*),c0(*)
      double precision zzr(*),zzi(*),zpr(*),zpi(*)
      double precision spr(*),spi(*)
      double precision om(4)
      double precision pren(*),pimn(*)
      dimension nzm(4),nzero(*),nze(*)
      double precision zm(maxdeg,4),sm(maxdeg,4)
      double precision rom(4),fact,adelp,adels
      nmaxi=(maxdeg+5)/2
      nb=(maxdeg+1)/2
c
      norma=0
      ndeg=0
      edeg=0.0d+0
      acx=0.0d+0
c
      adeg=0.0d+0
      adelta=0.0d+0
      a=0.0d+0
c
      do 10 i=1,4
        rom(i) =0.0d+0
  10  continue
      do 15 i=1,maxdeg
      zzr(i)=0.0d+0
      zzi(i)=0.0d+0
      zpr(i)=0.0d+0
      zpi(i)=0.0d+0
      spr(i)=0.0d+0
      spi(i)=0.0d+0
      pren(i)=0.0d+0
      pimn(i)=0.0d+0
      nzero(i)=0.0d+0
  15  continue
      do 20 i=1,nb
      b2(i)=0.0d+0
      b1(i)=0.0d+0
      b0(i)=0.0d+0
      c1(i)=0.0d+0
      c0(i)=0.0d+0
  20  continue
      do 21 i=1,maxdeg
      do 21 j=1,4
      sm(i,j)=0.0d+0
      zm(i,j)=0.0d+0
  21  continue
c
      ierr=0
      call desia (nmaxi,maxdeg,ityp,iapro,om,norma,
     *edeg,ndeg,adelp,adels,nbn,nzero,nzm,vsn,a,
     *adelta,adeg,sm,pren,pimn,ugc,ogc,ack,zm,zzr,zzi,
     *rom,b2,b1,b0,dk,dks,dcap02,dcap04,vsnn,ndegn,nh,vd,nze,ierr)
      if(2*ndeg.gt.maxdeg) then
         ierr=-9
         return
      endif
      if(ndeg.le.0) then
         ierr=-7
         return
      endif
      call desib(nmaxi,maxdeg,vsnn,ndegn,nbn,ityp,iapro,om,nh,adelp,
     *adels,vd,a,adelta,pren,pimn,ugc,ogc,ack,dk,
     *dks,dcap02,dcap04,acx,spr,spi,zpr,zpi,
     *nb,fact,c1,c0,sm,ierr,ndeg)
c
      return
      end

