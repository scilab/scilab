      function rednor(n,binf,bsup,x,epsx,g)
c     Copyright INRIA
      implicit double precision (a-h,o-z)
      dimension binf(n),bsup(n),x(n),epsx(n),g(n)
      rednor=0.0d+0
      do 1 i=1,n
      aa=g(i)
      if(x(i)-binf(i).le.epsx(i))aa=min(0.0d+0,aa)
      if(bsup(i)-x(i).le.epsx(i))aa=max(0.0d+0,aa)
1     rednor=rednor + aa**2
      rednor=sqrt(rednor)
      end
