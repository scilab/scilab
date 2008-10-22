getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/remezb_data.ref','rb');
// Choose the number of cosine functions and create a dense grid
// in [0,.24) and [.26,.5)
nc = 21;ngrid = nc * 16;
fg = 0.24 * (0:ngrid/2 - 1)/(ngrid/2 - 1);
fg(ngrid/2 + 1:ngrid) = fg(1:ngrid/2) + 0.26 * ones(1:ngrid/2);
// Specify a low pass filter magnitude for the desired response
ds(1:ngrid/2) = ones(1:ngrid/2);
ds(ngrid/2 + 1:ngrid) = zeros(1:ngrid/2);
// Specify a uniform weighting function
wt = ones(fg);
// Run remezb
an = remezb(nc, fg, ds, wt);
if load_ref('an') then   pause,end,

// Make a linear phase FIR filter
hn(1:nc - 1) = an(nc:-1:2)/2;
hn(nc) = an(1);
hn(nc + 1:2 * nc - 1) = an(2:nc)/2;
// Plot the filter's magnitude response
plot(0.5 * (0:255)/256, frmag(hn, 256));
//////////////
// Choose the number of cosine functions and create a dense grid in [0,.5)
nc = 21;ngrid = nc * 16;
fg = 0.5 * (0:ngrid - 1)/ngrid;
// Specify a triangular shaped magnitude for the desired response
ds(1:ngrid/2) = (0:ngrid/2 - 1)/(ngrid/2 - 1);
ds(ngrid/2 + 1:ngrid) = ds(ngrid/2:-1:1);
// Specify a uniform weighting function
wt = ones(fg);
// Run remezb
an = remezb(nc, fg, ds, wt);
if load_ref('an') then   pause,end,

// Make a linear phase FIR filter
hn(1:nc - 1) = an(nc:-1:2)/2;
hn(nc) = an(1);
hn(nc + 1:2 * nc - 1) = an(2:nc)/2;
// Plot the filter's magnitude response
plot(0.5 * (0:255)/256, frmag(hn, 256));
xdel_run(winsid());

mclose(%U);
