getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/pdiv_data.ref','r');
x = poly(0, 'x');
p1 = (1 + x^2) * (1 - x);p2 = 1 - x;
[r,q] = pdiv(p1, p2);
if load_ref('q') then   pause,end,
if load_ref('r') then   pause,end,

%ans = p2 * q - p1;
if load_ref('%ans') then   pause,end,

p2 = 1 + x;
[r,q] = pdiv(p1, p2);
if load_ref('q') then   pause,end,
if load_ref('r') then   pause,end,

%ans = p2 * q + r - p1;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
