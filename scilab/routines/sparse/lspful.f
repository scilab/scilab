      subroutine lspful(ma,na,nela,inda,r)
c     Copyright INRIA
      integer r(*)
      integer ma,na,nela,inda(*)
c
      call iset(ma*na,0,r,1)
      i0=0
      i1=i0
      i=1
      do 10 k=1,nela
 08      i0=i0+1
         if(i0-i1.le.inda(i)) goto 09
         i1=i0
         i=i+1
         goto 08
 09      j=inda(ma+k)
         r(1+(i-1)+(j-1)*ma)=1
 10   continue
      end
