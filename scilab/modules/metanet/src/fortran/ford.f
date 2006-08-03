      subroutine ford(i0,la1,long,lp1,ls1,m,n,p,pi)
      implicit integer (a-z)
      dimension lp1(*),ls1(m),p(n),la1(m)
      doubleprecision long(m),pi(n),infr,z
      logical modif
      infe=32700
      infr=10.e6
      if (i0 .lt. 0 .or. i0 .gt. n) then
         call erro('bad internal node number')
         return
      endif
      do 10 i=1,n
         pi(i)=  infr
         p(i)= - infe
 10   continue
      pi(i0)=0.
      p(i0)=0
      k=1
 100  continue
      modif=.false.
      do 160 i=1,n
         if(lp1(i).eq.lp1(i+1))goto 160
         do 150 ll= lp1(i),lp1(i+1)-1
            u=la1(ll)
            j=ls1(ll)
            z=pi(i)+long(u)
            if(z.ge.pi(j))goto 150
            pi(j)=z
            p(j)=i
            modif=.true.
 150     continue
 160  continue
      if(.not.modif)goto 999
      k=k+1
      if(k.lt.n)goto 100
      call erro('negative length circuit')
 999  continue
      end
