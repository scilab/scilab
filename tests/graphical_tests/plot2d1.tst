getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/plot2d1_data.ref','rb');

// multiple plot without giving x
x = (0:0.1:2 * %pi)';
%ans = plot2d1('enn', 1, [sin(x),sin(2 * x),sin(3 * x)]);
if load_ref('%ans') then   pause,end,

// multiple plot using only one x
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d1('onn', x, [sin(x),sin(2 * x),sin(3 * x)]);
if load_ref('%ans') then   pause,end,

// logarithmic plot
x = (0.1:0.1:3)';%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d1('oll', x, [exp(x),exp(x^2),exp(x^3)]);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
