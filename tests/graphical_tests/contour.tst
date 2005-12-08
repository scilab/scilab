getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/contour_data.ref','rb');
t = linspace(-%pi, %pi, 30);
function z=surf(x,y)
  ,z=x*sin(x)^2*cos(y),
endfunction
if load_ref('%ans') then   pause,end,


%ans = contour(t, t, surf, 10);
if load_ref('%ans') then   pause,end,

// changing the format of the printing of the levels
%ans = xset('fpf', '%.1f');
if load_ref('%ans') then   pause,end,

%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = contour(t, t, surf, 10);
if load_ref('%ans') then   pause,end,

// 3D
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

z = feval(t, t, surf);
plot3d(t, t, z);contour(t, t, z + 0.2 * abs(z), 20, flag=[0,2,4]);
//

xdel_run(winsid());

mclose(%U);
