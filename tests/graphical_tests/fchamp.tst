getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/fchamp_data.ref','rb');

%ans = deff('[xdot] = derpol(t,x)', ['xd1 = x(2)';'xd2 = -x(1) + (1 - x(1)**2)*x(2)';'xdot = [ xd1 ; xd2 ]']);
if load_ref('%ans') then   pause,end,

xf = -1:0.1:1;
yf = -1:0.1:1;
%ans = fchamp(derpol, 0, xf, yf);
if load_ref('%ans') then   pause,end,

%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = fchamp(derpol, 0, xf, yf, 1, [-2,-2,2,2], '011');
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
