// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Digiteo - unknown author
// Copyright (C) 2009 - Digiteo - Vincent LIARD
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

test_path = SCI + "/modules/signal_processing/tests/unit_tests/";
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
function z = xx(inc, is)
    z = x(is:is+inc-1);
endfunction
function z = yy(inc, is)
    z=y(is:is+inc-1);
endfunction
[c3,m3]=corr('fft',xx,yy,256,32);
if norm(c1-c3,1) > 10 *%eps then pause,end
[c4,m4]=corr('fft',xx,256,32);
if or(size(c4)<>[1 32]) then pause,end
if or(size(m4)<>[1 1]) then pause,end
if norm(m3,1) > %eps then pause,end
if norm(m4,1) > %eps then pause,end
if norm(c3-c1,1) > 10*%eps then pause,end
if norm(c4-c2,1) > 10*%eps then pause,end

// corr default mode
x = [1:10];
y = [5 10 6 4 8 2 9 3 1 7];
nlags = 3;
// x, nlag provided
cov_ref1 = [8.25 5.775 3.4];
Mean_ref1 = 5.5;
cov = corr(x, nlags);
if cov <> cov_ref1 then pause,end

[cov, Mean] = corr(x, nlags);
if or([cov, Mean] <> [cov_ref1, Mean_ref1]) then pause,end
// x, y, nlag provided
cov_ref2 = [-2.55 -2.825 -0.4];
Mean_ref2 = [5.5 5.5];
cov = corr(x, y, nlags);
if cov <> cov_ref2 then pause,end
[cov, Mean] = corr(x, y, nlags);
if or([cov, Mean] <> [cov_ref2, Mean_ref2]) then pause,end

// corr updates mode
import_from_hdf5(test_path + 'corr_updates_ref.h5');
[w1,xu1] = corr('updt', [1:10], zeros(1,8));
if or([w1, xu1] <> [w1_ref, xu1_ref]) then pause,end
[w2, xu2] = corr('updt', [1:8], [%i %i %i %i]);
of or([w2, xu2] <> [w2_ref, xu2_ref]) then pause,end
[w3, xu3] = corr('updt', [1:8], [1:8], zeros(1,8));
if or([w3, xu3] <> [w3_ref, xu3_ref]) then pause,end
[w4, xu4] = corr('updt', [1:10], [1:10],zeros(1,8));
if or([w4, xu4] <> [w4_ref, xu4_ref]) then pause,end
[w5, xu5] = corr('updt', [1:10], [1:10],ones(1,8));
if or([w5, xu5] <> [w5_ref, xu5_ref]) then pause,end
[w6, xu6] = corr('updt', [1:10], [5 10 6 4 8 2 9 3 1 7], zeros(1,8));
if or([w6, xu6] <> [w6_ref, xu6_ref]) then pause,end
[w7, xu7] = corr('updt', [1:8], [1:8], zeros(1,8), zeros(1,8));
if or([w7, xu7] <> [w7_ref, xu7_ref]) then pause,end
[w8, xu8] = corr('updt', [1:8], [1:8], zeros(1,8), 0);
if or([w8, xu8] <> [w8_ref, xu8_ref]) then pause,end
[w9, xu9] = corr('updt', [1:8], [1:8], zeros(1,8), [5 2 6 5 3 7 4 5]);
if or([w9, xu9] <> [w9_ref, xu9_ref]) then pause,end

// corr fft mode
import_from_hdf5(test_path + 'corr_fft_ref.h5');
y = -x;
function z = xx(inc, is)
    z = x(is:is+inc-1)
endfunction
function z = yy(inc,is)
    z = y(is:is+inc-1)
endfunction

[c1, m1] = corr('fft', xx, 64, 4);
if or([c1, m1] <> [c1_ref, m1_ref]) then pause,end
[c2, m2] = corr('fft', xx, yy, 64, 4);
if or([c2, m2] <> [c2_ref, m2_ref]) then pause,end
