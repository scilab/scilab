// =============================================================================
// Copyright (C) 2011 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - INRIA - Allan CORNET
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
function y = writereadcycle(x,filename)
    csvWrite(x,filename);
    separator = [];
    decimal=[];
    conversion ="double";
    y = csvRead(filename, separator, decimal, conversion);
endfunction

// Test write-read cycles
//
// =============================================================================
// Basic.
K = eye(3, 2) + 0.1;
filename = fullfile(TMPDIR,"test.csv");
K2 = writereadcycle(K,filename);
assert_checkequal ( K , K2 );
// =============================================================================
// Numerical issues.
huge=number_properties("huge");
tiny=number_properties("tiny");
tiniest=number_properties("tiniest");
x1 = nearfloat("succ",1);
x2 = nearfloat("pred",1);
K = [
   %inf %nan  huge  tiny  tiniest  x1  x2  0
  -%inf %nan -huge -tiny -tiniest -x1 -x2 -0
];
K2 = writereadcycle(K,filename);
assert_checkequal ( K , K2 );
// =============================================================================
// Complexes and numerical issues in complexes.
K = [
  1+2*%i                -1+0.5*huge*%i     2*tiny*%i          1
  1+0.1*%i              complex(%inf,%nan) complex(%nan,%inf) complex(tiniest,%inf)
  complex(%inf,tiniest) complex( 0  ,%nan) complex(%nan,0)    -1
];
K2 = writereadcycle(K,filename);
assert_checkequal ( K , K2 );
// =============================================================================
