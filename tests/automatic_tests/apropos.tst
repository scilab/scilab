getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/apropos_data.ref','rb');
%ans = apropos('ode');
if load_ref('%ans') then   pause,end,

%ans = apropos('ode');
if load_ref('%ans') then   pause,end,

%ans = apropos('list of');
if load_ref('%ans') then   pause,end,


//next only for Scilab Browser
global('%browsehelp');%browsehelp_save = %browsehelp;%browsehelp = 'Scilab Browser';

%ans = apropos('sin.*hyperbolic');
if load_ref('%ans') then   pause,end,

%ans = apropos('^ab');
if load_ref('%ans') then   pause,end,
//search help beginning the two characters "ab"
%ans = apropos('quadratic.*solver');
if load_ref('%ans') then   pause,end,


%browsehelp = %browsehelp_save;
xdel_run(winsid());

mclose(%U);
