getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/numdiff_data.ref','rb');
function f=myfun(x,y,z,t)
  f=(x+y)^t+z
endfunction
if load_ref('%ans') then   pause,end,


y = 3;z = 4;t = 2;
g = numdiff(list(myfun, 3, 4, 2), 1);
if load_ref('g') then   pause,end,


x = 1;
exact = t * ((x + y)^(t - 1));
if load_ref('exact') then   pause,end,

xdel_run(winsid());

mclose(%U);
