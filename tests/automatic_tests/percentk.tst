getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/percentk_data.ref','rb');
m = 0.4;
%ans = %asn(1, m);
if load_ref('%ans') then   pause,end,

%ans = %k(m);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
