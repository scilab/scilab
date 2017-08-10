hs: decimal numbers
hsc: complex numbers
hi: integers
hb: booleans
hc: chars
hp: polynomials
hpc: complex polynomials
hr: rationals
hrc: complex rationals

// hypermatrices used for the tests:
hs = grand(2,2,2,"uin",0,10);
hsc = hs + %i;
hb = hs<5;
hi = int8(hs);
hp = (hs +%z).^2;
hpc = hp+%i;
hr = hp ./ (hp+1);
hrc = hr+%i;
hc = string(hs);

EXTENSIVE TESTS RESULTS:
* %hm_and() is still used
* %hm_conj() is unused, but conj(hr) is very unstable and crashes Scilab 9 times / 10. conj(hrc) works without any problem
* cumsum(hr) => error in %r_cumsum
* find(hi) unsupported (==5.5.2), whereas find(hs) is
* max(hr)  => %c_max invoked in error_message instead of %r_max (%c_max is actually wrongly called)
* min(hr)  => %c_min invoked in error_message instead of %r_min (%c_min is actually wrongly called)
* min(hs, dim>2) and max(hs, dim>2) are not implemented and they return hs instead of an error.
  It was the same in 5.5.2
* median(hr) is accepted but should be rejected. It yields an error with unclear indirect message.
* %hm_or() is still used
* %hm_stdev() is still called
* %hm_string is not called, but string(hr) yields an error: FIX: https://codereview.scilab.org/18443
* zeros(hr) yields an error: FIX: https://codereview.scilab.org/18442
* elementary_functions => %hm_gsort.sci is still called
