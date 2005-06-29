getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/boolean_data.ref','r');
%ans = [1,2] == [1,3];
if load_ref('%ans') then   pause,end,

%ans = [1,2] == 1;
if load_ref('%ans') then   pause,end,

a = 1:5;%ans = a(a > 2);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
