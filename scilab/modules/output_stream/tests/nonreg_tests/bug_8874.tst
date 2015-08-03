// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 8874 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8874
//
// <-- ENGLISH IMPOSED -->

// <-- Short Description -->
// Wrong display of complex numbers with Inf or Nan real or Imaginary
// parts
setlanguage("en_US");
A=[complex(-1,%inf),complex(1,%nan),complex(%inf,%nan),complex(%nan,%nan);
1                2               3                  4];
As=sparse(A);
P=poly(A(1,:),"s","c");

format("v")
// Check string
assert_checkequal(string(complex(0,%inf)),"%i*Inf");
assert_checkequal(string(complex(1,%inf)),"1+%i*Inf");
assert_checkequal(string(complex(-1,%inf)),"-1+%i*Inf");
assert_checkequal(string(complex(1,%nan)),"1+%i*Nan");
assert_checkequal(string(complex(%inf,%nan)),"Inf+%i*Nan");
assert_checkequal(string(complex(-%inf,%nan)),"-Inf+%i*Nan");
assert_checkequal(string(complex(%nan,%nan)),"Nan+%i*Nan");


assert_checkequal(string(A),["-1+%i*Inf","1+%i*Nan","Inf+%i*Nan","Nan+%i*Nan";"1","2","3","4"]);


//Check display
complex(0,%inf)
complex(1,%inf)
complex(-1,%inf)
complex(1,%nan)

complex(%inf,%nan)
complex(-%inf,%nan)
complex(%nan,%nan)
A
As
P


format("e")
// Check string
assert_checkequal(string(complex(0,%inf)),"%i*Inf");
assert_checkequal(string(complex(1,%inf)),"1.000D+00+%i*Inf");
assert_checkequal(string(complex(-1,%inf)),"-1.000D+00+%i*Inf");
assert_checkequal(string(complex(1,%nan)),"1.000D+00+%i*Nan");
assert_checkequal(string(complex(%inf,%nan)),"Inf+%i*Nan");
assert_checkequal(string(complex(-%inf,%nan)),"-Inf+%i*Nan");
assert_checkequal(string(complex(%nan,%nan)),"Nan+%i*Nan");
assert_checkequal(string(A),["-1.000D+00+%i*Inf","1.000D+00+%i*Nan","Inf+%i*Nan","Nan+%i*Nan";
"1.000D+00","2.000D+00","3.000D+00","4.000D+00"]);

//Check display
complex(0,%inf)
complex(1,%inf)
complex(-1,%inf)
complex(1,%nan)

complex(%inf,%nan)
complex(-%inf,%nan)
complex(%nan,%nan)
A
As
P
