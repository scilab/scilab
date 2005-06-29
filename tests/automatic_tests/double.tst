getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/double_data.ref','r');
x = int8([0,12,140]);
if load_ref('x') then   pause,end,

%ans = double(x);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
