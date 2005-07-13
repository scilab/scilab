getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/graphic/param3d_data.ref','r');

t = 0:0.1:5 * %pi;
%ans = param3d(sin(t), cos(t), t/10, 35, 45, 'X@Y@Z', [2,3]);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
