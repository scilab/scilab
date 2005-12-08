getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/rref_data.ref','rb');
A = [1,2;3,4;5,6];
X = rref([A,eye(3, 3)]);
R = X(:, 1:2);
if load_ref('R') then   pause,end,

L = X(:, 3:5);%ans = L * A;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
