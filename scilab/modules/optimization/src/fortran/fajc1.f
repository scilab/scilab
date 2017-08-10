c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      subroutine fajc1(n,nc,nr,h,w,indi)
c
      implicit double precision (a-h,o-z)
      dimension h(*),w(n),indi(n)
c
      inc=indi(nc)
      nr1=nr+1
      nr2=nr-1
      nrr=n-nr
      nkk=nr-inc
c
c          recherche des composantes de h
      do 260 i=1,nr
      ik=i
      ij=inc
      ii=1
      ko=min0(ik,inc)
      v=0.d0
      if(ko.eq.1) go to 252
      kom1=ko-1
      do 250 k=1,kom1
      nk=nr1-k
      v=v+h(ij)*h(ik)*h(ii)
      ij=ij+nk-1
      ii=ii+nk
  250 ik=ik+nk-1
  252 a=1
      b=1
      if(ko.eq.i) go to 253
      a=h(ik)
  253 if(ko.eq.inc) go to 260
      b=h(ij)
  260 w(i)=v+a*b*h(ii)
c          mise a jour de l
      if(inc.eq.nr) go to 315
      inc1=inc-1
      nh=inc1*nr1-inc1*inc/2+2
      nh1=nh+nkk
      di=h(nh-1)
      do 310 j=1,nkk
      di1=h(nh1)
      nh1=nh1+1
      a=h(nh)
      ai=a*di
      c=(a**2)*di+di1
      h(nh)=c
      nh=nh+1
      if(j.eq.nkk) go to 315
      nkkmj=nkk-j
      do 300 i=1,nkkmj
      h1=h(nh)
      h2=h(nh1)
      u=ai*h1+h2*di1
      h(nh)=u/c
      h(nh1)=-h1+a*h2
      nh=nh+1
      nh1=nh1+1
  300 continue
      nh=nh+1
      di=di*di1/c
  310 continue
c          condensation de la matrice l
  315 nh=inc+1
      nsaut=1
      nj=nr-2
      if(inc.eq.1) nj=nj+1
      if(nr.eq.1) go to 440
      do 430 i=1,nr2
      do 425 j=1,nj
      h(nh-nsaut)=h(nh)
  425 nh=nh+1
      nsaut=nsaut+1
      nh=nh+1
      if(i.eq.inc-1) go to 430
      nj=nj-1
      if(nj.eq.0) go to 440
  430 continue
c          mise a jour de la matrice h
  440 nh=((nr*nr2)/2)+1
      nw=1
      nsaut=nr
      if(inc.eq.1) go to 470
      incm1=inc-1
      do 460 i=1,incm1
      h(nh)=w(nw)
      nw=nw+1
      nsaut=nsaut-1
      if(n.eq.nr) go to 455
      do 450 j=1,nrr
  450 h(nh+j)=h(nh+nsaut+j)
  455 nh=nh+nrr+1
  460 continue
  470 nw=nw+1
      if(nr.eq.n) go to 485
      do 480 i=1,nrr
  480 w(nr+i)=h(nh+nsaut+i-1)
      nsaut=nsaut+nrr
  485 if(inc.eq.nr) go to 510
      do 500 i=1,nkk
      nsaut=nsaut-1
      h(nh)=w(nw)
      nw=nw+1
      if(nr.eq.n) go to 495
      do 490 j=1,nrr
  490 h(nh+j)=h(nh+nsaut+j)
  495 nh=nh+nrr+1
  500 continue
  510 h(nh)=w(inc)
      if(nr.eq.n) go to 540
      do 520 i=1,nrr
  520 h(nh+i)=w(nr+i)
c          mise a jour de indi
  540 do 550 i=1,n
      ii=indi(i)
      if(ii.le.inc.or.ii.gt.nr) go to 550
      indi(i)=ii-1
  550 continue
      indi(nc)=nr
      nr=nr-1
      return
      end
