getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/pvm_reduce_data.ref','r');
A = rand(5, 5);msgtag = 1;
if load_ref('msgtag') then   pause,end,

[buff,info] = pvm_reduce('Max', A, msgtag, 'Workers', 0);
if load_ref('info') then   pause,end,
if load_ref('buff') then   pause,end,

xdel_run(winsid());

mclose(%U);
