getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/dirname_data.ref','rb');
files = dirname('SCI/macros/util/poo.sci');
if load_ref_nocheck('files') then   pause,end,

files = dirname('SCI/macros\util/poo.sci');
if load_ref_nocheck('files') then   pause,end,

files = dirname('SCI/macros\util/poo.sci.k');
if load_ref_nocheck('files') then   pause,end,

xdel_run(winsid());

mclose(%U);
