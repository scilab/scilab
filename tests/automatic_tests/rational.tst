getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/rational_data.ref','rb');
s = poly(0, 's');
W = [1/s,1/(s + 1)];
if load_ref('W') then   pause,end,

%ans = W' * W;
if load_ref('%ans') then   pause,end,

Num = [s,s + 2;1,s];Den = [s * s,s;s,s * s];
%ans = tlist(['r','num','den','dt'], Num, Den, []);
if load_ref('%ans') then   pause,end,

H = Num ./ Den;
if load_ref('H') then   pause,end,

%ans = syslin('c', Num, Den);
if load_ref('%ans') then   pause,end,

%ans = syslin('c', H);
if load_ref('%ans') then   pause,end,

[Num1,Den1] = simp(Num, Den);
if load_ref('Den1') then   pause,end,
if load_ref('Num1') then   pause,end,

xdel_run(winsid());

mclose(%U);
