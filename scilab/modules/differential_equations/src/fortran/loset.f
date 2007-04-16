c     ====================================       
      subroutine loset (sig1,ro1,beta1)
c     ====================================       
c     lorentz equation setup
c     ====================================       
      common / lorentz / sig,ro,beta 
      real sig1,ro1,beta1
      double precision sig,ro,beta
      
      sig = sig1
      ro = ro1
      beta =beta1
      
      return 
      end 
c     ====================================       
      