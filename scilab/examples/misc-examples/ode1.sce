//Copyright INRIA
Eps=1.e-2
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
//  1. fortran program wfex and wjex dynamically called
files=G_make(["/tmp/wfex.o"],"wfex.dll");
iwfex=link(files,"wfex");
files=G_make(["/tmp/wjex.o"],"wjex.dll");
iwjex=link(files,"wjex");
//     jacobian is not given
y1=ode(y0,t0,t1,'wfex');

//  2. fortran called type given (stiff), no jacobian
y2=ode('stiff',y0,t0,t1,'wfex');
//  3. fortran called type (stiff) given
y3=ode('stiff',y0,t0,t1,'wfex','wjex');
//   hot restart
[z,w,iw]=ode('stiff',y0,0,0.4,'wfex','wjex');
z=ode('stiff',z,0.4,4,'wfex','wjex',w,iw);
if max(z-y3(:,2))  > Eps then pause,end

[y1,w,iw]=ode(y0,t0,t1(1),'wfex');
y2=ode(y0,t1(1),t1(2:nt),'wfex',w,iw);
if max([y1 y2]-yref)  > Eps then pause,end

[y1,w,iw]=ode(y0,t0,t1(1),'wfex','wjex');
y2=ode(y0,t1(1),t1(2:nt),'wfex','wjex',w,iw);
if max([y1 y2]-yref)  > Eps then pause,end

// Unlink wfex and wjex

ulink(iwfex);
ulink(iwjex);
//   Setting tolerances
atol=[0.001,0.0001,0.001];rtol=atol;
//   
//  4. type given , scilab lhs ,jacobian not passed
y4=ode('stiff',y0,t0,t1(1),atol,rtol,f);
if maxi(y4(:,1)-yref(:,1))  > Eps then pause,end

//  5. type non given,  scilab rhs and jacobian functions
y5=ode(y0,t0,t1,f,j);
if max(y5-yref)  > Eps then pause,end
//  6. type given (stiff),rhs and jacobian  by scilab
y6=ode('stiff',y0,t0,t1,0.00001,0.00001,f,j);
if max(y6-yref)  > Eps then pause,end

//  7. matrix rhs, type given (adams),jacobian not passed
// 
a=rand(3,3);ea=expm(a);
deff('[ydot]=f(t,y)','ydot=a*y')
t1=1;y=ode('adams',eye(a),t0,t1,f);
if max(ea-y)  > Eps then pause,end




