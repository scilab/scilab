c-----------------------------------------------------------------
c     Examples of argument functions for scilab function ode 
c     Examples : wfex, wjex = rhs function, jacobian
c-----------------------------------------------------------------

c     Copyright INRIA
      subroutine wfex (neq, t, y, ydot)
c     -------------------------------------------
c     input variables neq, t, y
c     neq = dimension of state vector y
c     t = time
c     y = state variable 
c     output variable = ydot
c
c     argument routine must 
c     load ydot(1) with d/dt ( y(1)(t) )
c          ydot(2) with d/dt ( y(2)(t) )
c          ...
c     i.e. ydot vector of derivative of state y
c
c     Usage:
c     1/ dynamic link: 
c     -->link('wfex.o','wfex')
c     ode([1;0;0],0,[0.4,4],'wfex')
c
c     2/ passing a parameter to wfex routine:
c     link('wfex.o','wfex')    
c     y=ode([1;0;0],0,[0.4,4],list('wfex',param))
c     param can be retrieved in wfex by:
c     param(1)=y(neq+1) , param(2)=y(neq+2) etc 
c     with this calling sequence y is a neq+np vector
c     where np=dimension of scilab variable param
c     see ../../link-examples/ext10f.f
c
      double precision t, y, ydot
      dimension y(3), ydot(3)
      ydot(1) = -.0400d+0*y(1) + 1.0d+4*y(2)*y(3)
      ydot(3) = 3.0d+7*y(2)*y(2)
      ydot(2) = -ydot(1) - ydot(3)
      return
      end


