getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/graphic/contour_data.ref','r');
t = %pi * (-10:10)/10;
deff('[z]=surf(x,y)', 'z=sin(x)*cos(y)');z = feval(t, t, surf);
rect = [-%pi,%pi,-%pi,%pi,-1,1];
%ans = contour(t, t, z, 10, 35, 45, ' ', [0,1,0], rect);
if load_ref('%ans') then   pause,end,

// changing the format of the printing of the levels
%ans = xset('fpf', '%.2f');
if load_ref('%ans') then   pause,end,

%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = contour(t, t, z, 10, flag=[0,1,0], ebox=rect);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
