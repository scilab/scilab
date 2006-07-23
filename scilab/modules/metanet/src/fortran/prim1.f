      subroutine prim1(alpha,hw,ihw,la2,lp2,ls2,ma,mm,n,piw,wa)
      implicit integer (a-z)
      dimension lp2(*),la2(mm),ls2(mm)
      doubleprecision wa(ma), piw(n)
      dimension alpha(n)
      dimension hw(n),ihw(n)
      real infr,y
      real wj0,wj1,wj2
      infe=32700
      infr=10.d6
      i0=1
      do 30 i=1,n
         piw(i)=  infr
         alpha(i)= - infe
         hw(i)=i
         ihw(i)=i
 30   continue
      piw(i0)=0.
      alpha(i0)=0
      hw(1)=i0
      ihw(i0)=1
      hw(i0)=1
      ihw(1)=i0
 100  i=1
      imin=hw(1)
      if (alpha(imin).gt.0) go to 999
      hi=imin
 110  j1=i+i
      j2=j1+1
      if (j1.gt.n) go to 130
      wj1=infr
      hj1=hw(j1)
      if (alpha(hj1).lt.0) wj1=piw(hj1)
      wj2=infr
      if (j2.gt.n) go to 115
      hj2=hw(j2)
      if (alpha(hj2).lt.0) wj2=piw(hj2)
 115  if (wj2.lt.wj1) go to 117
      j0=j1
      hj0=hj1
      wj0=wj1
      go to 120
 117  j0=j2
      hj0=hj2
      wj0=wj2
 120  if (wj0.ge.infr) go to 130
      hw(j0)=hi
      hw(i)=hj0
      ihw(hi)=j0
      ihw(hj0)=i
      i=j0
      go to 110
 130  alpha(imin)=-alpha(imin)
      if(lp2(imin).eq.lp2(imin+1))goto 230
      do 220 ll = lp2(imin),lp2(imin+1)-1
         j=ls2(ll)
         if(alpha(j).ge.0) goto 220
         u=la2(ll)
         if(wa(u).ge.piw(j)) goto 220
         alpha(j)=-u
         piw(j)=wa(u)
         jj=ihw(j)
         y=piw(j)
 210     if (jj.eq.1) go to 220
         jj1=jj/2
         j1=hw(jj1)
         wj1=infr
         if (alpha(j1).lt.0) wj1=piw(j1)
         if (wj1.le.y) go to 220
         hw(jj1)=j
         hw(jj)=j1
         ihw(j)=jj1
         ihw(j1)=jj
         jj=jj1
         go to 210
 220  continue
 230  continue
      go to 100
 999  continue
      end
