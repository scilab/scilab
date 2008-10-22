getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/varn_data.ref','rb');
//
s = poly(0, 's');p = [s^2 + 1,s];
%ans = varn(p);
if load_ref('%ans') then   pause,end,

%ans = varn(p, 'x');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
