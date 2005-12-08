getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/ode_data.ref','rb');
// ---------- Simple one dimension ODE (Scilab function external)
// dy/dt=y^2-y sin(t)+cos(t), y(0)=0
function ydot=f(t,y)
  ,ydot=y^2-y*sin(t)+cos(t),
endfunction
if load_ref('%ans') then   pause,end,

y0 = 0;t0 = 0;t = 0:0.1:%pi;
y = ode(y0, t0, t, f);
if load_ref('y') then   pause,end,

%ans = plot(t, y);
if load_ref('%ans') then   pause,end,


// ---------- Simple one dimension ODE (C coded external)
ccode = ['#include <math.h>';
  'void myode(int *n,double *t,double *y,double *ydot)';
  '{';
  '  ydot[0]=y[0]*y[0]-y[0]*sin(*t)+cos(*t);';
  '}'];
if load_ref('ccode') then   pause,end,

%ans = mputl(ccode, TMPDIR + '/myode.c');
if load_ref('%ans') then   pause,end,
//create the C file
ilib_for_link('myode', 'myode.o', [], 'c', TMPDIR + '/Makefile', TMPDIR + '/loader.sce');//compile
%ans = exec(TMPDIR + '/loader.sce');
if load_ref('%ans') then   pause,end,
//incremental linking
y0 = 0;t0 = 0;t = 0:0.1:%pi;
y = ode(y0, t0, t, 'myode');

// ---------- Simulation of dx/dt = A x(t) + B u(t) with u(t)=sin(omega*t),
// x0=[1;0]
// solution x(t) desired at t=0.1, 0.2, 0.5 ,1.
// A and u function are passed to RHS function in a list.
// B and omega are passed as global variables
function xdot=linear(t,x,A,u)
  ,xdot=A*x+B*u(t),
endfunction
if load_ref('%ans') then   pause,end,

function ut=u(t)
  ,ut=sin(omega*t),
endfunction
if load_ref('%ans') then   pause,end,

A = [1,1;0,2];B = [1;1];omega = 5;
%ans = ode([1;0], 0, [0.1,0.2,0.5,1], list(linear, A, u));
if load_ref('%ans') then   pause,end,


// ---------- Matrix notation Integration of the Riccati differential equation
// Xdot=A'*X + X*A - X'*B*X + C , X(0)=Identity
// Solution at t=[1,2]
function Xdot=ric(t,X)
  ,Xdot=A'*X+X*A-X'*B*X+C,
endfunction
if load_ref('%ans') then   pause,end,

A = [1,1;0,2];B = [1,0;0,1];C = [1,0;0,1];
t0 = 0;t = 0:0.1:%pi;
X = ode(eye(A), 0, t, ric);
if load_ref('X') then   pause,end,

//
// ---------- Matrix notation, Computation of exp(A)
A = [1,1;0,2];
function xdot=f(t,x)
  ,xdot=A*x;,
endfunction
if load_ref('%ans') then   pause,end,

%ans = ode(eye(A), 0, 1, f);
if load_ref('%ans') then   pause,end,

%ans = ode('adams', eye(A), 0, 1, f);
if load_ref('%ans') then   pause,end,


// ---------- Matrix notation, Computation of exp(A) with stiff matrix, Jacobian given
A = [10,0;0,-1];
function xdot=f(t,x)
  ,xdot=A*x,
endfunction
if load_ref('%ans') then   pause,end,

function J=Jacobian(t,y)
  ,J=A,
endfunction
if load_ref('%ans') then   pause,end,

%ans = ode('stiff', [0;1], 0, 1, f, Jacobian);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
