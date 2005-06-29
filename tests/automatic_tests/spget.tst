getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/spget_data.ref','r');
sp = sparse([1,2;4,5;3,10], [1,2,3]);
if load_ref('sp') then   pause,end,

[ij,v,mn] = spget(sp);
xdel_run(winsid());

mclose(%U);
