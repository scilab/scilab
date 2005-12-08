getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/round_data.ref','rb');
%ans = round([1.9,-2.5]) - [2,-3];
if load_ref('%ans') then   pause,end,

%ans = round(1.6 + 2.1 * %i) - (2 + 2 * %i);
if load_ref('%ans') then   pause,end,

%ans = round(-%inf);
if load_ref('%ans') then   pause,end,

x = rand() * (10^20);%ans = round(x) - x;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
