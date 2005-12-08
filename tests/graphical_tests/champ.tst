getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/champ_data.ref','rb');

// using rect as plot boundaries
%ans = champ(-5:5, -5:5, rand(11, 11), rand(11, 11), rect=[-10,-10,10,10], arfact=2);
if load_ref('%ans') then   pause,end,

// using (x,y) to get boundaries
%ans = clf_run();
if load_ref('%ans') then   pause,end,

%ans = champ(-5:5, -5:5, rand(11, 11), rand(11, 11), 2, [-10,-10,10,10], '021');
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
