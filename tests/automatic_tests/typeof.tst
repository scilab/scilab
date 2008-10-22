getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/typeof_data.ref','rb');
%ans = typeof(1);
if load_ref('%ans') then   pause,end,

%ans = typeof(poly(0, 'x'));
if load_ref('%ans') then   pause,end,


%ans = typeof(1/poly(0, 'x'));
if load_ref('%ans') then   pause,end,

%ans = typeof(%t);
if load_ref('%ans') then   pause,end,


w = sprand(100, 100, 0.001);
%ans = typeof(w);
if load_ref('%ans') then   pause,end,

%ans = typeof(w == w);
if load_ref('%ans') then   pause,end,


deff('y=f(x)', 'y=2*x');
%ans = typeof(f);
if load_ref('%ans') then   pause,end,


L = tlist(['V','a','b'], 18, 'Scilab');
%ans = typeof(L);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
