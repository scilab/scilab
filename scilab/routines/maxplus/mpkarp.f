      double precision function mpkarp(ma,na,a,nela,inda,index)
      double precision a(*)
      double precision mpzero
      integer ma,na,nela,inda(*)
      mpkarp=1000000
c
c      call dset(ma*na,mpzero,r,1)
c      i0=0
c      i1=i0
c      i=1
c      do 10 k=1,nela
c 08      i0=i0+1
c         if(i0-i1.gt.inda(i)) then
c            i1=i0
c            i=i+1
c            goto 08
c         endif
c         j=inda(ma+k)
c         r(1+(i-1)+(j-1)*ma)=a(k)
c 10   continue
      end
