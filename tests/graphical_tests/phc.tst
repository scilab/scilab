getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/phc_data.ref','rb');
//
//This example may usefully be compared with the results from
//the 'levin' macro (see the corresponding help and example)
//
//We consider the process defined by two sinusoids (1Hz and 2 Hz)
//in additive Gaussian noise (this is the observation);
//the simulated process is sampled at 10 Hz.
//
t = 0:0.1:100;rand('normal');
y = sin(2 * %pi * t) + sin(2 * %pi * 2 * t);y = y + rand(y);%ans = plot(t, y);
if load_ref('%ans') then   pause,end,

//
//covariance of y
//
nlag = 128;
c = corr(y, nlag);
//
//hankel matrix from the covariance sequence
//(we can choose to take more information from covariance
//by taking greater n and m; try it to compare the results !
//
n = 20;m = 20;
h = hank(n, m, c);
//
//compute the Markov representation (mh,mf,mg)
//We just take here a state dimension equal to 4 :
//this is the rather difficult problem of estimating the order !
//Try varying ns !
//(the observation dimension is here equal to one)
ns = 4;
[mh,mf,mg] = phc(h, 1, ns);
//
//verify that the spectrum of mf contains the
//frequency spectrum of the observed process y
//(remember that y is sampled -in our example
//at 10Hz (T=0.1s) so that we need
//to retrieve the original frequencies through the log
//and correct scaling by the frequency sampling)
//
s = spec(mf);s = log(s);
s = s/2/%pi/0.1;
//
//now we get the estimated spectrum
%ans = imag(s);
if load_ref('%ans') then   pause,end,

//
xdel_run(winsid());

mclose(%U);
