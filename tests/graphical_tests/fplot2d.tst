getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/fplot2d_data.ref','rb');

%ans = deff('[y]=f(x)', 'y=sin(x)+cos(x)');
if load_ref('%ans') then   pause,end,

x = (0:0.1:10) * %pi/10;
%ans = fplot2d(x, f);
if load_ref('%ans') then   pause,end,

clf_run();
%ans = fplot2d(1:10, 'parab');
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
