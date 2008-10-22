getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/xls_read_data.ref','rb');
//Decode ole file, extract and open Excel stream
[fd,SST,Sheetnames,Sheetpos] = xls_open('SCI/demos/excel/Test1.xls');
if load_ref('Sheetpos') then   pause,end,
if load_ref('Sheetnames') then   pause,end,
if load_ref('SST') then   pause,end,
if load_ref('fd') then   pause,end,

//Read first data sheet
[Value,TextInd] = xls_read(fd, Sheetpos(1));
if load_ref('TextInd') then   pause,end,
if load_ref('Value') then   pause,end,

//close the excel stream
%ans = mclose(fd);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
