getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/champ1_data.ref','r');

%ans = champ1(-5:5, -5:5, rand(11, 11), rand(11, 11), rect=[-10,-10,10,10], arfact=2);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
