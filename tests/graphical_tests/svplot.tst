getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/svplot_data.ref','rb');
x = logspace(-3, 3);
y = svplot(ssrand(2, 2, 4), x);
xbasc_run();plot2d1('oln', x', 20 * log(y')/log(10));
%ans = xgrid(12);
if load_ref('%ans') then   pause,end,

xtitle('Singular values plot', '(Rd/sec)', 'Db');
xdel_run(winsid());

mclose(%U);
