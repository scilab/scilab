getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/TCL_CreateSlave_data.ref','rb');
%ans = TCL_CreateSlave('TCLinterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_SetVar('a', 'r', 'TCLinterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_ExistVar('a', 'TCLinterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_ExistVar('a');
if load_ref('%ans') then   pause,end,

%ans = TCL_DeleteInterp('TCLinterp');
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
