c     Example for int3d
c     ---------------------
c     Copyright INRIA
      
      subroutine int3dex(x,numfun,funvls)
      integer numfun
      double precision x(3),funvls(numfun)
      funvls(1) = exp(x(1)*x(1)+x(2)*x(2)+x(3)*x(3))
      return
      end




