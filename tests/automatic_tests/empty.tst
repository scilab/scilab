getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/empty_data.ref','r');
s = poly(0, 's');A = [s,s + 1];
%ans = A + [];
if load_ref('%ans') then   pause,end,
%ans = A * [];
if load_ref('%ans') then   pause,end,

A = rand(2, 2);AA = A([], 1);
if load_ref('AA') then   pause,end,
%ans = size(AA);
if load_ref('%ans') then   pause,end,

%ans = svd([]);
if load_ref('%ans') then   pause,end,

w = ssrand(2, 2, 2);wr = [] * w;%ans = size(wr);
if load_ref('%ans') then   pause,end,
w1 = ss2tf(wr);
if load_ref('w1') then   pause,end,
%ans = size(w1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
