// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2008 - INRIA - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

// With x as a row vector and 1 argument
data = [10 20 30 40 50 60 70 80 90];
[Variance, Mean] = variance(data);
expectedM = 50;
expectedV = 750;
assert_checkequal([Variance Mean], [expectedV expectedM]);
// With x as a column vector and 1 argument
data = [10; 20; 30; 40; 50; 60; 70; 80; 90];
[Variance, Mean] = variance(data);
expectedM = 50;
expectedV = 750;
assert_checkequal([Variance Mean], [expectedV expectedM]);
// With x as a matrix
data = [10 20 30 40; 50 60 70 90];
[Variance, Mean] = variance(data);
expectedM = 46.25;
expectedV = 712.5;
assert_checkequal([Variance Mean], [expectedV expectedM]);
// With x as a row vector and specified orient
[Variance, Mean] = variance(data, 1);
expectedM = [30 40 50 65];
expectedV = [800 800 800 1250];
assert_checkequal([Variance Mean], [expectedV expectedM]);
// With x as a row vector and specified orient
[Variance, Mean] = variance(data, "r");
expectedM = [30 40 50 65];
expectedV = [800 800 800 1250];
assert_checkequal([Variance Mean], [expectedV expectedM]);
// With x as a row vector and specified orient
[Variance, Mean] = variance(data, 2);
expectedM = [50; 135]/2;
expectedV = [500; 875]/3;
assert_checkequal([Variance Mean], [expectedV expectedM]);
// With x as a row vector and specified orient
[Variance, Mean] = variance(data, "c");
expectedM = [50; 135]/2;
expectedV = [500; 875]/3;
assert_checkequal([Variance Mean], [expectedV expectedM]);

// With x as a complex row vector and 1 argument
a = [
0.9, 0.7;
0.1, 0.1;
0.5, 0.4 ];
data = a + a * 2 * %i;
[Variance, Mean] = variance(data);
expectedM = 0.45+0.9*%i;
expectedV = 0.515;
assert_checkequal([Variance Mean], [expectedV expectedM]); // Must be variance(real(data)) + variance(imag(data))

// With x as a complex row vector and computation by column
data = a + a * 2 * %i;
[Variance, Mean] = variance(data, 1);
expectedM = [0.5+%i 0.4+0.8*%i];
expectedV = [0.8 0.45];
assert_checkalmostequal([Variance Mean], [expectedV expectedM]);
// With x as a complex row vector and computation by row
data = a + a * 2 * %i;
[Variance, Mean] = variance(data, 2);
expectedM = [0.8+1.6*%i; 0.1+0.2*%i; 0.45+0.9*%i];
expectedV = [0.1; 0; 0.025];
assert_checkalmostequal([Variance Mean], [expectedV expectedM]);

// Normalization with N-1, no a priori mean
x = [
0.9    0.7
0.1    0.1
0.5    0.4 ];
orien = 1;
[Variance, Mean] = variance(x,orien);
expectedM = [0.5 0.4];
expectedV = [0.16 0.09];
assert_checkalmostequal([Variance Mean], [expectedV expectedM]);
// Normalization with N, the a priori can be known
[Variance, Mean] = variance(x,orien,mean(x,orien));
expectedM = [0.5 0.4];
expectedV = [0.32 0.18]/3;
assert_checkalmostequal([Variance Mean], [expectedV expectedM]);
// Biased variance
[Variance, Mean] = variance(x,orien,%nan);
expectedM = [0.5 0.4];
expectedV = [0.32 0.18]/3;
assert_checkalmostequal([Variance Mean], [expectedV expectedM]);
