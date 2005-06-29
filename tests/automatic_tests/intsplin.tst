getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/intsplin_data.ref','r');
t = 0:0.1:%pi;
if load_ref('t') then   pause,end,

%ans = intsplin(t, sin(t));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
