      subroutine match(beta,c,deg,ex,hp1,hp2,ihp1,ihp2,ind,itmin,l1,l2,
     &     la2,lp2,m1,m2,ma,mm,modif,n,n1dim,ndim,or,p1w,
     &     p2w,pimin,pivw,q,r,s1,s2,sat,sortie,x)
      implicit doubleprecision (a-h,o-z)
      dimension pivw(ndim),pimin(ndim),p1w(ndim),p2w(ndim)
      integer q(ndim),x(ma),sat(ndim)
      integer m1(ndim),m2(ndim),l1(ndim),l2(ndim),ind(ndim)
      integer deg(ndim),s1(ndim),s2(ndim)
      integer beta(ndim),or(ma),ex(ma),lp2(n1dim)
      integer la2(mm)
      dimension c(ma),r(ndim),ihp1(ndim),ihp2(ndim)
      integer hp1(ndim),hp2(ndim),hi,hj1,hj2
      logical modif
      integer delta,smax,card
      logical somsat
      integer sortie,u,som
      doubleprecision infr1,infr2,eps3,eps4
      if(.not.modif) goto 100
      infr1=1.d8
      infr2=2.d8
      eps3=1.d-5
      eps4=1.d-6
      do 10 i=1,n+1
         lp2(i)=0
 10   continue
      do 20 i=1,n
         deg(i)=0
 20   continue
      do 30 u=1,ma
         i=or(u)
         j=ex(u)
         i=beta(i)
         j=beta(j)
         if(i.eq.j) goto 30
         deg(j)=deg(j)+1
         deg(i)=deg(i)+1
 30   continue
      som=1
      do 40 k=1,n
         if(deg(k).eq.0) goto 40
         lp2(k)=som
         som=som+deg(k)
 40   continue
      lp2(n+1)=som
      k=lp2(n+1)
      do 50 i=1,n
         j=n-i+1
         if(deg(j).ne.0) k=lp2(j)
         if(deg(j).eq.0) lp2(j)=k
 50   continue
      k=i+1
      do 60 i=1,n
         deg(i)=0
 60   continue
      do 70 u=1,ma
         i=or(u)
         j=ex(u)
         i=beta(i)
         j=beta(j)
         if(i.eq.j) goto 70
         ll=lp2(i)+deg(i)
         deg(i)=deg(i)+1
         la2(ll)=u
         ll=lp2(j)+deg(j)
         deg(j)=deg(j)+1
         la2(ll)=u
 70   continue
      modif=.false.
 100  continue
      do 110 i=1,n
         sat(i)=0
 110  continue
      do 120 l=1,ma
         if(x(l).eq.0)goto 120
         i=or(l)
         j=ex(l)
         i=beta(i)
         j=beta(j)
         sat(i)=1
         sat(j)=1
 120  continue
      do 130 i=1,n
         if(beta(i).ne.i) goto 130
         if(sat(i).eq.1) goto 130
         pinsat=pivw(i)
         goto 135
 130  continue
 135  continue
      somsat=.true.
      card=0
      do 140 i=1,n
         s1(i)=0
         s2(i)=0
 140  continue
      smax=0
      do 150 i=1,ndim
         hp1(i)=0
         ihp1(i)=0
         ihp2(i)=0
         hp2(i)=0
 150  continue
      ia=1
      ib=1
      do 160 i=1,n
         if(beta(i).ne.i) goto 160
         p1w(i)=-infr1
         m1(i)=i
         m2(i)=i
         hp1(ia)=i
         ihp1(i)=ia
         card=card+1
         ia=ia+1
         if(sat(i).eq.1) goto 160
         p2w(i)=0.
         hp2(ib)=i
         ihp2(i)=ib
         somsat=.false.
         ib=ib+1
 160  continue
      do 170 i=1,n
         if(beta(i).ne.i) goto 170
         if(sat(i).eq.0) goto 170
         p2w(i)=-infr1
         hp2(ib)=i
         ihp2(i)=ib
         ib=ib+1
 170  continue
      if(somsat)goto 400
      pmax=infr1
      if(.not.modif) goto 200
 200  card=2*card+1
 210  zz1=p1w(hp1(1))
 220  continue
      ii1=hp1(1)
      if(hp1(1).eq.0)zz1=-infr2
      zz2=p2w(hp2(1))
      ii2=hp2(1)
      if(hp2(1).eq.0) zz2=-infr2
      zz3=-infr1
      if(smax.eq.0) zz3=pmax
      iii=0
      zmax=-infr2
      if(zz1.gt.zzmax) iii=1
      if(zz1.gt.zzmax) zzmax=zz1
      if(zz2.gt.zzmax) iii=2
      if(zz2.gt.zzmax) zzmax=zz2
      if(zz3.gt.zzmax.and.smax.eq.0) iii=3
      if(zz3.gt.zzmax.and.smax.eq.0) zzmax=zz3
      if(iii.eq.1) goto 230
      if(iii.eq.2) goto 315
      if(iii.eq.3) goto 360
      if(iii.eq.0) then
         sortie=10
         return
      endif
 230  if(sat(ii1).eq.0) goto 235
      if(sat(ii1).eq.1) goto 270
 235  card=card-1
      i=1
 240  hi=hp1(i)
      j1=2*i
      if(j1.gt.ndim)goto 245
      pj1=-infr2
      if(hp1(j1).eq.0) goto 245
      hj1=hp1(j1)
      pj1=p1w(hj1)
 245  j2=2*i+1
      if(j2.gt.ndim)goto 250
      pj2=-infr2
      if(hp1(j2).eq.0) goto 250
      hj2=hp1(j2)
      pj2=p1w(hj2)
 250  pj0=-infr2
      if(pj1.gt.pj0) pj0=pj1
      if(pj2.gt.pj0) pj0=pj2
      if(pj0.eq.-infr2) goto 265
      if(pj0.eq.pj1) goto 255
      if(pj0.eq.pj2) goto 260
 255  hp1(i)=hj1
      ihp1(hj1)=i
      hp1(j1)=hi
      ihp1(hi)=j1
      i=j1
      goto 240
 260  hp1(i)=hj2
      ihp1(hj2)=i
      hp1(j2)=hi
      ihp1(hi)=j2
      i=j2
      goto 240
 265  hp1(i)=0
      eps=abs(pmax*eps3)+eps4
      if(p1w(ii1).le.(pmax+eps))goto 370
      pmax=p1w(ii1)
      imax=ii1
      goto 370
 270  card=card-1
      i=1
 275  hi=hp1(i)
      j1=2*i
      if(j1.gt.ndim)goto 280
      pj1=-infr2
      if(hp1(j1).eq.0)goto 280
      hj1=hp1(j1)
      pj1=p1w(hj1)
 280  j2=2*i+1
      if(j2.gt.ndim)goto 285
      pj2=-infr2
      if(hp1(j2).eq.0) goto 285
      hj2=hp1(j2)
      pj2=p1w(hj2)
 285  pj0=-infr2
      if(pj1.gt.pj0) pj0=pj1
      if(pj2.gt.pj0) pj0=pj2
      if(pj0.eq.-infr2) goto 300
      if(pj0.eq.pj1) goto 290
      if(pj0.eq.pj2) goto 295
 290  hp1(i)=hj1
      ihp1(hj1)=i
      hp1(j1)=hi
      ihp1(hi)=j1
      i=j1
      goto 275
 295  hp1(i)=hj2
      ihp1(hj2)=i
      hp1(j2)=hi
      ihp1(hi)=j2
      i=j2
      goto 275
 300  hp1(i)=0
      do 310 k=lp2(ii1),(lp2(ii1+1)-1)
         l=la2(k)
         if(x(l).eq.0) goto 310
         if(beta(or(l)).ne.ii1)  j=beta(or(l))
         if(beta(ex(l)).ne.ii1)  j=beta(ex(l))
         if(j.eq.ii1) goto 310
         if(beta(j).ne.j) goto 310
         if(beta(ii1).ne.ii1) goto 310
         cmod=c(l)-r(or(l))-r(ex(l))-pivw(ii1)-pivw(j)
         cc=p1w(ii1)-cmod
         eps=abs(p2w(j)*eps3)+eps4
         if(cc.le.(p2w(j)+eps))goto 310
         p2w(j)=cc
         m2(j)=ii1
         l2(j)=l
         jjj=ihp2(j)
 305     if(jjj.eq.1) goto 310
         jjj1=jjj/2
         j1=hp2(jjj1)
         if(p2w(j1).ge.cc) goto 310
         hp2(jjj1)=j
         hp2(jjj)=j1
         ihp2(j)=jjj1
         ihp2(j1)=jjj
         jjj=jjj1
         goto 305
 310  continue
      goto 370
 315  card=card-1
      i=1
 320  hi=hp2(i)
      j1=2*i
      if(j1.gt.ndim)goto 325
      pj1=-infr2
      if(hp2(j1).eq.0)goto 325
      hj1=hp2(j1)
      pj1=p2w(hj1)
 325  j2=2*i+1
      if(j2.gt.ndim) goto 330
      pj2=-infr2
      if(hp2(j2).eq.0) goto 330
      hj2=hp2(j2)
      pj2=p2w(hj2)
 330  pj0=-infr2
      if(pj1.gt.pj0) pj0=pj1
      if(pj2.gt.pj0) pj0=pj2
      if(pj0.eq.-infr2)goto 345
      if(pj0.eq.pj1) goto 335
      if(pj0.eq.pj2) goto 340
 335  hp2(i)=hj1
      ihp2(hj1)=i
      hp2(j1)=hi
      ihp2(hi)=j1
      i=j1
      goto 320
 340  hp2(i)=hj2
      ihp2(hj2)=i
      hp2(j2)=hi
      ihp2(hi)=j2
      i=j2
      goto 320
 345  hp2(i)=0
      do 355 k=lp2(ii2),(lp2(ii2+1)-1)
         l=la2(k)
         if(x(l).eq.2) goto 355
         if(beta(or(l)).ne.ii2)  j=beta(or(l))
         if(beta(ex(l)).ne.ii2)  j=beta(ex(l))
         if(j.eq.ii2) goto 355
         if(beta(j).ne.j) goto 355
         if(beta(ii2).ne.ii2) goto 355
         cmod=c(l)-r(or(l))-r(ex(l))-pivw(ii2)-pivw(j)
         cc=p2w(ii2)+cmod
         eps=abs(p1w(j)*eps3)+eps4
         if(cc.le.(p1w(j)+eps))goto 355
         p1w(j)=cc
         m1(j)=ii2
         l1(j)=l
         jjj=ihp1(j)
 350     if(jjj.eq.1) goto 355
         jjj1=jjj/2
         j1=hp1(jjj1)
         if(p1w(j1).ge.cc) goto 355
         hp1(jjj1)=j
         hp1(jjj)=j1
         ihp1(j)=jjj1
         ihp1(j1)=jjj
         jjj=jjj1
         goto 350
 355  continue
      go to 370
 360  smax=1
      card=card-1
      do 365 i=1,n
         if(beta(i).ne.i) goto 365
         if(sat(i).eq.0) goto 365
         eps=abs(p1w(i)*eps3)+eps4
         if((pmax+pinsat-pivw(i)).lt.(p1w(i)+eps)) goto 365
         p1w(i)=pmax+pinsat-pivw(i)
         m1(i)=i
 365  continue
 370  if(card.ne.0) goto 210
 400  zmax=pmax+2.*pivw(imax)
      if(somsat) zmax=-1.e6
      tmin=1.
      do 410 i=1,n
         if(sat(i).eq.0) goto 410
         if(beta(i).ne.i) goto 410
         h=(p1w(i)-p2w(i))/2.
         if(h.le.10.e-6) goto 410
         t=(pimin(i)-pivw(i))/h
         if(t.gt.tmin) goto 410
         tmin=t
         itmin=i
 410  continue
      do 420 i=1,n
         if(beta(i).ne.i) goto 420
         if(sat(i).eq.0) goto 415
         pivw(i)=pivw(i)+tmin*(p1w(i)-p2w(i))/2.
         goto 420
 415     pivw(i)=pivw(i)+tmin*(p1w(imax)/2.)
         if(pivw(i).lt.0) pivw(i)=0.
 420  continue
      if(.not.(tmin.lt.1.)) goto 425
      sortie=1
      return
 425  if(zmax.gt.0.) goto 430
      sortie=0
      return
 430  do 435 i=1,n
         ind(i)=0
 435  continue
      i1=imax
      ind(i1)=1
 440  i2=m1(i1)
      if(ind(i2).eq.1) goto 500
      ind(i2)=1
      if(m2(i2).eq.i2) goto 450
      i3=m2(i2)
      if(ind(i3).eq.1) goto 445
      ind(i3)=1
      i1=i3
      goto 440
 445  continue
      sortie=20
      return
 450  i1=imax
      delta=2.
 455  i2=m1(i1)
      l=l1(i1)
      x(l)=x(l)+delta
      if(m2(i2).ne.i2) goto 460
      goto 465
 460  i3=m2(i2)
      l=l2(i2)
      x(l)=x(l)-delta
      i1=i3
      goto 455
 465  continue
      goto 100
 500  continue
      jj=i2
      i1=jj
 510  i2=m1(i1)
      l=l1(i1)
      q(i1)=l
      if(i2.eq.jj)goto 515
      i3=m2(i2)
      l=l2(i2)
      q(i2)=l
      i1=i3
      goto 510
 515  sortie=2
      end
