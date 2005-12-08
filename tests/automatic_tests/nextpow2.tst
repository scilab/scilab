getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/nextpow2_data.ref','rb');
%ans = nextpow2(127);
if load_ref('%ans') then   pause,end,

%ans = nextpow2(128);
if load_ref('%ans') then   pause,end,

%ans = nextpow2(0:10);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
