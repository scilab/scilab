getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ss2ss_data.ref','rb');
Sl = ssrand(2, 2, 5);%ans = trzeros(Sl);
if load_ref('%ans') then   pause,end,
// zeros are invariant:
Sl1 = ss2ss(Sl, rand(5, 5), rand(2, 5), rand(5, 2));
%ans = trzeros(Sl1);
if load_ref('%ans') then   pause,end,
%ans = trzeros(rand(2, 2) * Sl1 * rand(2, 2));
if load_ref('%ans') then   pause,end,

// output injection [ A + GC, (B+GD,-G)]
//                  [   C   , (D   , 0)]
p = 1;
if load_ref('p') then   pause,end,
m = 2;
if load_ref('m') then   pause,end,
n = 2;sys = ssrand(p, m, n);

// feedback (m,n)  first and then output injection.

F1 = rand(m, n);
G = rand(n, p);
[sys1,right,left] = ss2ss(sys, rand(n, n), F1, G, 2);

// Sl1 equiv left*sysdiag(sys*right,eye(p,p)))

res = clean(ss2tf(sys1) - ss2tf(left * sysdiag(sys * right, eye(p, p))));
if load_ref('res') then   pause,end,


// output injection then feedback (m+p,n)
F2 = rand(p, n);F = [F1;F2];
[sys2,right,left] = ss2ss(sys, rand(n, n), F, G, 1);

// Sl1 equiv left*sysdiag(sys,eye(p,p))*right

res = clean(ss2tf(sys2) - ss2tf(left * sysdiag(sys, eye(p, p)) * right));
if load_ref('res') then   pause,end,


// when F2= 0; sys1 and sys2 are the same
F2 = 0 * rand(p, n);F = [F1;F2];
[sys2,right,left] = ss2ss(sys, rand(n, n), F, G, 1);

res = clean(ss2tf(sys2) - ss2tf(sys1));
if load_ref('res') then   pause,end,

xdel_run(winsid());

mclose(%U);
