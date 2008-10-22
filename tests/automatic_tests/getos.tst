getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/getos_data.ref','rb');
OS = getos();
if load_ref('OS') then   pause,end,

[OS,version] = getos();
if load_ref_nocheck('version') then   pause,end,
if load_ref_nocheck('OS') then   pause,end,

xdel_run(winsid());

mclose(%U);
