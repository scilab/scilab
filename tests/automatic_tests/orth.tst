getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/orth_data.ref','rb');
A = rand(5, 3) * rand(3, 4);
[X,dim] = rowcomp(A);X = X';
%ans = svd([orth(A),X(:, 1:dim)]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
