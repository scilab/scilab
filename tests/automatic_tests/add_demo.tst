getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/add_demo_data.ref','r');
//create a simple demo script
path = TMPDIR + '/foo.sce';
%ans = mputl('disp Hello', path);
if load_ref('%ans') then   pause,end,

%ans = add_demo('My first demo', path);
if load_ref('%ans') then   pause,end,

//the demo can now be run using the "Demos" menu.
xdel_run(winsid());

mclose(%U);
