c-----------------------------------------------------------------
c     Examples of argument functions for scilab function ode 
c     The function provided here must be listed 
c     in the file Flist in order to be used with scilab 
c     fydot_list : contains the rhs function
c     fjac_list   : contains the jacobian (if you want to give it)
c                   ( example jex -> jacobian for fex)
c     Examples :
c       fex, jex : first example 
c       fex2     : uses readmat (or matz) to get parameters 
c       fex3     : uses matptr to get parameters 
c       fexab    : a more complex example with matpr
c-----------------------------------------------------------------

c     Copyright INRIA
      subroutine fex (neq, t, y, ydot)
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
c     Example:
c     1/ call this fex routine: 
c     ode([1;0;0],0,[0.4,4],'fex')
c
c     2/ May use dynamic link: 
c     link('fex.o','fex')
c     ode([1;0;0],0,[0.4,4],'fex')
c
c     3/ passing a parameter to fex routine:
c     link('fex.o','fex')    
c     y=ode([1;0;0],0,[0.4,4],list('fex',param))
c     param can be retrieved in fex by:
c     param(1)=y(neq+1) , param(2)=y(neq+2) etc 
c     with this calling sequence y is a nc+nd+np vector
c     where np=dimension of scilab variable param
c
      double precision t, y, ydot
      dimension y(3), ydot(3)
      ydot(1) = -.0400d+0*y(1) + 1.0d+4*y(2)*y(3)
      ydot(3) = 3.0d+7*y(2)*y(2)
      ydot(2) = -ydot(1) - ydot(3)
      return
      end


      subroutine jex (neq, t, y, ml, mu, pd, nrpd)
c     -------------------------------------------
c     fex example continued : we provide here 
c     the jacobian function 
c     jacobian routine jex
c     scilab ode
c     ode([1;0;0],0,[0.4,4],'fex','jex')

      double precision pd, t, y
      dimension y(3), pd(nrpd,3)
      pd(1,1) = -.040d+0
      pd(1,2) = 1.0d+4*y(3)
      pd(1,3) = 1.0d+4*y(2)
      pd(2,1) = .040d+0
      pd(2,3) = -pd(1,3)
      pd(3,2) = 6.0d+7*y(2)
      pd(2,2) = -pd(1,2) - pd(3,2)
      return
      end


      subroutine fex2(neq, t, y, ydot)
c     -------------------------------------------
c     exemple with a call to readmat routine
c     param must be defined as a scilab variable
      double precision t, y, ydot, param
      dimension y(3), ydot(3), param(3)

      call readmat('param'//char(0),m,n,param)
c     *******************************

      ydot(1) = param(1)*y(1) + param(2)*y(2)*y(3)
      ydot(3) = param(3)*y(2)*y(2)
      ydot(2) = -ydot(1) - ydot(3)
      return
      end


      subroutine fex3(neq, t, y, ydot)
c     -------------------------------------------
c     same example with call to matptr
c     param must be defined as a scilab variable
      double precision t, y, ydot, param
      dimension y(3), ydot(3), param(3)

      include '../stack.h'
      call matptr('param'//char(0),m,n,lp)
      if(m.eq.-1) return
c     ***************************
c     param entries are in stk(lp),stk(lp+1),stk(lp+2)
c     m,n = dimensions of param = 3,1 (or 1,3 if row v.)
c     (note that vector param not used in this example)
      ydot(1) = stk(lp)*y(1) + stk(lp+1)*y(2)*y(3)
      ydot(3) = stk(lp+2)*y(2)*y(2)
      ydot(2) = -ydot(1) - ydot(3)
      return
      end


      subroutine fexab(neq, t, x, xdot)
c     -----------------------------------
c    Another example
c     xdot=A*xc+B*u
c     A and B real scilab matrices
c     u=u(t,x) given by function uinput below
c
c     [ User may extract this fexab.f routine 
c       and -->link('fexab.o','fexab') ]
c
c     -->A=..., B=... 
c     -->ode(x0,t0,t,'fexab')
c     
c     or customize this one and re-make scilab
c
c     input variables neq, t, x
c     neq = dimension of state vector x
c     t = time
c     x = state variable 
c     output variable = xdot
c
c     routine must 
c     load xdot(1) with d/dt ( x(1)(t) )
c          xdot(2) with d/dt ( x(2)(t) )
c          ...
c     i.e. xdot vector of derivative of state x
c
c
      double precision t,x(*),xdot(*)
c     set dimension of u here
      double precision u(1)
c
      include '../stack.h'
      call matptr('A'//char(0),m,n,la)
      call dmmul(stk(la),m,x,m,xdot,m,m,m,1)
      call matptr('B'//char(0),m,nb,lb)
      call uinput(t,x,u)
      call dgemm('n','n',m,1,nb,1.0d0,stk(lb),m,u,1,1.0d0,xdot,m)
      end

      subroutine uinput(t,x,u)
      double precision t,x(*),u(*)
      u(1)=sin(3*t)
c     u(2)=...
      end

