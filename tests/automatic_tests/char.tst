getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/char_data.ref','rb');
//Example with a hypermatrix of ascii codes :
x = hypermat([4,2,3], 61:84);
y = char(x);
if load_ref('y') then   pause,end,

%ans = size(x);
if load_ref('%ans') then   pause,end,

%ans = size(y);
if load_ref('%ans') then   pause,end,


//Example with more than one argument :
st1 = 'zeros';
st2 = ['one','two'];
st3 = 'three';
y = char(st1, st2, st3);
if load_ref('y') then   pause,end,

%ans = size(y);
if load_ref('%ans') then   pause,end,

//all strings rows are completed by 'blanks' to have the same length : 6
%ans = length(y);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
