getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/percentk_data.ref','r');
m = 0.4;
%ans = %asn(1, m);
if load_ref('%ans') then   pause,end,

%ans = %k(m);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
