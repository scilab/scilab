getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/part_data.ref','rb');
c = part(['a','abc','abcd'], [1,1,2]);
if load_ref('c') then   pause,end,

xdel_run(winsid());

mclose(%U);
