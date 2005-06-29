getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/deff_data.ref','r');
%ans = deff('[x]=myplus(y,z)', 'x=y+z');
if load_ref('%ans') then   pause,end,

//
%ans = deff('[x]=mymacro(y,z)', ['a=3*y+1';'x=a*z+y']);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
