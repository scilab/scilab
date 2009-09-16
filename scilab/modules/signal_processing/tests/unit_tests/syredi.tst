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

// 'syredi' unit tests

test_path = pathconvert(SCI + '/modules/signal_processing/tests/unit_tests');

// requires test_utilities.sci
exec(test_path + 'test_utilities.sci');

// ****************************************************************
// * Part I: defining the tests                                   *
// * Part II: performing the tests                                *
// ****************************************************************

// **** Part I: defining the tests ****

// old tests wrapped
function test_syredi_old() 
[fact,b2,b1,b0,c1,c0,zzeros,zpoles]=syredi(1,4,[2*%pi/10,4*%pi/10,0,0],0.02,0.001);
ref_fact = 0.005976;
if norm(fact-ref_fact) > 0.0000037 then pause,end;
if or(b2<>[1 1 1]) then pause,end;
if or(size(b1)<>[1 3]) then pause,end;
if b1(3)<> 1 then pause,end;
if or(b0<>[1 1 0]) then pause,end;
if or(size(c1)<>[1 3]) then pause,end;
if or(size(c0)<>[1 3]) then pause,end;
if c0(3)<> 0 then pause,end;
if or(size(zzeros)<>[1 5]) then pause,end;
if or(size(zpoles)<>[1 5]) then pause,end;
endfunction

function test_syredi_new()
load(test_path + 'syredi.dat', 'fact1_ref', 'b21_ref', 'b11_ref', 'b01_ref', 'c11_ref', 'c01_ref', 'zzeros1_ref', 'zpoles1_ref', 'fact2_ref', 'b22_ref', 'b12_ref', 'b02_ref', 'c12_ref', 'c02_ref', 'zzeros2_ref', 'zpoles2_ref');
[fact1, b21, b11, b01, c11, c01, zzeros1, zpoles1] = syredi(1, 4, [1, 2, 0, 0], 0.02, 0.001);
check(list(fact1, b21, b11, b01, c11, c01, zzeros1, zpoles1), list(fact1_ref,b21_ref,b11_ref,b01_ref,c11_ref,c01_ref,zzeros1_ref,zpoles1_ref));
[fact2, b22, b12, b02, c12, c02, zzeros2, zpoles2] = syredi(3, 4, [1, 2, 3, 3.01], 0.1, 0.001);
check(list(fact2, b22, b12, b02, c12, c02, zzeros2, zpoles2), list(fact2_ref,b22_ref,b12_ref,b02_ref,c12_ref,c02_ref,zzeros2_ref,zpoles2_ref));
endfunction

// **** Part II: performing the tests ****

// wrap all tests together
function test_syredi()
test_syredi_old();
test_syredi_new();
endfunction

test_syredi();
