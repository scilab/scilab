
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file is released under the 3-clause BSD license. See COPYING-BSD.

      subroutine ext8f(n, t, y, ydot)
c     argument function for ode
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
c     Example:
c     call this ext8f routine: 
c     ode([1;0;0],0,[0.4,4],'ext9f')
c
c     With dynamic link: 
c     -->link('ext8f.o','ext8f')
c     -->ode([1;0;0],0,[0.4,4],'ext8f')
c

      double precision t, y, ydot
      dimension y(3), ydot(3)
      ydot(1) = -.0400d+0*y(1) + 1.0d+4*y(2)*y(3)
      ydot(3) = 3.0d+7*y(2)*y(2)
      ydot(2) = -ydot(1) - ydot(3)
      end
