getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/TCL_DeleteInterp_data.ref','rb');

%ans = TCL_SetVar('Scilab', 'OK');
if load_ref('%ans') then   pause,end,

%ans = TCL_ExistVar('Scilab');
if load_ref('%ans') then   pause,end,

%ans = TCL_DeleteInterp();
if load_ref('%ans') then   pause,end,

%ans = TCL_ExistVar('Scilab');
if load_ref('%ans') then   pause,end,

%ans = TCL_CreateSlave('BisInterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_ExistInterp('BisInterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_SetVar('Scilab', 'OK', 'BisInterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_ExistVar('Scilab', 'BisInterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_DeleteInterp('BisInterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_ExistInterp('BisInterp');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
