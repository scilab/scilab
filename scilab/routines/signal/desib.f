C/MEMBR ADD NAME=DESIB,SSI=0
      subroutine desib(nmaxi,maxdeg,vsnn,ndegn,nbn,ityp,iapro,om,nh,
     *adelp,adels,vd,a,adelta,pren,pimn,ugc,ogc,ack,dk,
     *dks,dcap02,dcap04,acx,spr,spi,zpr,zpi,nb,fact,c1,c0,sm,ierr,ndeg)
c!purpose
c  filter design  --  second section
c!
c
      implicit double precision (a-h,o-z)
c
      double precision sm(maxdeg,4)
      double precision om(*),pren(*),pimn(*)
      double precision spr(*),spi(*)
      double precision zpr(*),zpi(*)
      double precision c1(*),c0(*)
c
      vsn = vsnn
      ndeg = ndegn
      nj = nbn
      go to (10, 20, 20, 30), iapro
  10  call desi21 (ndeg,adelp,adels,adelta,pren,pimn,ugc,ogc,nj,
     *acx,ac,rdelp,rdels,sfa,spr,spi)
      go to 40
  20  call desi22 (iapro,ndeg,adelp,adels,adelta,vsn,pren,pimn,
     *ugc,ogc,ack,nj,nh,acx,ac,rdels,sfa,spr,spi)
      go to 40
  30  call desi24 (ndeg,adelp,adels,adelta,pren,ugc,ogc,ack,nj,
     *nh,dk,dks,dcap02,dcap04,acx,ac,rdelp,rdels,sfa,spr,spi)
c
  40  continue
      call tranpo (nmaxi,maxdeg,ityp,vd,sm,a,ndeg,sfa,spr,spi,nj)
      call trbipo (nmaxi,maxdeg,nj,sfa,sm,spr,spi,zfa,zpr,zpi)
      call bldenz (ndeg,nj,zfa,zpr,zpi,nb,fact,c1,c0)
      return
      end
