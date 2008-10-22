getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/mfscanf_data.ref','rb');
s = '1 1.3';
if load_ref('s') then   pause,end,

[n,a,b] = msscanf(s, '%i %e');
if load_ref('b') then   pause,end,
if load_ref('a') then   pause,end,
if load_ref('n') then   pause,end,

%ans = msscanf(s, '%i %e');
if load_ref('%ans') then   pause,end,


%ans = msscanf(' 12\n', '%c%c%c%c');
if load_ref('%ans') then   pause,end,
//scan characters

%ans = msscanf('0xabc', '%x');
if load_ref('%ans') then   pause,end,
//scan with hexadecimal format

%ans = msscanf('012345abczoo', '%[0-9abc]%s');
if load_ref('%ans') then   pause,end,
//[] notation

//create a file with data
u = mopen(TMPDIR + '/foo', 'w');
t = (0:0.1:%pi)';%ans = mfprintf(u, '%6.3f %6.3f\n', t, sin(t));
if load_ref('%ans') then   pause,end,

mclose(u);

//read the file line by line
u = mopen(TMPDIR + '/foo', 'r');
[n,a,b] = mfscanf(u, '%e %e');
if load_ref('b') then   pause,end,
if load_ref('a') then   pause,end,
if load_ref('n') then   pause,end,

l = mfscanf(u, '%e %e');
if load_ref('l') then   pause,end,

mclose(u);

//use niter
[n,Names,Ages] = msscanf(-1, ['Alain 19';'Pierre 15';'Tom 12'], '%s %d');
if load_ref('Ages') then   pause,end,
if load_ref('Names') then   pause,end,
if load_ref('n') then   pause,end,

D = msscanf(-1, ['Alain 19';'Pierre 15';'Tom 12'], '%s %d');
if load_ref('D') then   pause,end,

%ans = typeof(D);
if load_ref('%ans') then   pause,end,

Names = D(:, 1);
if load_ref('Names') then   pause,end,
//strings
Age = D(:, 2);
if load_ref('Age') then   pause,end,
//numerical values


u = mopen(TMPDIR + '/foo', 'w');
%ans = mfprintf(u, '%s\n', ['Alain 19';'Pierre 15';'Tom 12']);
if load_ref('%ans') then   pause,end,

mclose(u);

u = mopen(TMPDIR + '/foo', 'r');
[n,Names,Ages] = mfscanf(2, u, '%s %d');
if load_ref('Ages') then   pause,end,
if load_ref('Names') then   pause,end,
if load_ref('n') then   pause,end,

mclose(u);


xdel_run(winsid());

mclose(%U);
