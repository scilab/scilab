C/MEMBR ADD NAME=QUADSD,SSI=0
      subroutine quadsd(nn, u, v, p, q, a, b)
c divides p by the quadratic  1,u,v placing the
c quotient in q and the remainder in a,b
      double precision p(nn), q(nn), u, v, a, b, c
      integer i
      b = p(1)
      q(1) = b
      a = p(2) - u*b
      q(2) =a
      do 10 i=3,nn
        c = p(i) - u*a - v*b
        q(i) =c
        b = a
        a = c
   10 continue
      return
      end
