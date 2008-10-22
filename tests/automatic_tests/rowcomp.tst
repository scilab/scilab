getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/rowcomp_data.ref','rb');
A = rand(5, 2) * rand(2, 4);// 4 col. vectors, 2 independent.
[X,dim] = rowcomp(A);Xp = X';
%ans = svd([Xp(:, 1:dim),A]);
if load_ref('%ans') then   pause,end,
//span(A) = span(Xp(:,1:dim)
x = A * rand(4, 1);//x belongs to span(A)
y = X * x;
if load_ref('y') then   pause,end,

%ans = norm(y(dim + 1:$))/norm(y(1:dim));
if load_ref('%ans') then   pause,end,
// small
xdel_run(winsid());

mclose(%U);
