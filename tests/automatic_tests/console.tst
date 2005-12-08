getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/console_data.ref','rb');
%ans = console('off');
if load_ref('%ans') then   pause,end,

state = console();
if load_ref('state') then   pause,end,

%ans = console();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
