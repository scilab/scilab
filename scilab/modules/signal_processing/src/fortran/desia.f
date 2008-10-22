      subroutine desia(nmaxi,maxdeg,ityp,iapro,om,norma,
     *edeg,ndeg,adelp,adels,
     *nbn,nzero,nzm,vsn,a,adelta,adeg,
     *sm,pren,pimn,ugc,ogc,ack,zm,zzr,zzi,rom,
     *b2,b1,b0,dk,dks,dcap02,dcap04,vsnn,ndegn,nh,vd,nze,ierr)
c!purpose
c filter design -- first section
c!
c
      implicit double precision (a-h,o-z)
      dimension sm(maxdeg,4),nzero(*),pren(*),pimn(*)
      dimension rom(*),nzm(*),zm(maxdeg,4),nze(*)
      double precision  om(*)
      double precision  zzr(*),zzi(*),b0(*),b1(*),b2(*)
c
      sfa = 0.
      call desi00 (ityp,om,norma,edeg,ndeg,adeg,vsn,vd,a)
      call desi01 (maxdeg,iapro,edeg,ndeg,adelp,adels,vsn,
     *adeg,gd1,gd2,acap12,ierr)
      go to (10, 20, 20, 30), iapro
  10  call desi11(nmaxi,maxdeg,vsn,ndeg,gd1,gd2,adelta,nzm,
     *sm,nzero,pren,pimn,ugc,ogc,nj,nh)
      go to 40
  20  call desi12 (nmaxi,maxdeg,iapro,ndeg,vsn,gd1,gd2,adelta,
     *nzm,sm,nzero,pren,pimn,ugc,ack,nj,nh)
      go to 40
  30  call desi14 (nmaxi,maxdeg,ndeg,vsn,gd1,gd2,adelta,nzm,sm,nzero,
     *pren,ugc,ogc,ack,nj,nh,dk,dks,dcap02,dcap04)
c
  40  vsnn = vsn
      ndegn = ndeg
      nbn = nj
      call tranze (nmaxi,maxdeg,ityp,ndeg,nzm,a,vd,sm,nzero)
      call trbize (nmaxi,maxdeg,nzm,nzero,sm,zm,zzr,zzi)
      call blnumz (nzm,nzero,zzr,zzi,b2,b1,b0,nze)
      call romeg (nmaxi,maxdeg,ityp,nzm,zm,rom)
      return
      end
