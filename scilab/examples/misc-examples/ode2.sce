//Copyright INRIA
Eps =1.e-5
//     Example of use of ode function:
//     System to solve:
//     dy1/dt = -0.04*y1 + 1.e4*y2*y3
//     dy2/dt = 0.04*y1 - 1.e4*y2*y3 - 3.e7*y2**2
//     dy3/dt = 3.e7*y2**2
// on the interval from t = 0.0 to t = 4.e10, with initial conditions
// y1 = 1.0, y2 = y3 = 0.  the problem is stiff.
//     Scilab definition of rhs function:
deff('[ydot]=f(t,y)',...
     'f1=-0.04*y(1)+1e4*y(2)*y(3),...
      f3=3e7*y(2)*y(2),...
      ydot=[f1;-f1-f3;f3]')
//     Definition of jacobian
deff('[jac]=j(t,y)',...
     'jac(1,1)=-0.04;jac(1,2)=1.e4*y(3);jac(1,3)=1.e4*y(2),...
      jac(3,1)=0;jac(3,2)=6.e7*y(2);jac(3,3)=0;...
      jac(2,1)=-jac(1,1);jac(2,2)=-jac(1,2)-jac(3,2);jac(2,3)=-jac(1,3);')
//    y0= Initial state  t1= times at which sol. is computed
y0=[1;0;0];t0=0;t1=[0.4,4];nt=size(t1,'*');
//    Reference solution is:
yref=[0.9851721 0.9055180;0.0000339 0.0000224;0.0147940 0.0944596];
//
//  Fortran program called in "routines/default" directory (Ex-ode.f)
//     jacobian is not given
y1=ode(y0,t0,t1,'fex');
if norm(yref-y1) > Eps then pause,end
//     jacobian is given
y2=ode(y0,t0,t1,'fex','jex');
if norm(yref-y2) > Eps then pause,end





