// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// unit tests for x_matrix() function
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->

i = int8(grand(3,4,"uin",-150,150))

// With booleans
// -------------
b = i > 0
m = x_matrix("<html><body>Matrix of <b>booleans", b)
assert_checkequal(type(m), 4);
assert_checkequal(size(m), size(b));

// With encoded integers
// ---------------------
m = x_matrix("<html><body>Matrix of <b>int8 integers", i)
assert_checkequal(inttype(m), inttype(i));
assert_checkequal(size(m), size(i));

// With real numbers
// -----------------
c = grand(5,4,"uin",-15,15) + grand(5,4,"uin",0,9)/10;
c([14 8 16]) = [%nan %inf -%inf]
m = x_matrix("<html><body>Matrix of <b>decimal numbers", c)

// With complex numbers
// --------------------
c = grand(5,3,"uin",-15,15) + grand(5,3,"uin",-15,15)*%i;
c(2,2) = 3-1.24e-31*%i;
c(4,3) = complex(0,%nan)
m = x_matrix("<body>Matrix of <b>complex numbers", c)

// With text
// ---------
t = ["ab" "I don''t" "αβδεϵζ"
     "j"  """Yes"""  "àéêèïôûù"]
m = x_matrix("<body>Matrix of <b>text", t)
assert_checkequal(type(m), 10);
     
