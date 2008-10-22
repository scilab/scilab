getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/odedc_data.ref','rb');
//Linear system with switching input
deff('xdu=phis(t,x,u,flag)', 'if flag==0 then xdu=A*x+B*u; else xdu=1-u;end');
x0 = [1;1];A = [-1,2;-2,-1];B = [1;2];u = 0;nu = 1;stdel = [1,0];u0 = 0;t = 0:0.05:10;
xu = odedc([x0;u0], nu, stdel, 0, t, phis);x = xu(1:2, :);u = xu(3, :);
nx = 2;
plot2d1('onn', t', x', 1:nx, '161');
plot2d2('onn', t', u', nx + 1:nx + nu, '000');
//Fortran external( see fydot2.f):
%ans = norm(xu - odedc([x0;u0], nu, stdel, 0, t, 'phis'), 1);
if load_ref('%ans') then   pause,end,


//Sampled feedback
//
//        |     xcdot=fc(t,xc,u)
//  (system)   |
//        |     y=hc(t,xc)
//
//
//        |     xd+=fd(xd,y)
//  (feedback) |
//        |     u=hd(t,xd)
//
deff('xcd=f(t,xc,xd,iflag)', ['if iflag==0 then ';
  '  xcd=fc(t,xc,e(t)-hd(t,xd));';
  'else ';
  '  xcd=fd(xd,hc(t,xc));';
  'end']);
A = [-10,2,3;4,-10,6;7,8,-10];B = [1;1;1];C = [1,1,1];
Ad = [1/2,1;0,1/20];Bd = [1;1];Cd = [1,1];
%ans = deff('st=e(t)', 'st=sin(3*t)');
if load_ref('%ans') then   pause,end,

%ans = deff('xdot=fc(t,x,u)', 'xdot=A*x+B*u');
if load_ref('%ans') then   pause,end,

%ans = deff('y=hc(t,x)', 'y=C*x');
if load_ref('%ans') then   pause,end,

%ans = deff('xp=fd(x,y)', 'xp=Ad*x + Bd*y');
if load_ref('%ans') then   pause,end,

%ans = deff('u=hd(t,x)', 'u=Cd*x');
if load_ref('%ans') then   pause,end,

h = 0.1;t0 = 0;t = 0:0.1:2;
x0c = [0;0;0];x0d = [0;0];nd = 2;
xcd = odedc([x0c;x0d], nd, h, t0, t, f);
%ans = norm(xcd - odedc([x0c;x0d], nd, h, t0, t, 'fcd1'));
if load_ref('%ans') then   pause,end,
// Fast calculation (see fydot2.f)
plot2d([t',t',t'], xcd(1:3, :)');
xset('window', 2);plot2d2('gnn', [t',t'], xcd(4:5, :)');
xset('window', 0);
xdel_run(winsid());

mclose(%U);
