getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/overloading_data.ref','rb');
//DISPLAY
%ans = deff('[]=%tab_p(l)', 'disp([['' '';l(3)] [l(2);string(l(4))]])');
if load_ref('%ans') then   pause,end,

%ans = tlist('tab', ['a','b'], ['x';'y'], rand(2, 2));
if load_ref('%ans') then   pause,end,


//OPERATOR
%ans = deff('x=%c_a_s(a,b)', 'x=a+string(b)');
if load_ref('%ans') then   pause,end,

%ans = 's' + 1;
if load_ref('%ans') then   pause,end,


//FUNCTION
%ans = deff('x=%c_sin(a)', 'x=''sin(''+a+'')''');
if load_ref('%ans') then   pause,end,

%ans = sin('2*x');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
