      subroutine calmaj(dh,n,g1,sig,w,ir,mk,epsmc,nfac)
c     Copyright INRIA
      implicit double precision (a-h,o-z)
c     subroutine de qnbd
      dimension dh(*),g1(*),w(*)
      if(nfac.eq.n) go to 50
      nfac1=nfac+1
      nnfac=n-nfac
      n2fac=(nfac*nfac1)/2
      do 10 i=1,n
10    w(i)=g1(i)*sig
      k=n2fac
      if(nfac.eq.0)go to 25
      do 20 j=1,nfac
      do 20 i=nfac1,n
      k=k+1
      dh(k)=dh(k)+g1(i)*w(j)
20    continue
25    k=n2fac+nfac*nnfac
      do 30 j=nfac1,n
      do 30 i=j,n
      k=k+1
      dh(k)=dh(k) + g1(i)*w(j)
30    continue
50    ir=nfac
      if(nfac.eq.0)return
      call majour(dh,g1,w,nfac,sig,ir,mk,epsmc)
      return
      end
