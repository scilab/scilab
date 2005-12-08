getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/floor_data.ref','rb');
%ans = floor([1.9,-2.5]) - [1,-3];
if load_ref('%ans') then   pause,end,

%ans = floor(-%inf);
if load_ref('%ans') then   pause,end,

x = rand() * (10^20);%ans = floor(x) - x;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
