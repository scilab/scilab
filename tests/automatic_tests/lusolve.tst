getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/lusolve_data.ref','rb');
non_zeros = [1,2,3,4];rows_cols = [1,1;2,2;3,3;4,4];
sp = sparse(rows_cols, non_zeros);
[h,rk] = lufact(sp);x = lusolve(h, [1;1;1;1]);%ans = ludel(h);
if load_ref('%ans') then   pause,end,

rk;
if load_ref('rk') then   pause,end,
%ans = sp * x;
if load_ref('%ans') then   pause,end,


non_zeros = [1,2,3,4];rows_cols = [1,1;2,2;3,3;4,4];
sp = sparse(rows_cols, non_zeros);
x = lusolve(sp, -ones(4, 1));
%ans = sp * x;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
