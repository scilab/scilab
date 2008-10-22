getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/fcontour_data.ref','rb');

deff('[z]=surf(x,y)', 'z=sin(x)*cos(y)');
t = %pi * (-10:10)/10;

%ans = fcontour(t, t, surf, 10);
if load_ref('%ans') then   pause,end,


xbasc_run();%ans = fcontour(t, t, surf, 10, ebox=[-4,4,-4,4,-1,1], zlev=-1, flag=[0,1,4]);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
