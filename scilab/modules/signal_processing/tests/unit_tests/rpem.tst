// -*- scilab -*-

// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Vincent Liard
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// RPEM unit tests

test_path = pathconvert(SCI + '/modules/signal_processing/tests/unit_tests');

// requires test_utilities.sci
exec(test_path + 'test_utilities.sci');

format(25);

n = 2;
theta=zeros(1,3*n);
p=eye(3*n,3*n);
phi=theta;
psi=theta;
l=theta;
w0=list(theta, p, l, phi, psi);
u0=[1 2 3 4];
y0=[5 2 7 3];
lambda=[1 1 1];
kappa=[1 1 1];
c=500;

// test 1 ****************************************************************
load(test_path + 'rpem_ref1.dat', 'w1_ref1', 'v_ref1');

check(rpem(w0, u0, y0), w1_ref1);

[w1,v] = rpem(w0, u0, y0);
check(w1, w1_ref1);
check(v, v_ref1);
// test 2 ****************************************************************
load(test_path + 'rpem_ref2.dat', 'w1_ref2', 'v_ref2');

check(rpem(w0, u0, y0, lambda), w1_ref2);

[w1,v] = rpem(w0, u0, y0, lambda);
check(w1, w1_ref2);
check(v, v_ref2);
// test 3 ****************************************************************
load(test_path + 'rpem_ref3.dat', 'w1_ref3', 'v_ref3');

check(rpem(w0, u0, y0, lambda, kappa), w1_ref3);

[w1,v] = rpem(w0, u0, y0, lambda, kappa);
check(w1, w1_ref3);
check(v, v_ref3);
// test 4 ****************************************************************
load(test_path + 'rpem_ref4.dat', 'w1_ref4', 'v_ref4');

check(rpem(w0, u0, y0, lambda, kappa, c), w1_ref4);

[w1,v] = rpem(w0, u0, y0, lambda, kappa, c);
check(w1, w1_ref4);
check(v, v_ref4);
//****************************************************************
