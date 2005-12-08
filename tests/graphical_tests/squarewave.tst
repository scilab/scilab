getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/squarewave_data.ref','rb');
t = (0:0.1:5 * %pi)';
%ans = plot2d1('onn', t, [2 * sin(t),1.5 * squarewave(t),squarewave(t, 10)]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
