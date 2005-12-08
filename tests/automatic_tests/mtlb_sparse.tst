getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/mtlb_sparse_data.ref','rb');
X = sparse(rand(2, 2));Y = mtlb_sparse(X);
Y;
if load_ref('Y') then   pause,end,
%ans = full(Y);
if load_ref('%ans') then   pause,end,
[ij,v,mn] = spget(Y);
if load_ref('mn') then   pause,end,
if load_ref('v') then   pause,end,
if load_ref('ij') then   pause,end,

xdel_run(winsid());

mclose(%U);
