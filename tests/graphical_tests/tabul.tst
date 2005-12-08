getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/tabul_data.ref','rb');
// first example
X = [2,8,0,3,7,6,8,7,9,1,6,7,7,2,5,2,2,2,9,7];
if load_ref('X') then   pause,end,

m1 = tabul(X);
if load_ref('m1') then   pause,end,

m2 = tabul(X, 'i');
if load_ref('m2') then   pause,end,


// second example
X = ['ba','baba','a','A','AA','a','aa','aa','aa','A','ba'];
if load_ref('X') then   pause,end,

m = tabul(X, 'i');
if load_ref('m') then   pause,end,


// third example
n = 50000;
X = grand(n, 1, 'bin', 70, 0.5);
m = tabul(X, 'i');
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d3(m(:, 1), m(:, 2)/n);
if load_ref('%ans') then   pause,end,

%ans = xtitle('empiral probabilities of B(70,0.5)');
if load_ref('%ans') then   pause,end,


// last example : computes the occurrences of words of the scilab license
text = read(SCI + '/license.txt', -1, 1, '(A)');// read the scilab license
bigstr = strcat(text, ' ');// put all the lines in a big string
sep = [' ',',','.',';','*',':','-','""'];// words separators
words = tokens(bigstr, sep);// cut the big string into words
m = tabul(words);// computes occurrences of each word
[occ,p] = sort(m(2));// sort by decreasing frequencies
results = [m(1)(p),string(occ)];
if load_ref('results') then   pause,end,
// display result
xdel_run(winsid());

mclose(%U);
