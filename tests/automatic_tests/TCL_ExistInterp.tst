getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/TCL_ExistInterp_data.ref','rb');
%ans = TCL_ExistInterp('SlaveInterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_CreateSlave('SlaveInterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_ExistInterp('SlaveInterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_DeleteInterp('SlaveInterp');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
