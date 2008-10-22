C/MEMBR ADD NAME=GEE,SSI=0
      double precision function gee(k,n,ad,x,y,grid)
c!purpose
c   function to evaluate the frequency response using the
c   lagrange interpolation formula in the barycentric form
c!
      dimension ad(*),x(*),y(*)
      dimension grid(*)
      double precision p,c,d,xf
      double precision pi2
      double precision ad,dev,x,y
      common /rem001/ pi2,dev,nfcns,ngrid
c
      p=0.0d+0
      xf=grid(k)
      xf=cos(pi2*xf)
      d=0.0d+0
      do 1 j=1,n
      c=xf-x(j)
      c=ad(j)/c
      d=d+c
    1 p=p+c*y(j)
      gee=p/d
      return
      end
