getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/convol_data.ref','rb');
x = 1:3;
h1 = [1,0,0,0,0];h2 = [0,1,0,0,0];h3 = [0,0,1,0,0];
x1 = convol(h1, x);
if load_ref('x1') then   pause,end,
x2 = convol(h2, x);
if load_ref('x2') then   pause,end,
x3 = convol(h3, x);
if load_ref('x3') then   pause,end,

%ans = convol(h1 + h2 + h3, x);
if load_ref('%ans') then   pause,end,

p1 = poly(x, 'x', 'coeff');
if load_ref('p1') then   pause,end,

p2 = poly(h1 + h2 + h3, 'x', 'coeff');
if load_ref('p2') then   pause,end,

%ans = p1 * p2;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
