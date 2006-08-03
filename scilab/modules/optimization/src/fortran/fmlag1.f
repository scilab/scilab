      subroutine fmlag1(n,nr,a,z,w)
      implicit double precision (a-h,o-z)
      dimension a(*),z(n),w(n)
c
      if(nr.eq.n)return
      nr1=nr+1
      if(nr.ne.0) go to 20
      do 10 i=nr1,n
   10 w(i)=0.d0
      return
   20 nrr=n-nr
      nh1=nr*nr1/2
      nh=nh1+1
      do 30 j=nr1,n
      u=0.d0
      nj=nh
      do 40 i=1,nr
      u=u+a(nj)*z(i)
   40 nj=nj+nrr
      nh=nh+1
      w(j)=u
   30 continue
      return
      end
