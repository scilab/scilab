      subroutine fmc11z(a,n,nr,z,sig,w,ir,mk,eps)
      implicit double precision (a-h,o-z)
      dimension a(*),z(n),w(n)
c
      if(nr.eq.n) go to 45
      nr1=nr+1
      nh=nr*(nr1)/2+1
      if(nr.eq.0) go to 25
      do 20 i=1,nr
      do 10 j=nr1,n
      a(nh)=a(nh)+sig*z(i)*z(j)
   10 nh=nh+1
   20 continue
   25 do 40 j=nr1,n
      do 30 i=j,n
      a(nh)=a(nh)+sig*z(i)*z(j)
   30 nh=nh+1
   40 continue
      if(nr.eq.0) return
   45 call fmc11a(a,nr,z,sig,w,ir,mk,eps)
      return
      end
