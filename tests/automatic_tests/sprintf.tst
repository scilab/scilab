getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sprintf_data.ref','rb');
fahr = 120;
if load_ref('fahr') then   pause,end,

%ans = sprintf('%3d Fahrenheit = %6.1f Celsius', fahr, 5/9 * (fahr - 32));
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
