      subroutine dijkst(i0,la1,long,lp1,ls1,m,n,p,pi)
      implicit integer (a-z)
      dimension lp1(*),ls1(m),la1(m),p(n)
      doubleprecision pi(n),long(m),infr,x,y
      if (i0 .lt. 0 .or. i0 .gt. n) then
         call erro('bad internal node number')
         return
      endif
      infr=10.e6
      infe=32700
      do 10 i=1,n
         pi(i)=  infr
         p(i)= - infe
 10   continue
      pi(i0)=0
      p(i0)=0
      j=i0
 100  continue 
      if(lp1(j).eq.lp1(j+1))goto 130
      do 120 ll = lp1(j),lp1(j+1)-1
         u=la1(ll)
         i=ls1(ll)
         if(p(i).gt.0) goto 120
         x=pi(i)
         y=pi(j) + long(u)
         if(x.le.y) goto 120
         p(i)= -j
         pi(i)=y
 120  continue
 130  continue
      p(j) = -p(j)
      j=0
      x=infr
      do 200 i=1,n
         if(p(i).ge.0) goto 200
         if(pi(i).ge.x) goto 200
         j=i
         x=pi(i)
 200  continue
      if(j.gt.0) goto 100
      end
