//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLLI SHELL MODE -->

vec = 1:60;

res = ifftshift(vec);

assert_checkequal(res(1:30), 31:60);
assert_checkequal(res(31:60), 1:30);

mat = matrix(vec, [10 6]);
refRES = (1:5)';
refRES = [refRES refRES+10 refRES+20];

res = ifftshift(mat);

assert_checkequal(res(1:5, 1:3), refRES+35);
assert_checkequal(res(1:5, 4:6), refRES+5);
assert_checkequal(res(6:10, 1:3), refRES+30);
assert_checkequal(res(6:10, 4:6), refRES);


// Check that ifftshift(fftshift(x)) = x

// Make a signal
t = 0:0.1:1000;
x = 3*sin(t)+8*sin(3*t)+0.5*sin(5*t)+3*rand(t);
// Compute the fft
y = fft(x, -1);
// Invert the result
invShift = ifftshift(fftshift(y));
// Check that we recreated the original result
assert_checkequal(invShift, y);
