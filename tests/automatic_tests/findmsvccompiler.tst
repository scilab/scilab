getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/findmsvccompiler_data.ref','r');
msvc = findmsvccompiler();
if load_ref('msvc') then   pause,end,

xdel_run(winsid());

mclose(%U);
