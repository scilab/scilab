getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/errbar_data.ref','rb');
t = (0:0.1:2 * %pi)';
y = [sin(t),cos(t)];x = [t,t];
%ans = plot2d(x, y);
if load_ref('%ans') then   pause,end,

%ans = errbar(x, y, 0.05 * ones(x), 0.03 * ones(x));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
