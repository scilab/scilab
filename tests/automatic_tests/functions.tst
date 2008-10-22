getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/functions_data.ref','rb');
//inline definition (see function)
function [x,y]=myfct(a,b)
  x=a+b
  y=a-b
endfunction
if load_ref('%ans') then   pause,end,


[x,y] = myfct(3, 2);
if load_ref('y') then   pause,end,
if load_ref('x') then   pause,end,


//inline definition (see deff)
%ans = deff('[x,y]=myfct(a,b)', ['x=a+b';
  'y=a-b']);
if load_ref('%ans') then   pause,end,

// definition in an ascii file (see exec)
exec('SCI/macros/elem/asinh.sci');

// definition in an ascii file (see getf)
getf('SCI/macros/elem/asinh.sci');

xdel_run(winsid());

mclose(%U);
