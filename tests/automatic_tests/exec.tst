getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/exec_data.ref','r');
// create a script file
%ans = mputl('a=1;b=2', TMPDIR + '/myscript');
if load_ref('%ans') then   pause,end,

// execute it
%ans = exec(TMPDIR + '/myscript');
if load_ref('%ans') then   pause,end,

who;
if load_ref('who') then   pause,end,


// create a function
%ans = deff('y=foo(x)', 'a=x+1;y=a^2');
if load_ref('%ans') then   pause,end,

%ans = clear('a', 'b');
if load_ref('%ans') then   pause,end,

// call the function
%ans = foo(1);
if load_ref('%ans') then   pause,end,

// a is a variable created in the environment of the function foo
//    it is destroyed when foo returns
who;
if load_ref('who') then   pause,end,


x = 1;
if load_ref('x') then   pause,end,
//create x to make it known by the script foo
%ans = exec(foo);
if load_ref('%ans') then   pause,end,

// a and y are created in the current environment
who;
if load_ref('who') then   pause,end,

xdel_run(winsid());

mclose(%U);
