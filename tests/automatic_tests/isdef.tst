getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/isdef_data.ref','r');
A = 1;
%ans = isdef('A');
if load_ref('%ans') then   pause,end,

%ans = clear('A');
if load_ref('%ans') then   pause,end,

%ans = isdef('A');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
