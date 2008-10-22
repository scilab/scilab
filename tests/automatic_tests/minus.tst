getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/minus_data.ref','rb');
%ans = [1,2] - 1;
if load_ref('%ans') then   pause,end,

%ans = [] - 2;
if load_ref('%ans') then   pause,end,


%ans = %s - 2;
if load_ref('%ans') then   pause,end,

%ans = 1/%s - 2;
if load_ref('%ans') then   pause,end,

%ans = 'cat' + 'enate';
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
