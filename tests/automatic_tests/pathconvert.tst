getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/pathconvert_data.ref','r');
%ans = pathconvert('SCI/macros\util/foo.sci', %f, %f, 'u');
if load_ref('%ans') then   pause,end,

%ans = pathconvert('SCI/macros\util/foo.sci', %f, %f, 'w');
if load_ref('%ans') then   pause,end,

%ans = pathconvert('SCI/macros/util/foo.sci', %f, %t, 'w');
if load_ref('%ans') then   pause,end,

%ans = pathconvert('HOME/macros/util/foo.sci', %t, %t, 'w');
if load_ref('%ans') then   pause,end,

%ans = pathconvert('c:/tmp', %f, %t, 'u');
if load_ref('%ans') then   pause,end,

%ans = pathconvert('/cygdrive/c/tmp', %f, %f, 'w');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
