getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/diophant_data.ref','rb');
s = poly(0, 's');p1 = (s + 3)^2;p2 = 1 + s;
x1 = s;x2 = 2 + s;
[x,err] = diophant([p1,p2], p1 * x1 + p2 * x2);
%ans = p1 * x1 + p2 * x2 - p1 * x(1) - p2 * x(2);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
