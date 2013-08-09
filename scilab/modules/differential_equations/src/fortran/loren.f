c     ====================================       
      subroutine loren (neq, t, y, ydot)
c     ==================================== 
c     lorenz equation 
c     ====================================       
      double precision t, y, ydot,sig,ro,beta
      common / lorenz / sig,ro,beta 
      dimension y(3), ydot(3)
      data sig,ro,beta  /10.0 ,28.0, 2.6666667 /
      
      ydot(1)=sig*(y(2)-y(1))
      ydot(2)=ro*y(1) -y(2)-y(1)*y(3)
      ydot(3)=-beta*y(3)+y(1)*y(2)
      
      return
      end
c     ====================================       