getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/trianfml_data.ref','rb');
A = ['1','2';'a','b'];
if load_ref('A') then   pause,end,

W = trianfml([A,string(eye(2, 2))]);
if load_ref('W') then   pause,end,

U = W(:, 3:4);
if load_ref('U') then   pause,end,

a = 5;b = 6;
A = evstr(A);
if load_ref('A') then   pause,end,

U = evstr(U);
if load_ref('U') then   pause,end,

%ans = U * A;
if load_ref('%ans') then   pause,end,

%ans = evstr(W(:, 1:2));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
