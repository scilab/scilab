      subroutine fretc1(mode,n,nc,nr,h,w,indi,indic2)
c     Copyright INRIA
      implicit double precision (a-h,o-z)
      dimension h(*),w(n),indi(n)
c
      inc=indi(nc)
      nr1=nr+1
      nr2=nr-1
      nrr=n-nr
      nii=n-inc
      incmr=inc-nr1
c
c      addition d'une ligne a l
c
c          stockage des elements de la colonne inc dans w
      nsaut=nii+1
      nh=inc*(n+1)-inc*(inc+1)/2
      nw=n
      if(inc.eq.n) go to 20
      do 10 i=1,nii
      w(nw)=h(nh)
      nw=nw-1
   10 nh=nh-1
   20 w(nr1)=h(nh)
      nh=nh-1
      if(inc.eq.nr1) go to 60
      do 40 i=1,incmr
      nl=nii+i-1
      if(nl.eq.0) go to 35
      do 30 j=1,nl
      h(nh+nsaut)=h(nh)
   30 nh=nh-1
   35 w(nw)=h(nh)
      nw=nw-1
      nh=nh-1
   40 nsaut=nsaut+1
      do 50 j=1,incmr
      h(nh+nsaut)=h(nh)
   50 nh=nh-1
c
   60 nw=nw-1
      nsaut=1
      if(nr.eq.0) go to 125
      if(inc.eq.n) go to 80
      do 70 i=1,nii
      h(nh+nsaut)=h(nh)
   70 nh=nh-1
   80 if(nr.eq.1) go to 110
      do 100 i=1,nr2
      w(nw)=h(nh)
      nw=nw-1
      nh=nh-1
      nsaut=nsaut+1
      if(n.eq.nr1) go to 100
      nrm1=n-nr1
      do 90 j=1,nrm1
      h(nh+nsaut)=h(nh)
   90 nh=nh-1
  100 continue
  110 w(nw)=h(nh)
      nh=nh-1
      nsaut=nsaut+1
      if(inc.eq.nr1) go to 125
      incmr=inc-nr1
      do 120 i=1,incmr
      h(nh+nsaut)=h(nh)
  120 nh=nh-1
c         mise a jour de l
  125 if(nr.ne.0) go to 130
      if(w(1).gt.0.d0) go to 220
      mode=-1
      return
  130 if(nr.eq.1) go to 160
      do 150 i=2,nr
      ij=i
      i1=i-1
      v=w(i)
      do 140 j=1,i1
      v=v-h(ij)*w(j)
  140 ij=ij+nr-j
  150 w(i)=v
  160 ij=1
      v=w(nr1)
      do 170 i=1,nr
      wi=w(i)
      hij=h(ij)
      v=v-(wi**2)/hij
      w(i)=wi/hij
  170 ij=ij+nr1-i
      if(v.gt.0.d0) go to 180
      mode=-1
      return
  180 w(nr1)=v
      if(indic2.ne.1) go to 190
      do 185 i=1,nr
  185 w(i)=0.d0
      if(n.eq.nr1) go to 190
      nr1p1=nr1+1
      do 187 i=nr1p1,n
  187 w(i)=0.d0
c          stockage de w dans h
  190 nh=nr*(nr+1)/2
      nw=nr1
      nsaut=nw
      h(nh+nsaut)=w(nw)
      nw=nw-1
      nsaut=nsaut-1
      if(nr.eq.1) go to 220
      do 210 i=1,nr2
      h(nh+nsaut)=w(nw)
      nw=nw-1
      nsaut=nsaut-1
      do 200 j=1,i
      h(nh+nsaut)=h(nh)
  200 nh=nh-1
  210 continue
  220 h(nr1)=w(1)
      if(n.eq.nr1) go to 233
      nh1=nr*(n+1)-nr*(nr+1)/2+1
      nw=nr1
      nmr1=n-nr1
      do 230 i=1,nmr1
  230 h(nh1+i)=w(nw+i)
c          mise a jour de indi
  233 do 235 i=1,n
      ii=indi(i)
      if(ii.le.nr.or.ii.ge.inc) go to 235
      indi(i)=ii+1
  235 continue
      nr=nr+1
      indi(nc)=nr
      mode=0
      return
      end
