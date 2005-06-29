getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/isequal_data.ref','r');
a = [1,2];
if load_ref('a') then   pause,end,

%ans = isequal(a, [1,2]);
if load_ref('%ans') then   pause,end,

%ans = isequal(a, 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
