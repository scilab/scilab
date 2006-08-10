      subroutine dspmat(ma,na,a,nela,inda,mr,indr,iw)
c     dspmat reshape a sparse matrix
c     Copyright INRIA
      integer inda(*),indr(*),iw(*)
      integer ma,na,nela
      double precision a(*)
c
      nr=(ma*na)/mr
      ja=0
      ki=1
      kj=nela+1
      do 20 i=1,ma
         nira=inda(i)
         if(nira.ne.0) then
            do 10 j=1,nira
               ija=i-1+(inda(ma+ja+j)-1)*ma
               iw(kj)=int(ija/mr)+1
               iw(ki)=ija-mr*(iw(kj)-1)+1
               ki=ki+1
               kj=kj+1
 10         continue
            ja=ja+nira
         endif
 20   continue
      call dij2sp(mr,nr,nela,iw,a,indr,mr+nela,iw(2*nela+1),ierr)
      end
