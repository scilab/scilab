getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sp2adj_data.ref','rb');
A = sprand(100, 50, 0.05);
[xadj,adjncy,anz] = sp2adj(A);
[n,m] = size(A);
p = adj2sp(xadj, adjncy, anz, [n,m]);
%ans = A - p;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
