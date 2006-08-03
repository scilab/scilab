      subroutine proj(n,binf,bsup,x)
      implicit double precision (a-h,o-z)
      dimension binf(n),bsup(n),x(n)
      do 1 i=1,n
1     x(i)=max(binf(i),min(x(i),bsup(i)))
      return
      end
