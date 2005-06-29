getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/configure_msvc_data.ref','r');
bOK = configure_msvc();
if load_ref('bOK') then   pause,end,

xdel_run(winsid());

mclose(%U);
