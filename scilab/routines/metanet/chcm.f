      subroutine chcm(capa,i0,la1,lp1,ls1,m,n,pcap,pcapi)
      implicit integer (a-z)
      dimension lp1(*),ls1(m),pcap(n),la1(m)
      doubleprecision capa(m),pcapi(n),infr,x,y
      infe=32700
      infr=10.e6
      if (i0 .lt. 0 .or. i0 .gt. n) then
         call erro('bad internal node number')
         return
      endif
      do 10 i=1,n
         pcapi(i)=0.
         pcap(i)= - infe
 10   continue
      pcapi(i0)= infr
      pcap(i0)=0
      j=i0
 100  continue
      if(lp1(j).eq.lp1(j+1))goto 130
      do 120 ll = lp1(j),lp1(j+1)-1
         i=ls1(ll)
         u=la1(ll)
         if(pcap(i).gt.0) goto 120
         x=pcapi(i)
         if(pcapi(j).le.capa(u)) go to 110
         y=capa(u)
         go to 115
 110     y=pcapi(j)
 115     if (x.ge.y) go to 120
         pcap(i)= -j
         pcapi(i)=y
 120  continue
 130  continue
      pcap(j) = -pcap(j)
      j=0
      x=0.
      do 200 i=1,n
         if(pcap(i).ge.0) goto 200
         if(pcapi(i).le.x) goto 200
         j=i
         x=pcapi(i)
 200  continue
      if(j.gt.0) goto 100
      end
