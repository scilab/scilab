getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/gsort_data.ref','rb');
alr = [1,2,2;
  1,2,1;
  1,1,2;
  1,1,1];
[alr1,k] = gsort(alr, 'lr', 'i');
if load_ref('k') then   pause,end,
if load_ref('alr1') then   pause,end,

[alr1,k] = gsort(alr, 'lc', 'i');
if load_ref('k') then   pause,end,
if load_ref('alr1') then   pause,end,

xdel_run(winsid());

mclose(%U);
