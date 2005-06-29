getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/companion_data.ref','r');
s = poly(0, 's');
p = poly([1,2,3,4,1], 's', 'c');
if load_ref('p') then   pause,end,

%ans = det(s * eye() - companion(p));
if load_ref('%ans') then   pause,end,

%ans = roots(p);
if load_ref('%ans') then   pause,end,

%ans = spec(companion(p));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
