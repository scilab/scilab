getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/add_demo_data.ref','rb');
//create a simple demo script
path = TMPDIR + '/foo.sce';
%ans = mputl('disp Hello', path);
if load_ref('%ans') then   pause,end,

%ans = add_demo('My first demo', path);
if load_ref('%ans') then   pause,end,

//the demo can now be run using the "Demos" menu.
xdel_run(winsid());

mclose(%U);
