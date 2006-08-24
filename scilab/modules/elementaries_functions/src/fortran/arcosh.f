      double precision function arcosh(x)
c!but
c calcule l'arcosinus hyperbolique d'un double precision
c!liste d'appel
c     double precision function arcosh(x)
c     double precision x
c!
c     Copyright INRIA
      double precision x
      if (x.lt.1.0d+0) go to 10
      arcosh = log(x+sqrt(x*x-1.0d+0))
      return
10    arcosh = 0.0d+0
      return
      end
      double precision function arsinh(x)
c!but
c calcule l'arcsinus hyperbolique d'un double precision
c!liste d'appel
c     double precision function arsinh(x)
c     double precision x
c!
      double precision x
c
      arsinh = log(x+sqrt(x*x+1.0d+0))
      return
      end
