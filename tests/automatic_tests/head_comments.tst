getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/head_comments_data.ref','rb');
%ans = head_comments('sinc');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
