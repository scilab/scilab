getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/names_data.ref','rb');
//Valid names
%eps;
if load_ref('%eps') then   pause,end,

A1 = 123;
if load_ref('A1') then   pause,end,

#Color = 8;
if load_ref('#Color') then   pause,end,

My_Special_Color_Table = rand(10, 3);
if load_ref('My_Special_Color_Table') then   pause,end,

//Non valid names
//1A , b%, .C
xdel_run(winsid());

mclose(%U);
