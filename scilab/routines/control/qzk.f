      subroutine qzk(q,a,n,kmax,c)
c ce sous programme determine les coefficients constant ck du quotient
c de la division euclidienne q*z**k/a pour k variant de 1 a kmax
c q polynome de degre n-1
c a polynome de degre n
c ck=(q(n+1-k)-sum(a(n+1-i)*c(k-i)) )/an i=1...min(n,k-1)

c     Copyright INRIA
c
      implicit double precision (a-h,o-z)
      dimension q(*),a(*),c(*)
      an=a(n+1)
      c(1)=q(n)/an
      if(kmax.eq.1) return
      n1=n+1
      k1=min(n,kmax)
      if(n.eq.1) goto 25
      do 20 k=2,k1
      km1=k-1
      tamp=0.0d+0
      do 10 i=1,km1
   10 tamp=tamp+a(n1-i)*c(k-i)
      c(k)=(q(n1-k)-tamp)/an
   20 continue
      if(k1.gt.kmax) return
   25 do 40 k=n1,kmax
      tamp=0.0d+0
      do 30 i=1,n
   30 tamp=tamp+a(n1-i)*c(k-i)
      c(k)=-tamp/an
   40 continue
      return
      end
