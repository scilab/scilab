getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/mprintf_data.ref','rb');
%ans = mprintf('At iteration %i, Result is:\nalpha=%f', 33, 0.535);
if load_ref('%ans') then   pause,end,


%ans = msprintf('%5.3f %5.3f', 123, 0.732);
if load_ref('%ans') then   pause,end,

%ans = msprintf('%5.3f\n%5.3f', 123, 0.732);
if load_ref('%ans') then   pause,end,


A = rand(5, 2);
// vectorized forms: the format directive needs
// two operand, each column of A is used as an operand.
// and the mprintf function is applied on each row of A
%ans = mprintf('%5.3f\t%5.3f\n', A);
if load_ref('%ans') then   pause,end,


colors = ['red';'green';'blue';'pink';'black'];
RGB = [1,0,0;0,1,0;0,0,1;1,0.75,0.75;0,0,0];
%ans = mprintf('%d\t%s\t%f\t%f\t%f\n', (1:5)', colors, RGB);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
