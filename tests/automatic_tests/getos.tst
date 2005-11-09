getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/getos_data.ref','r');
OS = getos();
if load_ref('OS') then   pause,end,

[OS,version] = getos();
if load_ref('version') then   pause,end,
if load_ref('OS') then   pause,end,

xdel_run(winsid());

mclose(%U);
