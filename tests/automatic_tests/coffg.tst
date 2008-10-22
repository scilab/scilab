getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/coffg_data.ref','rb');
s = poly(0, 's');
if load_ref('s') then   pause,end,

a = [s,s^2 + 1;s,s^2 - 1];
[a1,d] = coffg(a);
%ans = a1/d - inv(a);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
