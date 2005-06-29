getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/TCL_UpVar_data.ref','r');

%ans = TCL_SetVar('Scilab', 'OK');
if load_ref('%ans') then   pause,end,

%ans = TCL_UpVar('Scilab', 'ScilabBis');
if load_ref('%ans') then   pause,end,

%ans = TCL_GetVar('ScilabBis');
if load_ref('%ans') then   pause,end,

%ans = TCL_SetVar('Scilab', 'NOK');
if load_ref('%ans') then   pause,end,

%ans = TCL_GetVar('ScilabBis');
if load_ref('%ans') then   pause,end,

%ans = TCL_SetVar('ScilabBis', 'modified');
if load_ref('%ans') then   pause,end,

%ans = TCL_GetVar('ScilabBis');
if load_ref('%ans') then   pause,end,

%ans = TCL_GetVar('Scilab');
if load_ref('%ans') then   pause,end,



xdel_run(winsid());

mclose(%U);
