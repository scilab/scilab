getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/TCL_DeleteInterp_data.ref','r');
%ans = TCL_SetVar('Scilab', 'OK');
if load_ref('%ans') then   pause,end,

%ans = TCL_ExistVar('Scilab');
if load_ref('%ans') then   pause,end,

%ans = TCL_DeleteInterp();
if load_ref('%ans') then   pause,end,

%ans = TCL_ExistVar('Scilab');
if load_ref('%ans') then   pause,end,

TCL_CreateSlave('BisInterp');
TCL_ExistInterp('BisInterp');
%ans = TCL_SetVar('Scilab', 'OK', 'BisInterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_ExistVar('Scilab', 'BisInterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_DeleteInterp('BisInterp');
if load_ref('%ans') then   pause,end,

%ans = TCL_ExistVar('Scilab', 'BisInterp');
if load_ref('%ans') then   pause,end,




xdel_run(winsid());

mclose(%U);
