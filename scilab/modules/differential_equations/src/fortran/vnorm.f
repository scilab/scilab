C/MEMBR ADD NAME=VNORM,SSI=0
      double precision function vnorm (n, v, w)
clll. optimize
c%purpose
c-----------------------------------------------------------------------
c this function routine computes the weighted root-mean-square norm
c of the vector of length n contained in the array v, with weights
c contained in the array w of length n..
c   vnorm = sqrt( (1/n) * sum( v(i)*w(i) )**2 )
c-----------------------------------------------------------------------
c!
      integer n,   i
      double precision v, w,   sum
      dimension v(n), w(n)
      sum = 0.0d+0
      do 10 i = 1,n
 10     sum = sum + (v(i)*w(i))**2
      vnorm = sqrt(sum/dble(n))
      return
c----------------------- end of function vnorm -------------------------
      end
