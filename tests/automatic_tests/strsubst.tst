getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/strsubst_data.ref','rb');
%ans = strsubst('SCI/demos/scicos', 'SCI', '.');
if load_ref('%ans') then   pause,end,

%ans = strsubst('SCI/demos/scicos', '/', ' ');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
