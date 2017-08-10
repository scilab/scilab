// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->


format("v",10);

dmax = -log(2^(-53))/log(10);

digits =  assert_computedigits(1, 1);
assert_checkalmostequal(digits, dmax);

digits =  assert_computedigits(0, 0);
assert_checkalmostequal(digits, dmax);

digits =  assert_computedigits(1, 0);
assert_checkalmostequal(digits, 0);

digits =  assert_computedigits(0, 1);
assert_checkalmostequal(digits, 0);

digits =  assert_computedigits(1.1, 1.0);
assert_checkalmostequal(digits, 1);

digits =  assert_computedigits(0.9, 1.0);
assert_checkalmostequal(digits, 1);

digits =  assert_computedigits(3.1415926, %pi);
assert_checkalmostequal(digits, 7.7680677928004016);

for p=1:8
  e = 10^p;
  c = e + 0.1;
  digits =  assert_computedigits(c, e);
  assert_checkalmostequal(digits, p+1);

  if p>1
    c = strtod("1." + strcat(repmat("0", 1, p-1)) + "1");
  else
    c = 1.1;
  end
  e = 1;
  digits =  assert_computedigits(c, e);
  assert_checkalmostequal(digits, p);

  e = 10^-p;
  c = e + 10^(-p-1);
  digits =  assert_computedigits(c, e);
  assert_checkalmostequal(digits, 1);

  digits =  assert_computedigits(c, c);
  assert_checkalmostequal(digits, dmax);
end

// check special values
digits =  assert_computedigits([%nan %nan %nan %nan], [%nan %inf -%inf 10.]);
assert_checkalmostequal(digits, [dmax 0 0 0]);

digits =  assert_computedigits([%inf %inf %inf %inf], [%nan %inf -%inf 10.]);
assert_checkalmostequal(digits, [0 dmax 0 0]);

digits =  assert_computedigits([-%inf -%inf -%inf -%inf], [%nan %inf -%inf 10.]);
assert_checkalmostequal(digits, [0 0 dmax 0]);

digits =  assert_computedigits([10. 10. 10. 10.], [%nan %inf -%inf 10.]);
assert_checkalmostequal(digits, [0 0 0 dmax]);

digits =  assert_computedigits(1.224646799D-16, 8.462643383D-18);
assert_checkalmostequal(digits, 0);

// check vector
digits =  assert_computedigits([0 0 1 1], [0 1 0 1]);
assert_checkalmostequal(digits, [dmax 0 0 dmax]);

digits =  assert_computedigits(ones(3,2), ones(3,2));
assert_checkalmostequal(digits, dmax * ones(3,2));

c = [
10.001
10.01
nearfloat("succ", 10)
10.0
3.14159
%nan
%nan
10^15
-10^15
%inf
];

e = [
10.
10.
10.
10.
%pi
10.0
%nan
%inf
-%inf
%inf
];

expected_digits = [
4.
3.
-log(2^(-52))/log(10)
dmax
6.0733160846496004
0
dmax
0
0
dmax
];
digits =  assert_computedigits(c, e);
assert_checkalmostequal(digits, expected_digits);

// check with vector random permutation
perm = grand(1, "prm", size(e, 'r'));
cP = c(perm);
eP = e(perm);
digitsP = assert_computedigits(cP, eP);
expected_digitsP = expected_digits(perm);
assert_checkalmostequal(digitsP, expected_digitsP);

// base 2
digits =  assert_computedigits(1.5, 1, 2);
assert_checkalmostequal(digits, 1);

digits =  assert_computedigits(3.1415926, %pi, 2);
assert_checkalmostequal(digits, 25.804963);

c = [
3.982729777831130693D-59
2.584939414228211484D-26
4.391531370352049090D+43
1.725436586898508346D+68
];
e = [
3.982729777831130693D-59
2.584939414228211484D-26
4.391531370352048595D+43
1.725436586898508107D+68
];
digits = assert_computedigits(c, e, 2);
expected_digits = [
53.
53.
52.
52.
];
assert_checkalmostequal(digits, expected_digits);

// check complex values
digits =  assert_computedigits(1.2345 + %i*6.7891, 1.23456789 + %i*6.789123456);
assert_checkalmostequal(digits, 4.2597091684951387);

// the sign bit of the number of digits may be wrong because
// ieee(2); z=max(-0,0); 1/z is -%inf
back = ieee();
ieee(2);
digits =  assert_computedigits(1.e-305, 0);
assert_checkalmostequal(1/digits, %inf);

digits =  assert_computedigits(0, 1.e-305);
assert_checkalmostequal(1/digits, %inf);
ieee(back);




