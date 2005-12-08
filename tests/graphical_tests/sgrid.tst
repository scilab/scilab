getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/sgrid_data.ref','rb');
H = syslin('c', 352 * poly(-5, 's')/poly([0,0,2000,200,25,1], 's', 'c'));
%ans = evans(H, 100);
if load_ref('%ans') then   pause,end,

%ans = sgrid();
if load_ref('%ans') then   pause,end,

%ans = sgrid(0.6, 2, 7);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
