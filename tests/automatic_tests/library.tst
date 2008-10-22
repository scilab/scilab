getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/library_data.ref','rb');
// elemlib is a predefined library
elemlib;
if load_ref('elemlib') then   pause,end,
///displays the contents of the library
A = rand(3, 3);
%ans = cosm(A);
if load_ref('%ans') then   pause,end,
//loads cosm and executes it

%ans = whos('-name', 'cosm');
if load_ref('%ans') then   pause,end,
// now cosm is a variable

%ans = elemlib('sinm');
if load_ref('%ans') then   pause,end,
//loads sinm from the library

%ans = elemlib.cosm(A);
if load_ref('%ans') then   pause,end,
//reloads cosm and executes it
xdel_run(winsid());

mclose(%U);
