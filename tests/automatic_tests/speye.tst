getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/speye_data.ref','r');
%ans = eye(3, 3) - full(speye(3, 3));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
