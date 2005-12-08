getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/flts_data.ref','rb');
sl = syslin('d', 1, 1, 1);u = 1:10;
y = flts(u, sl);
%ans = plot2d2('onn', (1:size(u, 'c'))', y');
if load_ref('%ans') then   pause,end,

[y1,x1] = flts(u(1:5), sl);y2 = flts(u(6:10), sl, x1);
%ans = y - [y1,y2];
if load_ref('%ans') then   pause,end,


//With polynomial D:
z = poly(0, 'z');
D = 1 + z + z^2;p = degree(D);
sl = syslin('d', 1, 1, 1, D);
y = flts(u, sl);[y1,x1] = flts(u(1:5), sl);
y2 = flts(u(5 - p + 1:10), sl, x1);// (update)
%ans = y - [y1,y2];
if load_ref('%ans') then   pause,end,


//Delay (transfer form): flts(u,1/z)
// Usual responses
z = poly(0, 'z');
h = (1 - 2 * z)/(z^2 + 0.3 * z + 1);
if load_ref('h') then   pause,end,

u = zeros(1, 20);u(1) = 1;
imprep = flts(u, tf2ss(h));//Impulse response
%ans = plot2d2('onn', (1:size(u, 'c'))', imprep');
if load_ref('%ans') then   pause,end,

u = ones(1, 20);
stprep = flts(u, tf2ss(h));//Step response
%ans = plot2d2('onn', (1:size(u, 'c'))', stprep');
if load_ref('%ans') then   pause,end,

//
// Other examples
A = [1,2,3;0,2,4;0,0,1];B = [1,0;0,0;0,1];C = eye(3, 3);Sys = syslin('d', A, B, C);
H = ss2tf(Sys);u = [1;-1] * (1:10);
//
yh = flts(u, H);ys = flts(u, Sys);
%ans = norm(yh - ys, 1);
if load_ref('%ans') then   pause,end,

//hot restart
[ys1,x] = flts(u(:, 1:4), Sys);ys2 = flts(u(:, 5:10), Sys, x);
%ans = norm([ys1,ys2] - ys, 1);
if load_ref('%ans') then   pause,end,

//
yh1 = flts(u(:, 1:4), H);yh2 = flts(u(:, 5:10), H, [u(:, 2:4);yh(:, 2:4)]);
%ans = norm([yh1,yh2] - yh, 1);
if load_ref('%ans') then   pause,end,

//with D<>0
D = [-3,8;4,-0.5;2.2,0.9];
Sys = syslin('d', A, B, C, D);
H = ss2tf(Sys);u = [1;-1] * (1:10);
rh = flts(u, H);rs = flts(u, Sys);
%ans = norm(rh - rs, 1);
if load_ref('%ans') then   pause,end,

//hot restart
[ys1,x] = flts(u(:, 1:4), Sys);ys2 = flts(u(:, 5:10), Sys, x);
%ans = norm([ys1,ys2] - rs, 1);
if load_ref('%ans') then   pause,end,

//With H:
yh1 = flts(u(:, 1:4), H);yh2 = flts(u(:, 5:10), H, [u(:, 2:4);yh1(:, 2:4)]);
%ans = norm([yh1,yh2] - rh);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
