getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xend_data.ref','rb');
%ans = driver('Pos');
if load_ref('%ans') then   pause,end,

%ans = xinit('foo.ps');
if load_ref('%ans') then   pause,end,

%ans = plot2d();
if load_ref('%ans') then   pause,end,

%ans = xend();
if load_ref('%ans') then   pause,end,

%ans = driver('X11');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
