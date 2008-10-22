getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/TCL_UpVar_data.ref','rb');
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


%ans = TCL_CreateSlave('InterpBis');
if load_ref('%ans') then   pause,end,

%ans = TCL_SetVar('Scilab', 'Good', 'InterpBis');
if load_ref('%ans') then   pause,end,

%ans = TCL_UpVar('Scilab', 'ScilabBis', 'InterpBis');
if load_ref('%ans') then   pause,end,

%ans = TCL_GetVar('ScilabBis', 'InterpBis');
if load_ref('%ans') then   pause,end,

%ans = TCL_SetVar('Scilab', 'Not good', 'InterpBis');
if load_ref('%ans') then   pause,end,

%ans = TCL_GetVar('ScilabBis', 'InterpBis');
if load_ref('%ans') then   pause,end,

%ans = TCL_SetVar('ScilabBis', 'modified again', 'InterpBis');
if load_ref('%ans') then   pause,end,

%ans = TCL_GetVar('ScilabBis', 'InterpBis');
if load_ref('%ans') then   pause,end,

%ans = TCL_GetVar('Scilab', 'InterpBis');
if load_ref('%ans') then   pause,end,

%ans = TCL_DeleteInterp('InterpBis');
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
