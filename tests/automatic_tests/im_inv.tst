getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/im_inv_data.ref','rb');
A = [rand(2, 5);[zeros(3, 4),rand(3, 1)]];B = [[1,1;1,1];zeros(3, 2)];
W = rand(5, 5);A = W * A;B = W * B;
[X,dim] = im_inv(A, B);
if load_ref('dim') then   pause,end,
if load_ref('X') then   pause,end,

%ans = svd([A * X(:, 1:dim),B]);
if load_ref('%ans') then   pause,end,
//vectors A*X(:,1:dim) belong to range(B)
[X,dim,Y] = im_inv(A, B);%ans = [Y * A * X,Y * B];
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
