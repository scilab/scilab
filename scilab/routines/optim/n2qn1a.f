      subroutine n2qn1a (simul,n,x,f,ga,scale,acc,
c     Copyright INRIA
      & df1,mode,niter,nsim,iprint,lp,h,d,w,w1,g,
     & binf,bsup,indi,ibloc,iz,izs,rzs,dzs)
      implicit double precision (a-h,o-z)
      dimension x(n),g(n),scale(n),h(*),d(n),w(n),w1(n)
      dimension binf(n),bsup(n),ga(n),izs(*),iz(*)
      dimension dzs(*),ibloc(n),indi(n)
      real rzs(*)
      external simul,fuclid
 1001 format (40h n2qn1 termine par voeu de l'utilisateur)
 1002 format (25h n2qn1   appel incoherent)
 1003 format (23h n2qn1 bornes initiales,
     & 45h (0 inactive, -1 binf active, +1 bsup active),/
     & (6h n2qn1,17x,5(i5,i3)))
 1006 format (6h n2qn1,i4,6h iters,i6,7h simuls,5h   f=,d15.7,
     &7h   binf,i4,9h  activee)
 1007 format (6h n2qn1,i4,6h iters,i6,7h simuls,5h   f=,d15.7,
     &7h   bsup,i4,9h  activee)
 1010 format (45h n2qn1 remplace le hessien initial (qui n'est,
     & 20h pas defini positif)/27h par une diagonale positive)
 1011 format (38h n2qn1 erreur dans la mise a jour de l)
 1020 format (6h n2qn1,i4,6h iters,i6,7h simuls,5h   f=,d15.7)
 1022 format (6h n2qn1,i4,6h iters,i6,7h simuls,5h   f=,d15.7,
     & 8h   borne,i4,12h  desactivee)
 1024 format (1x)
 1025 format (25h n2qn1 probleme dans bfgs)
c
c      initialisations
c
      alfa=0.7d0
      beta=0.1d0
      prop=1.d0
      nfun=1
      iecri=0
      itr=0
      np=n+1
      indic2=1
      logic=0
      df=df1
      if(mode.le.3) go to 1
      nr=iz(1)
      go to 400
c                calcul des bornes actives
    1 nr=0
      do 14 i=1,n
      if(scale(i).gt.0.d0) go to 2
      mode=2
      write(lp,1002)
      go to 15
    2 bi=bsup(i)
      if(x(i).lt.bi-scale(i)) go to 4
      if(x(i).le.bi) go to 3
      mode=2
      write(lp,1002)
      go to 15
    3 if(ga(i).ge.0.d0 .and. mode.eq.1) go to 13
      ibloc(i)=1
      go to 14
    4 bi=binf(i)
      if(x(i).gt.bi+scale(i)) go to 13
      if(x(i).ge.bi) go to 5
      mode=2
      write(lp,1002)
      go to 15
    5 if(ga(i).le.0.d0 .and. mode.eq.1) go to 13
      ibloc(i)=-1
      go to 14
   13 nr=nr+1
      ibloc(i)=0
   14 continue
      go to 16
   15 niter=1
      acc=0.d0
      nsim=1
      go to 999
c
c      le point de depart est-il optimal?
c
   16 c=0.d0
      dnr=dsqrt(dble(float(nr)))
      acc1=acc*dnr
      do 100 i=1,n
      if(ibloc(i).ne.0)  go to 100
      gi=ga(i)*scale(i)
      c=c+gi*gi
  100 continue
      c=dsqrt(c)
      if(c.gt.acc1) go to 200
      call fcomp1(indic2,ibloc,indi,h,ga,d,w,w1,n,nr,ncs,dga,delta,
     &prop,acc,scale)
      if(ncs.ne.0) go to 102
      itr=1
      mode=1
      go to 900
  102 ibloc(ncs)=0
      nr=nr+1
      go to 200
c
c         initialisation du hessien, en fonction de scale et de df1
c
  200 go to (300,310,320),mode
  300 if(df1.gt.0) go to 301
      mode=2
      write(lp,1002)
      go to 15
  301 c=0.d0
      do 302 i=1,n
      if(ibloc(i).ne.0) go to 302
      gi=ga(i)
      sc=scale(i)
      c=c+gi*gi*sc*sc
  302 continue
      c=0.5d0*c/df1
      do 303 i=1,n
      sc=scale(i)
  303 w(i)=c/(sc*sc)
      nh=n*(n+1)/2
      do 304 i=1,nh
  304 h(i)=0.d0
c      permutation de la matrice h
      nr1=nr+1
      k1=1
      k2=nr+1
      do 306 i=1,n
      if(ibloc(i).ne.0) go to 305
      indi(i)=k1
      k1=k1+1
      go to 306
  305 indi(i)=k2
      k2=k2+1
  306 continue
      mode=1
      call fmani1(mode,n,w,d,indi)
      if(nr.eq.0) go to 308
      k=1
      do 307 i=1,nr
      h(k)=d(i)
  307 k=k+nr1-i
  308 if(nr.eq.n) go to 400
      k=np*nr-nr1*nr/2+1
      do 309 i=nr1,n
      h(k)=d(i)
  309 k=k+np-i
      go to 400
c
c      verification de la definie positivite de h
c      permutation et factorisation
c
  310 call fmc11b(h,n,k)
      if(k.ge.n) go to 312
  311 if(iprint.ne.0) write(lp,1010)
      go to 300
  312 nr=n
      do 313 i=1,n
  313 indi(i)=i
      do 314 i=1,n
      if(ibloc(i).eq.0) go to 314
      nc=i
      call fajc1(n,nc,nr,h,w,indi)
  314 continue
      go to 400
c
c      verification que la diagonale est positive
c
  320 k=1
      do 321 i=1,n
      if(h(k).le.0.d0) go to 311
  321 k=k+np-i
      go to 312
c                  on est pret a y aller
  400 indic2=0
      if(iprint.lt.2) go to 410
      write (lp,1003) (i,ibloc(i),i=1,n)
      write (lp,1024)
  410 dnr=dsqrt(dble(float(nr)))
      acc1=acc*dnr
c
c                   iteration
c
  500 itr=itr+1
      if(itr.ne.1)df=fa-f
      fa=f
      indic1=0
  501 if (itr.le.niter) go to 502
      mode=4
      go to 900
  502 if(iprint.le.2) go to 503
      write(lp,1020) itr,nfun,f
  503 iecri=iecri+1
      if (iecri.ne.-iprint) go to 510
      iecri=0
      indic=1
      call simul(indic,n,x,f,g,izs,rzs,dzs)
c               calcul de la direction de recherche
c               et du test d arret
  510 if(nr.ne.0) go to 511
      indic2=1
      go to 540
  511 mode=1
      call fmani1(mode,n,ga,w,indi)
      wii=0.d0
      do 512 i=1,nr
      wi=w(i)
      wiii=wi*scale(i)
      wii=wii+wiii*wiii
  512 w(i)=-wi
      wii=dsqrt(wii)
      if(wii.gt.acc1) go to 513
      indic2=1
      go to 540
  513 call fmc11e(h,nr,w,w1,nr)
      if(nr.eq.n) go to 520
      nrp1=nr+1
      do 514 i=nrp1,n
  514 w(i)=0.d0
c           calcul de la derivee directionnelle
  520 mode=-1
      call fmani1(mode,n,w,d,indi)
      dga=0.d0
      do 521 i=1,n
  521 dga=dga+ga(i)*d(i)
      if(dga.lt.0.d0) go to 522
      indic2=1
      go to 540
  522 if(indic1.eq.1) go to 550
c               contrainte sortante
  540 call fcomp1(indic2,ibloc,indi,h,ga,w,d,g,n,nr,ncs,
     & dga,delta,prop,acc,scale)
      if(ncs.ne.0) go to 543
      if(indic2.ne.1) go to 541
      mode=1
      go to 900
  541 mode=-1
      call fmani1(mode,n,w,d,indi)
      go to 550
  543 if(iprint.lt.2) go to 544
      write(lp,1022) itr,nfun,f,ncs
  544 indic1=1
      logic=6
c                mise a jour de ibloc et de h
      ibloc(ncs)=0
      call fretc1(mode,n,ncs,nr,h,w,indi,indic2)
      indic2=0
      dnr=dsqrt(dble(float(nr)))
      acc1=acc*dnr
      if(mode.eq.0) go to 511
      mode=7
      if(iprint.ne.0) write(lp,1011)
      go to 900
c                calcul de romax
  550 romax=1.d20
      nca=0
      do 555 i=1,n
      di=d(i)
      if(di.eq.0.d0) go to 555
      if(di.gt.0.d0) go to 552
      bi=binf(i)
      xi=bi-x(i)
      if(-1.d0.ge.di)go to 551
      if(xi.le.(di*1.d20)) go to 555
  551 rocand=xi/di
      i1=-1
      go to 554
  552 bi=bsup(i)
      xi=bi-x(i)
      if(di.ge.1.d0) go to 553
      if(xi.gt.(di*1.d20)) go to 555
  553 rocand=xi/di
      i1=1
  554 if(rocand.gt.romax) go to 555
      nca=i
      romax=rocand
      isign=i1
  555 continue
c                romax est-il nul?
      if(nca.eq.0) go to 570
      if(dabs(romax*d(nca)).le.scale(nca)) go to 560
      go to 570
c               addition d'une contrainte
  560 ibloc(nca)=isign
      indic1=1
      call fajc1(n,nca,nr,h,w,indi)
      if(iprint.ge.2 .and. isign.lt.0) write(lp,1006) itr,nfun,f,nca
      if(iprint.ge.2 .and. isign.gt.0) write(lp,1007) itr,nfun,f,nca
      dnr=dsqrt(dble(float(nr)))
      acc1=acc*dnr
      go to 510
c                recherche lineaire
  570 if((itr.le.n.and.itr.ne.1).
     &and.mode.eq.1) go to 571
      ro=1.d0
      go to 573
  571 if(logic.eq.1) go to 573
      if(logic.ne.6) go to 572
      ro=1.d0
      go to 573
  572 ro=-2.d0*df/dga
  573 roa=ro
      ro=dmin1(ro,romax)
      romin=0.d0
      do 574 i=1,n
      z=d(i)
  574 romin = dmax1(romin,dabs(z/scale(i)))
      romin=1.d0/romin
      call nlis0(n,simul,fuclid,x,f,dga,ro,romin,romax,d,g,
     &alfa,beta,iprint,lp,logic,nfun,nsim,
     &w,izs,rzs,dzs)
      if(iprint.gt.3) write(lp,1024)
      if(logic.le.1) go to 575
      if(logic.eq.6)mode=6
      if(logic.eq.4)mode=5
      if(logic.eq.5)mode=0
      if(logic.eq.7)mode=indic
      go to 900
c               formule de bfgs
  575 theta=1.d0
      if(logic.eq.0) go to 580
      dgaa=0.d0
      do 576 i=1,n
  576 dgaa=dgaa+g(i)*d(i)
      if (dgaa.lt.alfa*dga) theta=alfa*dga/dgaa
  580 mode=1
      call fmani1(mode,n,d,w,indi)
      ir=-nr
      call fmani1(mode,n,ga,d,indi)
      do 581 i=1,nr
  581 d(i)=-d(i)
      call fmlag1(n,nr,h,w,d)
      dga=0.d0
      do 582 i=1,nr
  582 dga=dga-w(i)*d(i)
      call fmc11z(h,n,nr,d,1.d0/dga,w1,ir,1,0.d0)
      ir=-ir
      do 583 i=1,n
      gi=g(i)
      g(i)=theta*gi-ga(i)
  583 ga(i)=gi
      call fmani1(mode,n,g,d,indi)
      dga=0.d0
      do 584 i=1,nr
  584 dga=dga+w(i)*d(i)
      dga=dga*ro
      ro=roa
      call fmc11z(h,n,nr,d,1.d0/dga,w1,ir,0,0.d0)
c                test du rang de la nouvelle
c                sous-matrice active
      if(ir.ge.nr) go to 500
      mode=3
      if(iprint.eq.0) go to 900
      write(lp,1025)
c                ici,tout est termine
  900 if(mode.ne.5.and.mode.ne.3.and.mode.ge.0) go to 910
      indic=4
      call simul(indic,n,x,f,ga,izs,rzs,dzs)
  910 iz(1)=nr
c           calcul de la precision obtenue
      acc=0.d0
      do 920 i=1,n
      if(ibloc(i).ne.0) go to 920
      gi=ga(i)
      acc=acc+gi*gi
  920 continue
      if(dnr.eq.0.d0) go to 921
      acc=dsqrt(acc)/dnr
  921 niter=itr
      nsim=nfun
  999 return
      end
