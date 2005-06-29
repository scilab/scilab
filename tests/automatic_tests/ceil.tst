getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/ceil_data.ref','r');
%ans = ceil([1.9,-2.5]) - [2,-2];
if load_ref('%ans') then   pause,end,

%ans = ceil(-%inf);
if load_ref('%ans') then   pause,end,

x = rand() * (10^20);%ans = ceil(x) - x;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
