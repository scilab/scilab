getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/fcontour2d_data.ref','rb');
%ans = deff('z=surf(x,y)', 'z=x^4-y^4');
if load_ref('%ans') then   pause,end,

x = -3:0.1:3;
y = x;
fcontour2d(x, y, surf, 10);
xdel_run(winsid());

mclose(%U);
