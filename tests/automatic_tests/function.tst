getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/function_data.ref','rb');
//inline definition (see function)
function [x,y]=myfct(a,b)
  x=a+b
  y=a-b
endfunction
if load_ref('%ans') then   pause,end,


[x,y] = myfct(3, 2);
if load_ref('y') then   pause,end,
if load_ref('x') then   pause,end,


//a one line function definition
function y=sq(x)
  ,y=x^2,
endfunction
if load_ref('%ans') then   pause,end,


%ans = sq(3);
if load_ref('%ans') then   pause,end,


//nested functions definition
function y=foo(x)
  a=sin(x)
  function y=sq(x), y=x^2,endfunction
  y=sq(a)+1
endfunction
if load_ref('%ans') then   pause,end,


%ans = foo(%pi/3);
if load_ref('%ans') then   pause,end,



// definition in an script file (see exec)
exec('SCI/macros/elem/asinh.sci');


xdel_run(winsid());

mclose(%U);
