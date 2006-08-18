      subroutine quad(a, b1, c, sr, si, lr, li)
c calculate the zeros of the quadratic a*z**2+b1*z+c.
c the quadratic formula, modified to avoid
c overflow, is used to find the larger zero if the
c zeros are real and both zeros are complex
c the smaller real zero is found directly from the
c product of the zero c/a.
      double precision a, b1, c, sr, si, lr, li, b,
     * d, e
      if (a.ne.0.0d+0) go to 20
      sr = 0.0d+0
      if (b1.ne.0.0d+0) sr = -c/b1
      lr = 0.0d+0
   10 si = 0.0d+0
      li = 0.0d+0
      return
   20 if (c.ne.0.0d+0) go to 30
      sr = 0.0d+0
      lr = -b1/a
      go to 10
c compute discriminant avoiding overflow
   30 b = b1/2.0d+0
      if (abs(b).lt.abs(c)) go to 40
      e = 1.0d+0 - (a/b)*(c/b)
      d = sqrt(abs(e))*abs(b)
      go to 50
   40 e = a
      if (c.lt.0.0d+0) e = -a
      e = b*(b/abs(c)) - e
      d = sqrt(abs(e))*sqrt(abs(c))
   50 if (e.lt.0.0d+0) go to 60
c real zeros
      if (b.ge.0.0d+0) d = -d
      lr = (-b+d)/a
      sr = 0.0d+0
      if (lr.ne.0.0d+0) sr = (c/lr)/a
      go to 10
c complex conjugate zeros
   60 sr = -b/a
      lr = sr
      si = abs(d/a)
      li = -si
      return
      end
