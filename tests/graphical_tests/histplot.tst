getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/histplot_data.ref','rb');
// example #1: variations around an histogram of a gaussian random sample
d = rand(1, 10000, 'normal');// the gaussian random sample
clf_run();%ans = histplot(20, d);
if load_ref('%ans') then   pause,end,

clf_run();%ans = histplot(20, d, normalization=%f);
if load_ref('%ans') then   pause,end,

clf_run();%ans = histplot(20, d, leg='rand(1,10000,''normal'')', style=5);
if load_ref('%ans') then   pause,end,

clf_run();histplot(20, d, leg='rand(1,10000,''normal'')', style=16, rect=[-3,0,3,0.5]);

// example #2: histogram of a binomial (B(6,0.5)) random sample
d = grand(1000, 1, 'bin', 6, 0.5);
c = linspace(-0.5, 6.5, 8);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 1, 1);
if load_ref('%ans') then   pause,end,

%ans = histplot(c, d, style=2);
if load_ref('%ans') then   pause,end,

%ans = xtitle('normalized histogram');
if load_ref('%ans') then   pause,end,

%ans = subplot(2, 1, 2);
if load_ref('%ans') then   pause,end,

%ans = histplot(c, d, normalization=%f, style=5);
if load_ref('%ans') then   pause,end,

%ans = xtitle('non normalized histogram');
if load_ref('%ans') then   pause,end,


// example #3: histogram of an exponential random sample
lambda = 2;
X = grand(100000, 1, 'exp', 1/lambda);
Xmax = max(X);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = histplot(40, X, style=2);
if load_ref('%ans') then   pause,end,

x = linspace(0, max(Xmax), 100)';
%ans = plot2d(x, lambda * exp(-lambda * x), strf='000', style=5);
if load_ref('%ans') then   pause,end,

legend(['exponential random sample histogram','exact density curve']);
// Note: if you use the old graphic mode use instead
// legends(["exponential random sample histogram" "exact density curve"],[2 5],"ur");
xdel_run(winsid());

mclose(%U);
