getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/getdate_data.ref','rb');
w = getdate();
if load_ref_nocheck('w') then   pause,end,

// Next line commented out because w  may not have the saved value
//mprintf('Year:%d,Month:%d,Day:%d', w(1), w(2), w(6));

x = getdate('s');
if load_ref_nocheck('x') then   pause,end,

%ans = getdate(x);
if load_ref_nocheck('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
