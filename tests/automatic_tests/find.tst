getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/find_data.ref','rb');
A = rand(1, 20);
w = find(A < 0.4);
if load_ref('w') then   pause,end,

%ans = A(w);
if load_ref('%ans') then   pause,end,

w = find(A > 100);
if load_ref('w') then   pause,end,


B = rand(1, 20);
w = find(B < 0.4, 2);
if load_ref('w') then   pause,end,
//at most 2 returned values

H = rand(4, 3, 5);//an hypermatrix
[i,j,k] = find(H > 0.9);
if load_ref('k') then   pause,end,
if load_ref('j') then   pause,end,
if load_ref('i') then   pause,end,


%ans = H(i(1), j(1), k(1));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
