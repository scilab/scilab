getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/acos_data.ref','rb');
x = [1,%i,-1,-%i];
if load_ref('x') then   pause,end,

%ans = cos(acos(x));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
