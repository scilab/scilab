// -*- scilab -*-

// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Digiteo - unknown author
// Copyright (C) 2009 - Digiteo - Vincent LIARD
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// 'corr' unit tests

test_path = pathconvert(SCI + '/modules/signal_processing/tests/unit_tests');

// requires test_utilities.sci
exec(test_path + 'test_utilities.sci');

// ****************************************************************
// * Part I: defining the tests                                   *
// * Part II: performing the tests                                *
// ****************************************************************

// **** Part I: defining the tests ****

// old tests wrapped
function test_corr_old() 
rand('normal');
x = rand(1,256);
y = -x;
[c, mxy]=corr(x,y,32);
if or(size(c)<>[1 32]) then pause,end
if or(size(mxy)<>[1 2]) then pause,end
x = x-mxy(1)*ones(x);
y = y-mxy(2)*ones(y);
c1 = corr(x,y,32);
c2 = corr(x,32);
if norm(c1+c2,1) > %eps then pause,end
deff('[z]=xx(inc,is)','z=x(is:is+inc-1)');
deff('[z]=yy(inc,is)','z=y(is:is+inc-1)');
[c3,m3]=corr('fft',xx,yy,256,32);
if norm(c1-c3,1) > 10 *%eps then pause,end
[c4,m4]=corr('fft',xx,256,32);
if or(size(c4)<>[1 32]) then pause,end
if or(size(m4)<>[1 1]) then pause,end
if norm(m3,1) > %eps then pause,end
if norm(m4,1) > %eps then pause,end
if norm(c3-c1,1) > 10*%eps then pause,end
if norm(c4-c2,1) > 10*%eps then pause,end
endfunction

// corr default mode
function test_corr_default()
x = [1:10];
y = [5 10 6 4 8 2 9 3 1 7];
nlags = 3;
// x, nlag provided
cov_ref1 = [8.25 5.775 3.4];
Mean_ref1 = 5.5;
cov = corr(x, nlags);
check(cov, cov_ref1);
[cov, Mean] = corr(x, nlags);
check([cov, Mean], [cov_ref1, Mean_ref1]);
// x, y, nlag provided
cov_ref2 = [-2.55 -2.825 -0.4];
Mean_ref2 = [5.5 5.5];
cov = corr(x, y, nlags);
check(cov, cov_ref2);
[cov, Mean] = corr(x, y, nlags);
check([cov, Mean], [cov_ref2, Mean_ref2]);
endfunction

// corr updates mode
function test_corr_updates()
load(test_path + 'corr_updates_ref.dat',..
    'w1_ref', 'xu1_ref', 'w2_ref', 'xu2_ref', 'w3_ref', 'xu3_ref',..
    'w4_ref', 'xu4_ref', 'w5_ref', 'xu5_ref', 'w6_ref', 'xu6_ref',..
    'w7_ref', 'xu7_ref', 'w8_ref', 'xu8_ref', 'w9_ref', 'xu9_ref');
[w1,xu1] = corr('updt', [1:10], zeros(1,8));
check([w1, xu1], [w1_ref, xu1_ref]);
[w2, xu2] = corr('updt', [1:8], [%i %i %i %i]);
check([w2, xu2], [w2_ref, xu2_ref]);
[w3, xu3] = corr('updt', [1:8], [1:8], zeros(1,8));
check([w3, xu3], [w3_ref, xu3_ref]);
[w4, xu4] = corr('updt', [1:10], [1:10],zeros(1,8));
check([w4, xu4], [w4_ref, xu4_ref]);
[w5, xu5] = corr('updt', [1:10], [1:10],ones(1,8));
check([w5, xu5], [w5_ref, xu5_ref]);
[w6, xu6] = corr('updt', [1:10], [5 10 6 4 8 2 9 3 1 7], zeros(1,8));
check([w6, xu6], [w6_ref, xu6_ref]);
[w7, xu7] = corr('updt', [1:8], [1:8], zeros(1,8), zeros(1,8));
check([w7, xu7], [w7_ref, xu7_ref]);
[w8, xu8] = corr('updt', [1:8], [1:8], zeros(1,8), 0);
check([w8, xu8], [w8_ref, xu8_ref]);
[w9, xu9] = corr('updt', [1:8], [1:8], zeros(1,8), [5 2 6 5 3 7 4 5]);
check([w9, xu9], [w9_ref, xu9_ref]);
endfunction

// corr fft mode
function test_corr_fft()
load(test_path + 'corr_fft_ref.dat',..
    'x', 'c1_ref', 'm1_ref', 'c2_ref', 'm2_ref');
// x = rand(1,64);
y = -x;
deff('[z]=xx(inc,is)', 'z=x(is:is+inc-1)');
deff('[z]=yy(inc,is)', 'z=y(is:is+inc-1)');
[c1, m1] = corr('fft', xx, 64, 4);
check([c1, m1], [c1_ref, m1_ref]);
[c2, m2] = corr('fft', xx, yy, 64, 4);
check([c2, m2], [c2_ref, m2_ref]);
endfunction

// **** Part II: performing the tests ****

// wrap all tests together
function test_corr()
test_corr_old();
test_corr_default();
test_corr_updates();
test_corr_fft();
endfunction

test_corr();
