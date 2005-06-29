getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/TCL_UnsetVar_data.ref','r');

%ans = TCL_SetVar('Scilab', 'OK');
if load_ref('%ans') then   pause,end,

%ans = TCL_GetVar('Scilab');
if load_ref('%ans') then   pause,end,

%ans = TCL_UnsetVar('Scilab');
if load_ref('%ans') then   pause,end,

%ans = TCL_ExistVar('Scilab');
if load_ref('%ans') then   pause,end,



xdel_run(winsid());

mclose(%U);
