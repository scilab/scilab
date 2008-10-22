getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/findmsvccompiler_data.ref','rb');
msvc = findmsvccompiler();
if load_ref('msvc') then   pause,end,

xdel_run(winsid());

mclose(%U);
