getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/realtime_data.ref','rb');


realtimeinit(1/2);//sets time unit to half a second
realtime(0);//sets current date to 0
for k = 1:10,  realtime(k);mprintf('current time is ' + string(k/2) + 'sec .\r\n');end,

//next instruction outputs a dot each 2 seconds
realtimeinit(2);realtime(0);for k = 1:10,  realtime(k);mprintf('.\r\n');end,

realtimeinit(1);realtime(0);
dt = getdate('s');realtime(10);%ans = getdate('s') - dt;
if load_ref('%ans') then   pause,end,




xdel_run(winsid());

mclose(%U);
