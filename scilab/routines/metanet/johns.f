      subroutine johns(hw,i0,ihw,la1,long,lp1,ls1,m,n,p,pi)
      implicit integer (a-z)
      doubleprecision long(m),pi(n),x,y,infr,wj1,wj2,wj0
      dimension lp1(*),ls1(m),la1(m)
      dimension p(n),hw(n),ihw(n)
      infr=10.e6
      infe=32000
      if (i0 .lt. 0 .or. i0 .gt. n) then
         call erro('bad internal node number')
         return
      endif
      if (lp1(i0).eq.lp1(i0+1)) go to 999
      do 10 i=1,n
         pi(i)=infr
         hw(i)=i
         ihw(i)=i
         p(i)=-infe
 10   continue
      hw(1)=i0
      ihw(i0)=1
      hw(i0)=1
      ihw(1)=i0
      p(i0)=0
      pi(i0)=0
      i=i0
 100  if (lp1(i).eq.lp1(i+1)) go to 130
      do 120 ll=lp1(i),lp1(i+1)-1
         u=la1(ll)
         j=ls1(ll)
         if (p(j).ge.0) go to 120
         x=pi(j)
         y=pi(i)+long(u)
         if (x.le.y) go to 120
         pi(j)=y
         p(j)=-i
         jj=ihw(j)
 110     if (jj.eq.1) go to 120
         jj1=jj/2
         j1=hw(jj1)
         wj1=infr
         if (p(j1).lt.0) wj1=pi(j1)
         if (wj1.le.y) go to 120
         hw(jj1)=j
         hw(jj)=j1
         ihw(j)=jj1
         ihw(j1)=jj
         jj=jj1
         go to 110
 120  continue
 130  i=hw(1)
      hi=i
      k=1
 210  j1=k+k
      j2=j1+1
      if (j1.gt.n) go to 230
      wj1=infr
      hj1=hw(j1)
      if (p(hj1).lt.0) wj1=pi(hj1)
      wj2=infr
      if (j2.gt.n) go to 215
      hj2=hw(j2)
      if (p(hj2).lt.0) wj2=pi(hj2)
 215  if (wj2.lt.wj1) go to 217
      j0=j1
      hj0=hj1
      wj0=wj1
      go to 220
 217  j0=j2
      hj0=hj2
      wj0=wj2
 220  if (wj0.ge.infr) go to 230
      hw(j0)=hi
      hw(k)=hj0
      ihw(hi)=j0
      ihw(hj0)=k
      k=j0
      go to 210
 230  if(p(i).ge.0) go to 999
      p(i)=-p(i)
      go to 100
 999  return
      end

