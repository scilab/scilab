getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/power_data.ref','rb');
A = [1,2;3,4];
%ans = A^2.5;
if load_ref('%ans') then   pause,end,

%ans = A.^2.5;
if load_ref('%ans') then   pause,end,

%ans = (1:10)^2;
if load_ref('%ans') then   pause,end,

%ans = (1:10).^2;
if load_ref('%ans') then   pause,end,


s = poly(0, 's');
if load_ref('s') then   pause,end,

%ans = s^(1:10);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
