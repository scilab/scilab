getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/det_data.ref','r');
x = poly(0, 'x');
%ans = det([x,1 + x;2 - x,x^2]);
if load_ref('%ans') then   pause,end,

w = ssrand(2, 2, 4);%ans = roots(det(systmat(w)));
if load_ref('%ans') then   pause,end,
%ans = trzeros(w);
if load_ref('%ans') then   pause,end,
//zeros of linear system
A = rand(3, 3);
%ans = det(A);
if load_ref('%ans') then   pause,end,
%ans = prod(spec(A));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
