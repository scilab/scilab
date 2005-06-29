getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/apropos_data.ref','r');
%ans = apropos('+');
if load_ref('%ans') then   pause,end,

%ans = apropos('ode');
if load_ref('%ans') then   pause,end,

%ans = apropos('list of');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
