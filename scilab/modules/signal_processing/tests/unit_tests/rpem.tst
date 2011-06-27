// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

test_path = SCI + '/modules/signal_processing/tests/unit_tests/';

n = 2;
theta = zeros(1,3*n);
p = eye(3*n,3*n);
phi = theta;
psi = theta;
l = theta;
w0 = list(theta, p, l, phi, psi);
u0 = [1 2 3 4];
y0 = [5 2 7 3];
lambda = [1 1 1];
kappa = [1 1 1];
c = 500;

import_from_hdf5(test_path + "rpem_ref1.h5");
w1 = rpem(w0, u0, y0);
if w1 <> w1_ref then pause, end
[w1,v] = rpem(w0, u0, y0);
if w1 <> w1_ref1 then pause, end
if v <> v_ref1 then pause, end

import_from_hdf5(test_path + "rpem_ref2.h5");
w1 = rpem(w0, u0, y0, lambda);
if w1 <> w1_ref2 then pause, end

[w1,v] = rpem(w0, u0, y0, lambda);
if w1 <> w1_ref2 then pause, end
if v <> v_ref2 then pause, end

import_from_hdf5(test_path + "rpem_ref3.h5");

w1 = rpem(w0, u0, y0, lambda, kappa);
if w1 <> w1_ref3 then pause, end

[w1,v] = rpem(w0, u0, y0, lambda, kappa);
if w1 <> w1_ref3 then pause, end
if v <> v_ref3 then pause, end

import_from_hdf5(test_path + "rpem_ref4.h5");
w1 = rpem(w0, u0, y0, lambda, kappa, c);
if w1 <> w1_ref4 then pause, end

[w1,v] = rpem(w0, u0, y0, lambda, kappa, c);
if w1 <> w1_ref4 then pause, end
if v <> v_ref4 then pause, end
