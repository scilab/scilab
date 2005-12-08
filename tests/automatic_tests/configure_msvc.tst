getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/configure_msvc_data.ref','rb');
bOK = configure_msvc();
if load_ref('bOK') then   pause,end,

xdel_run(winsid());

mclose(%U);
