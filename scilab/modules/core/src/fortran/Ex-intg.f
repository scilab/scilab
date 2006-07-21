c     Example for intg 
c     ---------------------
c     Copyright INRIA
      
      double precision function intgex(x)
      double precision x,pi
      pi=3.1415926520d+0
      intgex=x*sin(30.0d+0*x)/sqrt(1.0d+0-(x/(2.0d+0*pi))**2)
      return
      end



