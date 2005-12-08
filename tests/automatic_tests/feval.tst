getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/feval_data.ref','rb');
deff('[z]=f(x,y)', 'z=x^2+y^2');
%ans = feval(1:10, 1:5, f);
if load_ref('%ans') then   pause,end,

deff('[z]=f(x,y)', 'z=x+%i*y');
%ans = feval(1:10, 1:5, f);
if load_ref('%ans') then   pause,end,

%ans = feval(1:10, 1:5, 'parab');
if load_ref('%ans') then   pause,end,
//See ffeval.f file
%ans = feval(1:10, 'parab');
if load_ref('%ans') then   pause,end,

// For dynamic link (see example ftest in ffeval.f)
// you can use the link command (the parameters depend on the machine):
// unix('make ftest.o');link('ftest.o','ftest); feval(1:10,1:5,'ftest')
xdel_run(winsid());

mclose(%U);
