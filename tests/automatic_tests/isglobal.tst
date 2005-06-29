getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/isglobal_data.ref','r');
%ans = isglobal(1);
if load_ref('%ans') then   pause,end,

%ans = global('a');
if load_ref('%ans') then   pause,end,

%ans = isglobal(a);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
