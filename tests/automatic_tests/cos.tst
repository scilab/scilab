getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/cos_data.ref','r');
x = [0,1,%i];
if load_ref('x') then   pause,end,

%ans = acos(cos(x));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
