getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/nnz_data.ref','r');
sp = sparse([1,2;4,5;3,10], [1,2,3]);
%ans = nnz(sp);
if load_ref('%ans') then   pause,end,

a = [1,0,0,0,2];
%ans = nnz(a);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
