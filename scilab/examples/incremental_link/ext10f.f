      subroutine ext10f(n, t, y, ydot)
c     external fonction for ode
c     input variables n, t, y
c     n = dimension of state vector y
c     t = time
c     y = state variable 
c     output variable = ydot
c
c     external routine must 
c     load ydot(1) with d/dt ( y(1)(t) )
c          ydot(2) with d/dt ( y(2)(t) )
c          ...
c     i.e. ydot vector of derivative of state y
c
c     With dynamic link: 
c     link('ext10f.o','ext10f')
c
c     passing a parameter to ext10f routine by a list:
c     -->param=[0.04,10000,3d+7];    
c     -->y=ode([1;0;0],0,[0.4,4],list('ext10f',param))
c     param is retrieved in ext10f by:
c     param(1)=y(n+1) , param(2)=y(n+2) etc 
c     with this calling sequence y is a n+np vector
c     where np=dimension of scilab variable param
c
c     Copyright Inria/Enpc
      double precision t, y, ydot, param
      dimension y(3), ydot(3),param(3)
      param(1)=y(n+1)
      param(2)=y(n+2)
      param(3)=y(n+3)
      ydot(1) = -param(1)*y(1) + param(2)*y(2)*y(3)
      ydot(3) = param(3)*y(2)*y(2)
      ydot(2) = -ydot(1) - ydot(3)
      end
