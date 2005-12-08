getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/square_data.ref','rb');
t = (0:0.1:2 * %pi)';
%ans = plot2d(sin(t), cos(t));
if load_ref('%ans') then   pause,end,

%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = square(-1, -1, 1, 1);
if load_ref('%ans') then   pause,end,

%ans = plot2d(sin(t), cos(t));
if load_ref('%ans') then   pause,end,

%ans = xset('default');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
