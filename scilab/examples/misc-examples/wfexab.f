      subroutine wfexab(neq, t, x, xdot)
c     -----------------------------------
c     xdot=A*xc+B*u
c     A and B real scilab matrices
c     u=u(t,x) given by function uinput below
c
c     Usage:
c
c     -->link('wfexab.o','wfexab') ]
c
c     -->A=..., B=... 
c     -->ode(x0,t0,t,'wfexab')
c     
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
c     Copyright INRIA
      double precision t,x(*),xdot(*)
c     set dimension of u here
      double precision u(1)
c
      include '../../routines/stack.h'
      call matptr('A'//char(0),m,n,la)
      call dmmul(stk(la),m,x,m,xdot,m,m,m,1)
      call matptr('B'//char(0),m,nb,lb)
      call sinput(t,x,u)
      call dgemm('n','n',m,1,nb,1.0d0,stk(lb),m,u,1,1.0d0,xdot,m)
      end

      subroutine sinput(t,x,u)
      double precision t,x(*),u(*)
      u(1)=sin(3*t)
c     u(2)=...
      end
