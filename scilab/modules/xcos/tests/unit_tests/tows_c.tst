// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/tows_c.zcos"));
Info = scicos_simulate(scs_m);

// Reference values
t = (2.2 : 0.1 : 14.9)';
A_ref = struct("values", sin(t), "time", t); // A_ref = (sin(t), t)
imgM_ref = [1+2*%i 3+4*%i ; 1 2]; // imgM_ref is an element of hypermatrix imgM
imgV_ref = ones(128, 2);
imgV_ref = (1+%i)*imgV_ref;
imgV_ref(:, 2) = 2*imgV_ref(:, 2); // imgV_ref = [1+%i 2+%i] with 128 rows
intM1_ref = int32([1 2 ; 3 4]);    // intM1_ref is an element of hypermatrix intM1
intV_ref = int32(ones(128, 2));
intV_ref = 5*intV_ref;
intV_ref(:, 2) = intV_ref(:, 2) + 1;     // intV_ref = [5, 6] with 128 rows
shortV_ref = int16(ones(128, 2));
shortV_ref = 7*shortV_ref;
shortV_ref(:, 2) = shortV_ref(:, 2) + 1; // shortV_ref = [7, 8] with 128 rows
charV_ref = int8(ones(128, 2));
charV_ref = 9*charV_ref;
charV_ref(:, 2) = charV_ref(:, 2) + 1;   // charV_ref = [9, 10] with 128 rows
intM2_ref = int32([11 12 ; 13 14]);      // intM2_ref is an element of hypermatrix intM2
shortM_ref = int16([15 16; 17 18]);      // shortM_ref is an element of hypermatrix shortM
charM_ref = int8([19 20 ; 21 22]);       // charM_ref is an element of hypermatrix charM
nineteen_ref = struct("values", ones(128,1), "time", t);  // Checks that a ninteteen characters variable name is possible, with correct time values

// Run simulation with scicos_simulate() + check results
try scicos_simulate(scs_m, Info); catch disp(lasterror()); end  // Run simulation
assert_checkequal(fieldnames(A), fieldnames(A_ref)); // Check A fields
assert_checkalmostequal(A.values, A_ref.values);     // Check A values
assert_checkalmostequal(A.time, A_ref.time);         // Check A time values
assert_checkequal(fieldnames(B), fieldnames(A_ref)); // Check B fields
assert_checkalmostequal(B.values, A_ref.values);     // Check B values
assert_checkequal(fieldnames(C), fieldnames(A_ref)); // Check C fields
assert_checkalmostequal(C.values, A_ref.values);     // Check C values

assert_checktrue(imgV.values   == imgV_ref);    // Check that imgV contains a complex vector
assert_checktrue(intV.values   == intV_ref);    // Check that intV contains an int32 vector
assert_checktrue(shortV.values == shortV_ref);  // Check that shortV contains an int16 vector
assert_checktrue(charV.values  == charV_ref);   // Check that charV contains an int8 vector
for i = 1:128
    // Check that the following are hypermatrices with correct values
    assert_checktrue(and(imgM.values(:, :, i)   == imgM_ref));
    assert_checktrue(and(intM1.values(:, :, i)  == intM1_ref));
    assert_checktrue(and(intM2.values(:, :, i)  == intM2_ref));
    assert_checktrue(and(shortM.values(:, :, i) == shortM_ref));
    assert_checktrue(and(charM.values(:, :, i)  == charM_ref));
end
assert_checkalmostequal(nineteen19Letters19.values, nineteen_ref.values);
assert_checkalmostequal(nineteen19Letters19.time, nineteen_ref.time);

// Run simulation with xcos_simulate() + check results
try xcos_simulate(scs_m, 4); catch disp(lasterror()); end  // Run simulation
assert_checkequal(fieldnames(A), fieldnames(A_ref)); // Check A fields
assert_checkalmostequal(A.values, A_ref.values);     // Check A values
assert_checkalmostequal(A.time, A_ref.time);         // Check A time values
assert_checkequal(fieldnames(B), fieldnames(A_ref)); // Check B fields
assert_checkalmostequal(B.values, A_ref.values);     // Check B values
assert_checkequal(fieldnames(C), fieldnames(A_ref)); // Check C fields
assert_checkalmostequal(C.values, A_ref.values);     // Check C values

assert_checktrue(imgV.values   == imgV_ref);    // Check that imgV contains a complex vector
assert_checktrue(intV.values   == intV_ref);    // Check that intV contains an int32 vector
assert_checktrue(shortV.values == shortV_ref);  // Check that shortV contains an int16 vector
assert_checktrue(charV.values  == charV_ref);   // Check that charV contains an int8 vector
for i = 1:128
    // Check that the following are hypermatrices with correct values
    assert_checktrue(and(imgM.values(:, :, i)   == imgM_ref));
    assert_checktrue(and(intM1.values(:, :, i)  == intM1_ref));
    assert_checktrue(and(intM2.values(:, :, i)  == intM2_ref));
    assert_checktrue(and(shortM.values(:, :, i) == shortM_ref));
    assert_checktrue(and(charM.values(:, :, i)  == charM_ref));
end
assert_checkalmostequal(nineteen19Letters19.values, nineteen_ref.values);
assert_checkalmostequal(nineteen19Letters19.time, nineteen_ref.time);

// Type checks
assert_checkequal(type(A.values), 1);              // A takes real numbers
assert_checkequal(type(imgV.values(1)), 1);       // imgV takes complex numbers
assert_checkequal(type(imgM.values(:, :, 1)), 1); // imgM takes complex numbers
assert_checkequal(type(intM1.values(1)), 8);
assert_checkequal(type(intV.values(1)), 8);
assert_checkequal(type(shortV.values(1)), 8);
assert_checkequal(type(charV.values(1)), 8);
assert_checkequal(type(intM2.values(1)), 8);
assert_checkequal(type(shortM.values(1)), 8);
assert_checkequal(type(charM.values(1)), 8);
assert_checkequal(inttype(intM1.values(1)), 4);  // intM1 takes 4-octets coded integers
assert_checkequal(inttype(intV.values(1)), 4);   // intV takes 4-octets coded integers
assert_checkequal(inttype(shortV.values(1)), 2); // shortV takes 2-octets coded integers
assert_checkequal(inttype(charV.values(1)), 1);  // charV takes 1-octets coded integers
assert_checkequal(inttype(intM2.values(1)), 4);
assert_checkequal(inttype(shortM.values(1)), 2);
assert_checkequal(inttype(charM.values(1)), 1);
